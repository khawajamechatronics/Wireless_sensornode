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
#include "project.h"
#include "bme280.h"
#include "stdio.h"
#include "EEPROM.h"

#define SPI2


bme280_dev dev;
int8_t rslt = BME280_OK;

int8_t get_sensor_data_normal_mode(bme280_dev *dev)
{
	int8_t rslt;
	uint8_t settings_sel;
	bme280_data comp_data;

	dev->settings.osr_h = BME280_OVERSAMPLING_4X;
	dev->settings.osr_p = BME280_OVERSAMPLING_4X;
	dev->settings.osr_t = BME280_OVERSAMPLING_4X;

	settings_sel = BME280_OSR_PRESS_SEL|BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);
	/* Give some delay for the sensor to go into normal mode */
	CyDelay(5);
	
	while (1) {
		rslt = bme280_get_sensor_data(BME280_PRESS | BME280_HUM | BME280_TEMP, &comp_data, dev);
		//print_sensor_data(&comp_data);
	}

	return rslt;
}

int8_t get_sensor_data_forced_mode(bme280_dev *devp)
{
	int8_t rslt;
	uint8_t settings_sel;
	bme280_data comp_data;

	/* Continuously get the sensor data */
	while (1) {
		devp->settings.osr_h = BME280_OVERSAMPLING_1X;
		devp->settings.osr_p = BME280_OVERSAMPLING_1X;
		devp->settings.osr_t = BME280_OVERSAMPLING_1X;
 
		settings_sel = BME280_OSR_PRESS_SEL|BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL;

		rslt = bme280_set_sensor_settings(settings_sel, devp);
		rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, devp);
		/* Give some delay for the sensor to go into force mode */
		CyDelay(5);

		rslt = bme280_get_sensor_data(BME280_PRESS | BME280_HUM | BME280_TEMP, &comp_data, devp);
        
        char outstring[100];
        uint16 templ = comp_data.temperature;
        uint16 temph = comp_data.temperature /100;
        templ = templ - (temph *100);
        sprintf(outstring, "Temperature is:\t%d.%02d ºC\r\n",temph,templ);
        UART_UartPutString(outstring);
        
        uint16 humil = comp_data.humidity;
        uint16 humih = comp_data.humidity / 1000;
        humil = humil - (humih *1000);
        sprintf(outstring, "Humidity is:\t%d.%03d %%\r\n",humih,humil);
        UART_UartPutString(outstring);
        
        uint16 presh = comp_data.pressure / 100;
        uint16 presl = comp_data.pressure;
        presl = presl - (presh *100);
        sprintf(outstring, "Pressure is:\t%d.%02d Pa\r\n\r\n",presh,presl);
        UART_UartPutString(outstring);
        
        CyDelay(6000);
	}
	return rslt;
}



int8_t BME_SpiRead( uint8_t dev_id,volatile uint8_t reg_addr,volatile uint8_t *data,volatile uint16_t len)
{
    volatile int test = 0;
    if(reg_addr == 0x74)
    {
        CyDelay(1);
    }
    len = len +1;
    uint8 packetsize = len;
    uint8 dummyBuffer[30];
    volatile uint8 tmpBuffer[30];
    uint8 status;
    uint32 i=0;
    for(i=0;i<30;i++)
    {
        dummyBuffer[i] = 0xFFu;
        tmpBuffer[i] = 0x00u;
    }
    i=0;
    dummyBuffer[0] = (reg_addr | 0x80);
    SPIM_ClearRxBuffer();
    while(len--)
    {
        SPIM_WriteTxData(dummyBuffer[i]);
        i++;
    }
    while (packetsize != SPIM_GetRxBufferSize())
    {
    }
    SPIM_ClearTxBuffer();
    
    i=0u;
    while(0u != SPIM_GetRxBufferSize())
    {
        tmpBuffer[i] = SPIM_ReadRxData();
        if(i > 0)
        {
            *data = tmpBuffer[i];
            data++;
        }
        i++;
        
    }
}

int8_t BME_SpiWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{

    static uint8_t mTxBuffer[2];
    mTxBuffer[0] = (reg_addr & (~0x80));
    mTxBuffer[1] = *data;
    SPIM_WriteTxData(mTxBuffer[0]);
    SPIM_WriteTxData(mTxBuffer[1]);
    while(0 != SPIM_GetTxBufferSize());
    return 0;

}

void BME_DelayMs(uint32_t delay)
{
    CyDelay(delay);
}

int main(void)
{
        //BME280//////////////////////////////////////////////////////
    /* Sensor_0 interface over SPI with native chip select line */
    dev.id = 0;
    dev.interface = BME280_SPI_INTF;
    dev.read = &BME_SpiRead;
    dev.write = &BME_SpiWrite;
    dev.delay_ms = (void *)CyDelay;
    CyGlobalIntEnable; /* Enable global interrupts. */
    #ifdef SPI1
        SPI_BME280_Start();
    #endif
    #ifdef SPI2
        SPIM_Start();
    #endif
    rslt = bme280_init(&dev);
    
    //UART//////////////////////////////////////////////////////
    UART_Start();    
    UART_UartPutString("\r\n***********************************************************************************\r\n");
    UART_UartPutString("Corne Bos Wireless Sensor Node\r\n");
    UART_UartPutString("\r\n");
    
    //RTC////////////////////////////////////////////////////////
    
    RTC_Start();
    char8 outstring[100];
    sprintf(outstring, "123456789a123456789b123456789c123456789d123456789e123456789");
    //uint8_t txbuf[40] = "Hallo ik ben Corne";
    //txbuf[19] = 0;
    
    char8 rxbuf[100];
    int i;
    for(i=0;i<100;i++)
        rxbuf[i]=0;
   //EEPROM/////////////////////////////////////////////////////
    I2C_Start();
    //EEPROM_PULLUP_Write(1);
   


    

    //LOOP/////////////////////////////////////////////////////////
    
    //get_sensor_data_forced_mode(&dev);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    uint64 unixtime=0;
    //EEPROM_WriteBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    EEPROM_ReadBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    if(unixtime != 0)
        RTC_SetUnixTime(unixtime);
    for(;;)
    {
        //RTC
        
        RTC_DATE_TIME datetime;
        uint32 oldtime = datetime.time;
        uint32 olddate = datetime.date;
        uint8 day,month,hours,minutes,oldminutes,seconds;
        uint16 year;
        
        
        RTC_GetDateAndTime(&datetime);
        if(oldtime != datetime.time)
        {
            day     = RTC_GetDay(datetime.date);
            month   = RTC_GetMonth(datetime.date);
            year    = RTC_GetYear(datetime.date);
            hours   = RTC_GetHours(datetime.time);
            oldminutes = minutes;
            minutes = RTC_GetMinutes(datetime.time);
            seconds = RTC_GetSecond(datetime.time);
            sprintf(outstring, "%02d-%02d-%04d  %02d:%02d:%02d\r\n",day,month,year,hours,minutes,seconds);
            UART_UartPutString(outstring);
            int i;
            for(i=0;i<22;i++)
            {
                UART_UartPutCRLF(0);
            }
            if(minutes != oldminutes)
            {
                unixtime = RTC_GetUnixTime();
                EEPROM_WriteBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
            }
        }
        //RTC
        
        //UART
        
        //UART
        
        
    }
}

/* [] END OF FILE */
