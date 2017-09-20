/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "MyHwPsoc.h"



uint8_t eeprom[256];

uint8_t spi_txbuff[RFM69_MAX_PACKET_LEN + 1];
uint8_t spi_rxbuff[RFM69_MAX_PACKET_LEN + 1];

uint32_t MyHwPsoc_Random(uint32_t min, uint32_t max)
{
    uint32_t rand = PRS_Read();
    return (rand * (max - min) / 65535 + min);
}

void MyHwPsoc_init(void)
{
    PRS_Start();
    TIMER1_Start();
    SPI_RFM69HW_Start();
    //NRF24_SPI_Start();
    //NRF24_SPI_SpiUartClearTxBuffer();
    //NRF24_SPI_SpiUartClearRxBuffer();
    MyHwPsoc_SetOutput(RX_LED, 1);
    MyHwPsoc_SetOutput(TX_LED, 1);
    MyHwPsoc_SetOutput(ERR_LED, 1);
    MyHwPsoc_SerialBegin();
#if !defined(MY_DISABLED_SERIAL)
    UART_Start();
#endif
	//EEPROM.begin(EEPROM_size);
}

void MyHwPsoc_RandomNumberInit(void)
{
    PRS_Init();
    CyDelay(1u);
    PRS_Step();
}

void MyHwPsoc_ReadConfigBlock(void* buf, void* addr, size_t length)
{
    uint8_t* dst = (uint8_t*) buf;
    int pos = (int)addr;
    while(length-- > 0) {
        *dst++ = eeprom[pos++];
    } 
}

uint8_t MyHwPsoc_ReadConfig(const int addr)
{
    uint8_t value;
    MyHwPsoc_ReadConfigBlock( &value, (void*) addr, 1);
    return value;
}
 
void MyHwPsoc_WriteConfigBlock(void* buf, void* addr, size_t length)
{
    uint8_t* src = (uint8_t*)buf;
    int pos = (int)addr;
    while(length-- > 0) {
        eeprom[pos++] = *src++;
    }
}
    
void MyHwPsoc_WriteConfig(const int addr, uint8_t buf)
{
    MyHwPsoc_WriteConfigBlock(&buf, (void*)addr, 1);
}

void MyHwPsoc_Reboot(void)
{
}

int8_t MyHwPsoc_hwSleep(unsigned long ms)
{
    // TODO: Not supported!
	(void)ms;
    return MY_SLEEP_NOT_POSSIBLE;
}

int8_t MyHwPsoc_hwSleep1(uint8_t interrupt, uint8_t mode, unsigned long ms)
{
    // TODO: Not supported!
	(void)interrupt;
	(void)mode;
	(void)ms;
    return MY_SLEEP_NOT_POSSIBLE;
}

int8_t MyHwPsoc_hwSleep2(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms)
{
    // TODO: Not supported!
	(void)interrupt1;
	(void)mode1;
	(void)interrupt2;
	(void)mode2;
	(void)ms;
    return MY_SLEEP_NOT_POSSIBLE;
}


void MyHwPsoc_SetOutput(outputs pin, uint8_t state)
{
    switch(pin)
    {
        case RX_LED:
            LED_1_Write(state);
            break;
        case TX_LED:
            LED_2_Write(state);
            break;
        case ERR_LED:
            LED_3_Write(state);
            break;
        case NRF24_CSN:
            //NRF24_CS_Write(state);
            break;
        case NRF24_CE:
            //NRF24_CE_Write(state);
            break;
    }
}

uint8_t MyHwPsoc_SpiTransfer(uint8_t data)
{
    /*NRF24_SPI_SpiUartWriteTxData(data);
    CyDelayUs(20);
    while(NRF24_SPI_SpiUartGetTxBufferSize());    
    //NRF24_SPI_SpiUartClearTxBuffer();
    return NRF24_SPI_SpiUartReadRxData();*/
}

void MyHwPsoc_SpiEndTransaction(void)
{
 
}

uint8_t MyHwPsoc_RFM69_spiMultiByteTransfer(const uint8_t cmd, uint8_t* buf, uint8_t len, const bool aReadMode)
{
    uint8_t status;
    uint8_t* current = buf;
    uint8_t* ptx = &spi_txbuff[0];
    uint8_t* prx = &spi_rxbuff[0];
    uint8_t size = len + 1;
    uint32 i;
    
    *ptx++ = cmd;
    while(len--)
    {
        if(aReadMode)
        {
            *ptx++= (uint8_t)RFM69_NOP;
        }
        else
        {
            *ptx++= *current++;
        }
    }
    SPI_RFM69HW_SpiUartPutArray(ptx,size);
    
    while(size != SPI_RFM69HW_SpiUartGetRxBufferSize())
    {
    }
    /* Clear dummy bytes from TX buffer */
    SPI_RFM69HW_SpiUartClearTxBuffer();
   
    i= 0u;
    while(0u != SPI_RFM69HW_SpiUartGetRxBufferSize())
    {
        *prx++ = SPI_RFM69HW_SpiUartReadRxData();
        i++;
    }
    if (aReadMode)
    {
        if (size ==2)
        {
            status = *++prx; //result is 2nd byte of receive buffer
        }
        else
        {
            status = *prx++; //status is 1st byte of receive buffer
            //decrement before to skip status byte
            while (--size)
            {
                *buf++ = *prx++;
            }
        }
    }
    else
    {
        status = *prx; //status is 1st byte of receive buffer
    }
    return status;
}

