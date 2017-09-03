/*******************************************************************************
* File Name: BME_MOSI.h  
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

#if !defined(CY_PINS_BME_MOSI_ALIASES_H) /* Pins BME_MOSI_ALIASES_H */
#define CY_PINS_BME_MOSI_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define BME_MOSI_0			(BME_MOSI__0__PC)
#define BME_MOSI_0_PS		(BME_MOSI__0__PS)
#define BME_MOSI_0_PC		(BME_MOSI__0__PC)
#define BME_MOSI_0_DR		(BME_MOSI__0__DR)
#define BME_MOSI_0_SHIFT	(BME_MOSI__0__SHIFT)
#define BME_MOSI_0_INTR	((uint16)((uint16)0x0003u << (BME_MOSI__0__SHIFT*2u)))

#define BME_MOSI_INTR_ALL	 ((uint16)(BME_MOSI_0_INTR))


#endif /* End Pins BME_MOSI_ALIASES_H */


/* [] END OF FILE */
