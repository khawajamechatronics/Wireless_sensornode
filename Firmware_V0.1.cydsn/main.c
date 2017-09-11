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
#include <stdlib.h>
#include <string.h>
#include "EEPROM.h"

#define BUFSIZE 75
char buffer[BUFSIZE];
int bufpos = 0;

long mEVLT = 0; //Meter reading Electrics - consumption low tariff
long mEVHT = 0; //Meter reading Electrics - consumption high tariff
long mEAV = 0;  //Meter reading Electrics - Actual consumption
long mG = 0;   //Meter reading Gas

void P1_getdata(void)
{
    //Setup
    uint8_t data;
    uint8_t databuf[31];
    char outstring[100];
    sprintf(outstring, "EMPTY");
    uint8_t i;
    uint8_t messagestate=0;
    long tl = 0;
    long tld = 0;
    
    UART_P1_Start();
    
    _Bool end = 0;
    while(end == 0)
    {
        while(0 == UART_P1_GetRxBufferSize());
        data = UART_P1_GetByte() & 0xFF;
        if(data == 0x2F)
            end = 1;
    }
    end = 0;
    while(end == 0)
    {
        while(0 == UART_P1_GetRxBufferSize());
        data = UART_P1_GetByte() & 0xFF;
        if(data == 0x2F)
            end = 1;
        //UART_UartPutChar(data);
        buffer[bufpos] = data & 0x7F;
        bufpos++;
        
        if(data == '\n') //We received a newline
        {
            //electraverbruik laag tarief
            if (sscanf(buffer,"1-0:1.8.1(%ld.%ld", &tl, &tld)==2)
            {
                tl *= 1000;
                tl += tld;
                mEVLT = tl;
                sprintf(outstring,"Laag Tarief: %ld Wh\r\n",mEVLT);
                UART_UartPutString(outstring);
            }
            //electraverbruik hoog tarief
            if (sscanf(buffer,"1-0:1.8.2(%ld.%ld", &tl, &tld)==2)
            {
                tl *= 1000;
                tl += tld;
                mEVHT = tl;
                sprintf(outstring,"Hoog Tarief: %ld Wh\r\n",mEVHT);
                UART_UartPutString(outstring);
            }
            //electraverbruik actual usage
            if (sscanf(buffer,"1-0:1.7.0(%ld.%ld", &tl, &tld)==2)
            {
                mEAV = (tl*1000)+tld;
                sprintf(outstring,"Huidig gebruik: %ld W\r\n",mEAV);
                UART_UartPutString(outstring);
            }
            // 0-1:24.2.1 = Gas (DSMR v4.0) on Kaifa MA105 meter
            if (strncmp(buffer, "0-1:24.2.1", strlen("0-1:24.2.1")) == 0) 
            {
                if (sscanf(strrchr(buffer, '(') + 1, "%d.%d", &tl, &tld) == 2) 
                {
                    mG = (tl*1000)+tld;
                    float gas = mG;
                    gas = gas / 1000;
                    sprintf(outstring,"Gas: %ld.%ld\r\n",tl,tld);
                    UART_UartPutString(outstring);
                }
            }
            for (i=0; i<75; i++)
            {
                buffer[i] = 0;
            }
            bufpos = 0;
        }
    }
    //Wait until next message
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
    
    
    
    WEATHER_Setup();
    //UART//////////////////////////////////////////////////////
    UART_Start();    
    UART_UartPutString("\r\n***********************************************************************************\r\n");
    UART_UartPutString("Corne Bos Wireless Sensor Node new edition\r\n");
    UART_UartPutString("\r\n");
    P1_getdata();
    
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