// low level register access
uint8_t MyHwPsoc_RFM69_RAW_readByteRegister(const uint8_t address)
{
	const uint8_t value =  MyHwPsoc_RFM69_spiMultiByteTransfer(address, NULL, 1, true);
	//RFM69_DEBUG(PSTR("RFM69:read register, reg=0x%02" PRIx8 ", value=%" PRIu8 "\n"), address, value);
	return value;
}

// low level register access
uint8_t MyHwPsoc_RFM69_RAW_writeByteRegister(const uint8_t address, uint8_t value)
{
	//RFM69_DEBUG(PSTR("RFM69:write register, reg=0x%02" PRIx8 ", value=%" PRIu8 "\n"), address & 0x7F, value);
	return MyHwPsoc_RFM69_spiMultiByteTransfer(address, &value, 1, false);
}



uint16_t MyHwPsoc_hwMillis(void)
{
    uint16_t hwMillis = TIMER1_ReadCounter();
    return hwMillis;
}

void MyHwPsoc_SerialBegin(void)
{
    bool flag = true;
    #ifdef USB  
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);    
    while(flag)
    {
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USBUART_CDC_Init();
                flag = false;
                CyDelay(2000);
            }
        }
    }
    #else
    UART_Start();
    #endif
}

void MyHwPsoc_SerialPrint(char *buf)
{
    #ifdef USB
    while(!USBUART_CDCIsReady());
    USBUART_PutString(buf);
    //USBUART_PutCRLF();
    #else
    UART_UartPutString(buf);
    #endif
    
    return;
}

uint8_t MyHwPsoc_SerialAvailable(void)
{
    #ifdef USB
    return USBUART_GetCount();   
    #else
    return UART_SpiUartGetRxBufferSize();
    #endif
    
}

uint8_t MyHwPsoc_SerialRead(void)
{
    #ifdef USB
    return USBUART_GetChar(); 
    #else
    return UART_UartGetChar();
    #endif
    
}

void    pinMode(uint8_t x,uint8_t y)
{
    x=x+y;
    return;
}
void    digitalWrite(uint8_t x,uint8_t y)
{
    x=x+y;
    return;
}
uint8_t    digitalRead(uint8_t x)
{
    return x;
}



#define MY_DEBUG
#if defined(MY_DEBUG) || defined(MY_SPECIAL_DEBUG)

uint16_t hwCPUVoltage()
{
#if defined(MY_SPECIAL_DEBUG)
	// in mV, requires ADC_VCC set
	return ESP.getVcc();
#else
	// not possible
	return 0;
#endif
}

uint16_t hwCPUFrequency()
{
	// in 1/10Mhz
    return 480;
	//return ESP.getCpuFreqMHz()*10;
}

uint16_t hwFreeMem()
{
    return 0;
    //return ESP.getFreeHeap();
}
#endif

#ifdef MY_DEBUG
void hwDebugPrint(const char *fmt, ...)
{
	char fmtBuffer[MY_SERIAL_OUTPUT_SIZE];
#ifdef MY_GATEWAY_FEATURE
	// prepend debug message to be handled correctly by controller (C_INTERNAL, I_LOG_MESSAGE)
    
	snprintf_P(fmtBuffer, sizeof(fmtBuffer), PSTR("0;255;%d;0;%d;"), C_INTERNAL, I_LOG_MESSAGE);
	MyHwPsoc_SerialPrint(fmtBuffer);
#else
	// prepend timestamp
	MY_SERIALDEVICE.print(hwMillis());
	MY_SERIALDEVICE.print(" ");
#endif
	va_list args;
	va_start(args, fmt);
#ifdef MY_GATEWAY_FEATURE
	// Truncate message if this is gateway node
	vsnprintf_P(fmtBuffer, sizeof(fmtBuffer), fmt, args);
	fmtBuffer[sizeof(fmtBuffer) - 2] = '\n';
	fmtBuffer[sizeof(fmtBuffer) - 1] = '\0';
#else
	vsnprintf_P(fmtBuffer, sizeof(fmtBuffer), fmt, args);
#endif
	va_end(args);
	MyHwPsoc_SerialPrint(fmtBuffer);
	// Disable flush since current STM32duino implementation performs a reset
	// instead of an actual flush
	//MY_SERIALDEVICE.flush();
}
#endif


/* [] END OF FILE */
