/*******************************************************************************
* File Name: CydwClock_1.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_CydwClock_1_H)
#define CY_CLOCK_CydwClock_1_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void CydwClock_1_StartEx(uint32 alignClkDiv);
#define CydwClock_1_Start() \
    CydwClock_1_StartEx(CydwClock_1__PA_DIV_ID)

#else

void CydwClock_1_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void CydwClock_1_Stop(void);

void CydwClock_1_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 CydwClock_1_GetDividerRegister(void);
uint8  CydwClock_1_GetFractionalDividerRegister(void);

#define CydwClock_1_Enable()                         CydwClock_1_Start()
#define CydwClock_1_Disable()                        CydwClock_1_Stop()
#define CydwClock_1_SetDividerRegister(clkDivider, reset)  \
    CydwClock_1_SetFractionalDividerRegister((clkDivider), 0u)
#define CydwClock_1_SetDivider(clkDivider)           CydwClock_1_SetDividerRegister((clkDivider), 1u)
#define CydwClock_1_SetDividerValue(clkDivider)      CydwClock_1_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define CydwClock_1_DIV_ID     CydwClock_1__DIV_ID

#define CydwClock_1_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define CydwClock_1_CTRL_REG   (*(reg32 *)CydwClock_1__CTRL_REGISTER)
#define CydwClock_1_DIV_REG    (*(reg32 *)CydwClock_1__DIV_REGISTER)

#define CydwClock_1_CMD_DIV_SHIFT          (0u)
#define CydwClock_1_CMD_PA_DIV_SHIFT       (8u)
#define CydwClock_1_CMD_DISABLE_SHIFT      (30u)
#define CydwClock_1_CMD_ENABLE_SHIFT       (31u)

#define CydwClock_1_CMD_DISABLE_MASK       ((uint32)((uint32)1u << CydwClock_1_CMD_DISABLE_SHIFT))
#define CydwClock_1_CMD_ENABLE_MASK        ((uint32)((uint32)1u << CydwClock_1_CMD_ENABLE_SHIFT))

#define CydwClock_1_DIV_FRAC_MASK  (0x000000F8u)
#define CydwClock_1_DIV_FRAC_SHIFT (3u)
#define CydwClock_1_DIV_INT_MASK   (0xFFFFFF00u)
#define CydwClock_1_DIV_INT_SHIFT  (8u)

#else 

#define CydwClock_1_DIV_REG        (*(reg32 *)CydwClock_1__REGISTER)
#define CydwClock_1_ENABLE_REG     CydwClock_1_DIV_REG
#define CydwClock_1_DIV_FRAC_MASK  CydwClock_1__FRAC_MASK
#define CydwClock_1_DIV_FRAC_SHIFT (16u)
#define CydwClock_1_DIV_INT_MASK   CydwClock_1__DIVIDER_MASK
#define CydwClock_1_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_CydwClock_1_H) */

/* [] END OF FILE */
