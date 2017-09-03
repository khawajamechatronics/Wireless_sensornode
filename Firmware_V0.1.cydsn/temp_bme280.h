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
#ifndef TEMP_BME280_H_
#define TEMP_BME280_H_
    
#include "project.h"
#include "bme280.h"

void BME280_setup();
int8_t get_sensor_data_normal_mode(bme280_dev *dev);
int8_t get_sensor_data_forced_mode(bme280_dev *devp);
int8_t BME_SpiRead( uint8_t dev_id,volatile uint8_t reg_addr,volatile uint8_t *data,volatile uint16_t len);
int8_t BME_SpiWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void BME_DelayMs(uint32_t delay);


#endif /* TEMP_BME280_H_ */
/* [] END OF FILE */
