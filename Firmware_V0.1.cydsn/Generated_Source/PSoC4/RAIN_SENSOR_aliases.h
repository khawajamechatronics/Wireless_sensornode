/*******************************************************************************
* File Name: RAIN_SENSOR.h  
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

#if !defined(CY_PINS_RAIN_SENSOR_ALIASES_H) /* Pins RAIN_SENSOR_ALIASES_H */
#define CY_PINS_RAIN_SENSOR_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RAIN_SENSOR_0			(RAIN_SENSOR__0__PC)
#define RAIN_SENSOR_0_PS		(RAIN_SENSOR__0__PS)
#define RAIN_SENSOR_0_PC		(RAIN_SENSOR__0__PC)
#define RAIN_SENSOR_0_DR		(RAIN_SENSOR__0__DR)
#define RAIN_SENSOR_0_SHIFT	(RAIN_SENSOR__0__SHIFT)
#define RAIN_SENSOR_0_INTR	((uint16)((uint16)0x0003u << (RAIN_SENSOR__0__SHIFT*2u)))

#define RAIN_SENSOR_INTR_ALL	 ((uint16)(RAIN_SENSOR_0_INTR))


#endif /* End Pins RAIN_SENSOR_ALIASES_H */


/* [] END OF FILE */
