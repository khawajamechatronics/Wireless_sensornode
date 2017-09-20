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

#include "p1_meter.h"

void P1_getdata(P1_data *meterData)
{
    uint8_t data;
    uint8_t databuf[31];
    char outstring[100];
    uint8_t i;
    long tl = 0;
    long tld = 0;
    char buffer[BUFSIZE];
    int bufpos = 0;
    
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
                meterData->lowTariff = tl;
            }
            //electraverbruik hoog tarief
            if (sscanf(buffer,"1-0:1.8.2(%ld.%ld", &tl, &tld)==2)
            {
                tl *= 1000;
                tl += tld;
                meterData->highTariff = tl;
            }
            //electraverbruik actual usage
            if (sscanf(buffer,"1-0:1.7.0(%ld.%ld", &tl, &tld)==2)
            {
                meterData->currentConsumption = (tl*1000)+tld;
            }
            // 0-1:24.2.1 = Gas (DSMR v4.0) on Kaifa MA105 meter
            if (strncmp(buffer, "0-1:24.2.1", strlen("0-1:24.2.1")) == 0) 
            {
                if (sscanf(strrchr(buffer, '(') + 1, "%d.%d", &tl, &tld) == 2) 
                {
                    meterData->gas = (tl*1000)+tld;
                }
            }
            for (i=0; i<75; i++)
            {
                buffer[i] = 0;
            }
            bufpos = 0;
        }
    }
    UART_P1_Stop();
}


/* [] END OF FILE */
