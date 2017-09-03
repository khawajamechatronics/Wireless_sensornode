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
#include "temp_bme280.h"
#include "weathersensors.h"
#include "stdio.h"
#include "EEPROM.h"





int main(void)
{
    

    CyGlobalIntEnable; /* Enable global interrupts. */

    BME280_setup();
    
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
   
    //ADC//////////////////////////////////////////////////////
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
   

    //LOOP/////////////////////////////////////////////////////////
    
    //get_sensor_data_forced_mode(&dev);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    uint64 unixtime=0;
    //EEPROM_WriteBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    EEPROM_ReadBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    if(unixtime != 0)
        //RTC_SetUnixTime(unixtime);
    
    WEATHER_Setup();
    //ConsoleInit();
    for(;;)
    {
        //ConsoleProcess();
        winddirection direction_main;
    for(;;)    
    {    
        const char* directions[] = {"north","northeast","east","southeast","south","southwest","west","northwest","invalid"};
        
        direction_main = WEATHER_getWindDirection();
        sprintf(outstring,"%s\t", directions[direction_main]);
        UART_UartPutString(outstring);
        WEATHER_getWindSpeed();
        CyDelay(1000);
    }    
        
        
        
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
                //EEPROM_WriteBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
            }
        }        
    }
}

/* [] END OF FILE */
