/*******************************************************************************
* File Name: RFM69HW_RESET.h  
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

#if !defined(CY_PINS_RFM69HW_RESET_ALIASES_H) /* Pins RFM69HW_RESET_ALIASES_H */
#define CY_PINS_RFM69HW_RESET_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RFM69HW_RESET_0			(RFM69HW_RESET__0__PC)
#define RFM69HW_RESET_0_PS		(RFM69HW_RESET__0__PS)
#define RFM69HW_RESET_0_PC		(RFM69HW_RESET__0__PC)
#define RFM69HW_RESET_0_DR		(RFM69HW_RESET__0__DR)
#define RFM69HW_RESET_0_SHIFT	(RFM69HW_RESET__0__SHIFT)
#define RFM69HW_RESET_0_INTR	((uint16)((uint16)0x0003u << (RFM69HW_RESET__0__SHIFT*2u)))

#define RFM69HW_RESET_INTR_ALL	 ((uint16)(RFM69HW_RESET_0_INTR))


#endif /* End Pins RFM69HW_RESET_ALIASES_H */


/* [] END OF FILE */
