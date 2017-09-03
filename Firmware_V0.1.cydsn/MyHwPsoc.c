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

uint32_t MyHwPsoc_Random(uint32_t min, uint32_t max)
{
    uint32_t rand = PRS_Read();
    return (rand * (max - min) / 65535 + min);
}

void MyHwPsoc_init(void)
{
    PRS_Start();
    TIMER1_Start();
    MyHwPsoc_LedSet(RX_LED, 1);
    MyHwPsoc_LedSet(TX_LED, 1);
    MyHwPsoc_LedSet(ERR_LED, 1);
}

void MyHwPsoc_RandomNumberInit(void)
{
    PRS_Init();
    CyDelay(1u);
    PRS_Step();
}

void MyHwPsoc_ReadConfigBlock(void* buf, uint16_t addr, size_t length)
{
    memcpy( buf, &eeprom[addr], length ); 
}

uint8_t MyHwPsoc_ReadConfig(uint16_t addr)
{
    uint8_t value;
    MyHwPsoc_ReadConfigBlock( &value, addr, 1);
    return value;
}
    
void MyHwPsoc_WriteConfigBlock(void* buf, uint16_t addr, size_t length)
{
    memcpy( &eeprom[addr], buf, length ); 
}

void MyHwPsoc_WriteConfig(uint16_t addr, uint8_t buf)
{
    MyHwPsoc_WriteConfigBlock(&buf, addr, 1);
}

void MyHwPsoc_Reboot(void)
{
}

int8_t MyHwPsoc_hwSleep(unsigned long ms)
{
}

int8_t MyHwPsoc_hwSleep1(uint8_t interrupt, uint8_t mode, unsigned long ms)
{
}

int8_t MyHwPsoc_hwSleep2(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms)
{
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
            // TODO: csn pin state definition
            break;
        case NRF24_CE:
            // TODO: csn pin state definition
            break;
    }
}

uint8_t MyHwPsoc_SpiTransfer(uint8_t data)
{
    return data;
}

void MyHwPsoc_SpiEndTransaction(void)
{
    
}




/* [] END OF FILE */
