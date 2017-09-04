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
#define WEATHERSENSORS_H_
    
//#define bmetest
    
#include "project.h"
#include "bme280.h"
#include <stdlib.h>
    
/* Define to store the Frequency of the Clock used for the Counter(TCPWM component).
 * The value is defined in Hz */
#define CLOCK_FREQ (float)3000000u

/* Define to indicate the number of microseconds in 1 second. Used for scaling purpose */
#define NO_OF_USEC (float)1000000u

//Weather station defines
#define RAINGAUGE_MULTIPLIER    2794 //defined in (1/10000)
#define WINDSPEED_MULTIPLIER    2400 //2.4km/h per tick


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
void    lookupWindVane(winddirection *p_angle, uint16 ain); 
#ifndef bmetest
void lookupWindVane(winddirection *p_angle, uint16 ain);
int8_t get_sensor_data_normal_mode();
int8_t get_sensor_data_forced_mode();
int8_t BME_SpiRead( uint8_t dev_id,volatile uint8_t reg_addr,volatile uint8_t *data,volatile uint16_t len);
int8_t BME_SpiWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void BME_DelayMs(uint32_t delay);

#endif

//Public functions
void            WEATHER_Setup();
winddirection   WEATHER_getWindDirection();
uint8_t         WEATHER_getWindSpeed();
uint16_t        WEATHER_getRainGauge();
uint8_t         WEATHER_getBME280(bme280_data *comp_data);


#endif /* WEATHERSENSORS_H__H_ */
/* [] END OF FILE */

