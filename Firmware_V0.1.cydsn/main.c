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
#include "weathersensors.h"
#include "stdio.h"
#include "EEPROM.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    WEATHER_Setup();
    //UART//////////////////////////////////////////////////////
    UART_Start();    
    UART_UartPutString("\r\n***********************************************************************************\r\n");
    UART_UartPutString("Corne Bos Wireless Sensor Node\r\n");
    UART_UartPutString("\r\n");
    
    //RTC////////////////////////////////////////////////////////
    
    RTC_Start();
    char outstring[100];
    
    
    //EEPROM/////////////////////////////////////////////////////
    I2C_Start();
    //EEPROM_PULLUP_Write(1);
   
    //ADC//////////////////////////////////////////////////////
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
   

    //LOOP/////////////////////////////////////////////////////////
    
    get_sensor_data_forced_mode();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    uint64 unixtime=0;
    //EEPROM_WriteBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    EEPROM_ReadBuffer( 0x50, 0, (uint8_t*)&unixtime, sizeof(unixtime));
    //if(unixtime != 0)
        //RTC_SetUnixTime(unixtime);
    
    
    winddirection   direction_main;
    uint16_t        windspeed=0;
    uint16_t        rain=0,rain_d=0,rain_i=0;
    bme280_data     bmedata;

        
        
    for(;;)    
    {    
        const char* directions[] = {"north","northeast","east","southeast","south","southwest","west","northwest","invalid"};
        
        direction_main = WEATHER_getWindDirection();
        sprintf(outstring,"Wind: %s\t", directions[direction_main]);
        UART_UartPutString(outstring);
        
        windspeed = WEATHER_getWindSpeed();
        sprintf(outstring, "Speed: %d\t", windspeed);
        UART_UartPutString(outstring);
        
        rain = WEATHER_getRainGauge();
        rain_i = rain / 10;
        rain_d = rain - (rain_i *10);
        sprintf(outstring, "Rain: %d.%dmm\t",rain_i,rain_d);
        UART_UartPutString(outstring);
        
        WEATHER_getBME280(&bmedata);
        uint16 templ = bmedata.temperature;
        uint16 temph = bmedata.temperature /100;
        templ = templ - (temph *100);
        sprintf(outstring, "Temp:%d.%02dC\t",temph,templ);
        UART_UartPutString(outstring);
        
        uint16 humil = bmedata.humidity;
        uint16 humih = bmedata.humidity / 1000;
        humil = humil - (humih *1000);
        sprintf(outstring, "Hum:%d.%03d%%\t",humih,humil);
        UART_UartPutString(outstring);
        
        uint16 presh = bmedata.pressure / 100;
        uint16 presl = bmedata.pressure;
        presl = presl - (presh *100);
        sprintf(outstring, "Pres:%d.%02dPa\r\n",presh,presl);
        UART_UartPutString(outstring);
        
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

/* [] END OF FILE */
