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

/* [] END OF FILE */
#ifndef EEPROM_H_
#define EEPROM_H_


#include "project.h"
#include "stdio.h"
    
void EEPROM_WriteBuf(uint8_t addr, uint16_t subaddr, uint8_t *data, uint8_t len);
uint8_t EEPROM_ReadBuf(uint8_t addr, uint16_t subaddr, uint8_t *data, uint8_t len);    
    
#endif /* EEPROM_H_ */