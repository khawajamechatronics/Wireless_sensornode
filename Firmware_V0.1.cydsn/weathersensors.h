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

#ifndef WEATHERSENSORS_H_
#define WEATHERSENSORS_H_0_H_
    
#include "project.h"
#include "bme280.h"
#include <stdlib.h>
    
/* Define to store the Frequency of the Clock used for the Counter(TCPWM component).
 * The value is defined in Hz */
#define CLOCK_FREQ (float)3000000u

/* Define to indicate the number of microseconds in 1 second. Used for scaling purpose */
#define NO_OF_USEC (float)1000000u



typedef enum winddirection 
{
    north,
    northeast,
    east,
    southeast,
    south,
    southwest,
    west,
    northwest,
    invalid
} winddirection;

typedef struct sensors
{
    uint8_t winddirection;
    uint8_t windspeed;
    uint8_t rain;
    uint8_t temperature;
    uint8_t humidity;
    uint8_t pressure;
} sensors;

//Local functions
void lookupWindVane(winddirection *p_angle, uint16 ain);   

//Public functions
void            WEATHER_Setup();
winddirection   WEATHER_getWindDirection();
uint8_t         WEATHER_getWindSpeed();




#endif /* WEATHERSENSORS_H__H_ */
/* [] END OF FILE */

/* [] END OF FILE */
