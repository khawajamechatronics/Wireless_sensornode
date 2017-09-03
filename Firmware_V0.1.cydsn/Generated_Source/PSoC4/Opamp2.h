/*******************************************************************************
* File Name: Opamp2.h
* Version 1.20
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Opamp (Analog Buffer) Component.
*
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_OPAMP_Opamp2_H)
#define CY_OPAMP_Opamp2_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*       Type Definitions
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} Opamp2_BACKUP_STRUCT;


/**************************************
*        Function Prototypes
**************************************/
void Opamp2_Init(void);
void Opamp2_Enable(void);
void Opamp2_Start(void);
void Opamp2_Stop(void);
void Opamp2_SetPower(uint32 power);
void Opamp2_PumpControl(uint32 onOff);
void Opamp2_Sleep(void);
void Opamp2_Wakeup(void);
void Opamp2_SaveConfig(void);
void Opamp2_RestoreConfig(void);


/**************************************
*           API Constants
**************************************/

/* Parameters for SetPower() function */
#define Opamp2_LOW_POWER      (1u)
#define Opamp2_MED_POWER      (2u)
#define Opamp2_HIGH_POWER     (3u)


/* Parameters for PumpControl() function */
#define Opamp2_PUMP_ON        (1u)
#define Opamp2_PUMP_OFF       (0u)


/***************************************
*   Initial Parameter Constants
****************************************/

#define Opamp2_OUTPUT_CURRENT         (0u)
#define Opamp2_POWER                  (2u)
#define Opamp2_MODE                   (1u)
#define Opamp2_OA_COMP_TRIM_VALUE     (3u)
#define Opamp2_DEEPSLEEP_SUPPORT      (0u)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8  Opamp2_initVar;


/**************************************
*             Registers
**************************************/

#ifdef CYIPBLOCK_m0s8pass4b_VERSION
    #define Opamp2_CTB_CTRL_REG       (*(reg32 *) Opamp2_cy_psoc4_abuf__CTB_CTB_CTRL)
    #define Opamp2_CTB_CTRL_PTR       ( (reg32 *) Opamp2_cy_psoc4_abuf__CTB_CTB_CTRL)
#else
    #define Opamp2_CTB_CTRL_REG       (*(reg32 *) Opamp2_cy_psoc4_abuf__CTBM_CTB_CTRL)
    #define Opamp2_CTB_CTRL_PTR       ( (reg32 *) Opamp2_cy_psoc4_abuf__CTBM_CTB_CTRL)
#endif /* CYIPBLOCK_m0s8pass4b_VERSION */

#define Opamp2_OA_RES_CTRL_REG    (*(reg32 *) Opamp2_cy_psoc4_abuf__OA_RES_CTRL)
#define Opamp2_OA_RES_CTRL_PTR    ( (reg32 *) Opamp2_cy_psoc4_abuf__OA_RES_CTRL)
#define Opamp2_OA_COMP_TRIM_REG   (*(reg32 *) Opamp2_cy_psoc4_abuf__OA_COMP_TRIM)
#define Opamp2_OA_COMP_TRIM_PTR   ( (reg32 *) Opamp2_cy_psoc4_abuf__OA_COMP_TRIM)


/***************************************
*        Registers Constants
***************************************/

/* Opamp2_CTB_CTRL_REG */
#define Opamp2_CTB_CTRL_DEEPSLEEP_ON_SHIFT    (30u)   /* [30] Selects behavior CTB IP in the DeepSleep power mode */
#define Opamp2_CTB_CTRL_ENABLED_SHIFT         (31u)   /* [31] Enable of the CTB IP */


#define Opamp2_CTB_CTRL_DEEPSLEEP_ON          ((uint32) 0x01u << Opamp2_CTB_CTRL_DEEPSLEEP_ON_SHIFT)
#define Opamp2_CTB_CTRL_ENABLED               ((uint32) 0x01u << Opamp2_CTB_CTRL_ENABLED_SHIFT)


