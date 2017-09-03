/*******************************************************************************
* File Name: EEPROM_PULLUP.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_EEPROM_PULLUP_ALIASES_H) /* Pins EEPROM_PULLUP_ALIASES_H */
#define CY_PINS_EEPROM_PULLUP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define EEPROM_PULLUP_0			(EEPROM_PULLUP__0__PC)
#define EEPROM_PULLUP_0_PS		(EEPROM_PULLUP__0__PS)
#define EEPROM_PULLUP_0_PC		(EEPROM_PULLUP__0__PC)
#define EEPROM_PULLUP_0_DR		(EEPROM_PULLUP__0__DR)
#define EEPROM_PULLUP_0_SHIFT	(EEPROM_PULLUP__0__SHIFT)
#define EEPROM_PULLUP_0_INTR	((uint16)((uint16)0x0003u << (EEPROM_PULLUP__0__SHIFT*2u)))

#define EEPROM_PULLUP_INTR_ALL	 ((uint16)(EEPROM_PULLUP_0_INTR))


#endif /* End Pins EEPROM_PULLUP_ALIASES_H */


/* [] END OF FILE */