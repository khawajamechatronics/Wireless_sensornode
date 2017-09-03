/*******************************************************************************
* File Name: ANEMOMETER.h  
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

#if !defined(CY_PINS_ANEMOMETER_ALIASES_H) /* Pins ANEMOMETER_ALIASES_H */
#define CY_PINS_ANEMOMETER_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define ANEMOMETER_0			(ANEMOMETER__0__PC)
#define ANEMOMETER_0_PS		(ANEMOMETER__0__PS)
#define ANEMOMETER_0_PC		(ANEMOMETER__0__PC)
#define ANEMOMETER_0_DR		(ANEMOMETER__0__DR)
#define ANEMOMETER_0_SHIFT	(ANEMOMETER__0__SHIFT)
#define ANEMOMETER_0_INTR	((uint16)((uint16)0x0003u << (ANEMOMETER__0__SHIFT*2u)))

#define ANEMOMETER_INTR_ALL	 ((uint16)(ANEMOMETER_0_INTR))


#endif /* End Pins ANEMOMETER_ALIASES_H */


/* [] END OF FILE */