/* Opamp2_OA_RES_CTRL_REG */
#define Opamp2_OA_PWR_MODE_SHIFT          (0u)    /* [1:0]    Power level */
#define Opamp2_OA_DRIVE_STR_SEL_SHIFT     (2u)    /* [2]      Opamp output strenght select: 0 - 1x, 1 - 10x */
#define Opamp2_OA_COMP_EN_SHIFT           (4u)    /* [4]      CTB IP mode: 0 - Opamp, 1 - Comparator  */
#define Opamp2_OA_PUMP_EN_SHIFT           (11u)   /* [11]     Pump enable */


#define Opamp2_OA_PWR_MODE                ((uint32) 0x02u << Opamp2_OA_PWR_MODE_SHIFT)
#define Opamp2_OA_PWR_MODE_MASK           ((uint32) 0x03u << Opamp2_OA_PWR_MODE_SHIFT)
#define Opamp2_OA_DRIVE_STR_SEL_1X        ((uint32) 0x00u << Opamp2_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp2_OA_DRIVE_STR_SEL_10X       ((uint32) 0x01u << Opamp2_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp2_OA_DRIVE_STR_SEL_MASK      ((uint32) 0x01u << Opamp2_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp2_OA_COMP_EN                 ((uint32) 0x00u << Opamp2_OA_COMP_EN_SHIFT)
#define Opamp2_OA_PUMP_EN                 ((uint32) 0x01u << Opamp2_OA_PUMP_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define Opamp2_GET_DEEPSLEEP_ON(deepSleep)    ((0u != (deepSleep)) ? (Opamp2_CTB_CTRL_DEEPSLEEP_ON) : (0u))
#define Opamp2_GET_OA_DRIVE_STR(current)      ((0u != (current)) ? (Opamp2_OA_DRIVE_STR_SEL_10X) : \
                                                                             (Opamp2_OA_DRIVE_STR_SEL_1X))
#define Opamp2_GET_OA_PWR_MODE(mode)          ((mode) & Opamp2_OA_PWR_MODE_MASK)
#define Opamp2_CHECK_PWR_MODE_OFF             (0u != (Opamp2_OA_RES_CTRL_REG & \
                                                                Opamp2_OA_PWR_MODE_MASK))

/* Returns true if component available in Deep Sleep power mode*/ 
#define Opamp2_CHECK_DEEPSLEEP_SUPPORT        (0u != Opamp2_DEEPSLEEP_SUPPORT) 

#define Opamp2_DEFAULT_CTB_CTRL (Opamp2_GET_DEEPSLEEP_ON(Opamp2_DEEPSLEEP_SUPPORT) | \
                                           Opamp2_CTB_CTRL_ENABLED)

#define Opamp2_DEFAULT_OA_RES_CTRL (Opamp2_OA_COMP_EN | \
                                              Opamp2_GET_OA_DRIVE_STR(Opamp2_OUTPUT_CURRENT))

#define Opamp2_DEFAULT_OA_COMP_TRIM_REG (Opamp2_OA_COMP_TRIM_VALUE)


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define Opamp2_LOWPOWER                   (Opamp2_LOW_POWER)
#define Opamp2_MEDPOWER                   (Opamp2_MED_POWER)
#define Opamp2_HIGHPOWER                  (Opamp2_HIGH_POWER)

/* PUMP ON/OFF defines */
#define Opamp2_PUMPON                     (Opamp2_PUMP_ON)
#define Opamp2_PUMPOFF                    (Opamp2_PUMP_OFF)

#define Opamp2_OA_CTRL                    (Opamp2_CTB_CTRL_REG)
#define Opamp2_OA_RES_CTRL                (Opamp2_OA_RES_CTRL_REG)

/* Bit Field  OA_CTRL */
#define Opamp2_OA_CTB_EN_SHIFT            (Opamp2_CTB_CTRL_ENABLED_SHIFT)
#define Opamp2_OA_PUMP_CTRL_SHIFT         (Opamp2_OA_PUMP_EN_SHIFT)
#define Opamp2_OA_PUMP_EN_MASK            (0x800u)
#define Opamp2_PUMP_PROTECT_MASK          (1u)


#endif    /* CY_OPAMP_Opamp2_H */


/* [] END OF FILE */
