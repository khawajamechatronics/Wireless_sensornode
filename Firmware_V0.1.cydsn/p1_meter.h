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

#ifndef P1_METER_H_
#define P1_METER_H_

#include "project.h"
#include "stdio.h"
    
#define BUFSIZE 75   

typedef struct {
    uint32_t    lowTariff;          //Meter reading Electrics - consumption low tariff
    uint32_t    highTariff;         //Meter reading Electrics - consumption high tariff
    uint32_t    currentConsumption; //Meter reading Electrics - Actual consumption
    uint32_t    gas;                 //Meter reading Gas
} P1_data;
   
void P1_getdata(P1_data *meterData);
    
#endif /* P1_METER_H_ */