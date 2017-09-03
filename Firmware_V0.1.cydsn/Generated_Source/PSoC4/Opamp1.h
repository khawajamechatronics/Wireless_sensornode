/*******************************************************************************
* File Name: Opamp1.h
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


#if !defined(CY_OPAMP_Opamp1_H)
#define CY_OPAMP_Opamp1_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*       Type Definitions
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} Opamp1_BACKUP_STRUCT;


/**************************************
*        Function Prototypes
**************************************/
void Opamp1_Init(void);
void Opamp1_Enable(void);
void Opamp1_Start(void);
void Opamp1_Stop(void);
void Opamp1_SetPower(uint32 power);
void Opamp1_PumpControl(uint32 onOff);
void Opamp1_Sleep(void);
void Opamp1_Wakeup(void);
void Opamp1_SaveConfig(void);
void Opamp1_RestoreConfig(void);


/**************************************
*           API Constants
**************************************/

/* Parameters for SetPower() function */
#define Opamp1_LOW_POWER      (1u)
#define Opamp1_MED_POWER      (2u)
#define Opamp1_HIGH_POWER     (3u)


/* Parameters for PumpControl() function */
#define Opamp1_PUMP_ON        (1u)
#define Opamp1_PUMP_OFF       (0u)


/***************************************
*   Initial Parameter Constants
****************************************/

#define Opamp1_OUTPUT_CURRENT         (0u)
#define Opamp1_POWER                  (3u)
#define Opamp1_MODE                   (1u)
#define Opamp1_OA_COMP_TRIM_VALUE     (3u)
#define Opamp1_DEEPSLEEP_SUPPORT      (0u)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8  Opamp1_initVar;


/**************************************
*             Registers
**************************************/

#ifdef CYIPBLOCK_m0s8pass4b_VERSION
    #define Opamp1_CTB_CTRL_REG       (*(reg32 *) Opamp1_cy_psoc4_abuf__CTB_CTB_CTRL)
    #define Opamp1_CTB_CTRL_PTR       ( (reg32 *) Opamp1_cy_psoc4_abuf__CTB_CTB_CTRL)
#else
    #define Opamp1_CTB_CTRL_REG       (*(reg32 *) Opamp1_cy_psoc4_abuf__CTBM_CTB_CTRL)
    #define Opamp1_CTB_CTRL_PTR       ( (reg32 *) Opamp1_cy_psoc4_abuf__CTBM_CTB_CTRL)
#endif /* CYIPBLOCK_m0s8pass4b_VERSION */

#define Opamp1_OA_RES_CTRL_REG    (*(reg32 *) Opamp1_cy_psoc4_abuf__OA_RES_CTRL)
#define Opamp1_OA_RES_CTRL_PTR    ( (reg32 *) Opamp1_cy_psoc4_abuf__OA_RES_CTRL)
#define Opamp1_OA_COMP_TRIM_REG   (*(reg32 *) Opamp1_cy_psoc4_abuf__OA_COMP_TRIM)
#define Opamp1_OA_COMP_TRIM_PTR   ( (reg32 *) Opamp1_cy_psoc4_abuf__OA_COMP_TRIM)


/***************************************
*        Registers Constants
***************************************/

/* Opamp1_CTB_CTRL_REG */
#define Opamp1_CTB_CTRL_DEEPSLEEP_ON_SHIFT    (30u)   /* [30] Selects behavior CTB IP in the DeepSleep power mode */
#define Opamp1_CTB_CTRL_ENABLED_SHIFT         (31u)   /* [31] Enable of the CTB IP */


#define Opamp1_CTB_CTRL_DEEPSLEEP_ON          ((uint32) 0x01u << Opamp1_CTB_CTRL_DEEPSLEEP_ON_SHIFT)
#define Opamp1_CTB_CTRL_ENABLED               ((uint32) 0x01u << Opamp1_CTB_CTRL_ENABLED_SHIFT)


/* Opamp1_OA_RES_CTRL_REG */
#define Opamp1_OA_PWR_MODE_SHIFT          (0u)    /* [1:0]    Power level */
#define Opamp1_OA_DRIVE_STR_SEL_SHIFT     (2u)    /* [2]      Opamp output strenght select: 0 - 1x, 1 - 10x */
#define Opamp1_OA_COMP_EN_SHIFT           (4u)    /* [4]      CTB IP mode: 0 - Opamp, 1 - Comparator  */
#define Opamp1_OA_PUMP_EN_SHIFT           (11u)   /* [11]     Pump enable */


#define Opamp1_OA_PWR_MODE                ((uint32) 0x02u << Opamp1_OA_PWR_MODE_SHIFT)
#define Opamp1_OA_PWR_MODE_MASK           ((uint32) 0x03u << Opamp1_OA_PWR_MODE_SHIFT)
#define Opamp1_OA_DRIVE_STR_SEL_1X        ((uint32) 0x00u << Opamp1_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp1_OA_DRIVE_STR_SEL_10X       ((uint32) 0x01u << Opamp1_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp1_OA_DRIVE_STR_SEL_MASK      ((uint32) 0x01u << Opamp1_OA_DRIVE_STR_SEL_SHIFT)
#define Opamp1_OA_COMP_EN                 ((uint32) 0x00u << Opamp1_OA_COMP_EN_SHIFT)
#define Opamp1_OA_PUMP_EN                 ((uint32) 0x01u << Opamp1_OA_PUMP_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define Opamp1_GET_DEEPSLEEP_ON(deepSleep)    ((0u != (deepSleep)) ? (Opamp1_CTB_CTRL_DEEPSLEEP_ON) : (0u))
#define Opamp1_GET_OA_DRIVE_STR(current)      ((0u != (current)) ? (Opamp1_OA_DRIVE_STR_SEL_10X) : \
                                                                             (Opamp1_OA_DRIVE_STR_SEL_1X))
#define Opamp1_GET_OA_PWR_MODE(mode)          ((mode) & Opamp1_OA_PWR_MODE_MASK)
#define Opamp1_CHECK_PWR_MODE_OFF             (0u != (Opamp1_OA_RES_CTRL_REG & \
                                                                Opamp1_OA_PWR_MODE_MASK))

/* Returns true if component available in Deep Sleep power mode*/ 
#define Opamp1_CHECK_DEEPSLEEP_SUPPORT        (0u != Opamp1_DEEPSLEEP_SUPPORT) 

#define Opamp1_DEFAULT_CTB_CTRL (Opamp1_GET_DEEPSLEEP_ON(Opamp1_DEEPSLEEP_SUPPORT) | \
                                           Opamp1_CTB_CTRL_ENABLED)

#define Opamp1_DEFAULT_OA_RES_CTRL (Opamp1_OA_COMP_EN | \
                                              Opamp1_GET_OA_DRIVE_STR(Opamp1_OUTPUT_CURRENT))

#define Opamp1_DEFAULT_OA_COMP_TRIM_REG (Opamp1_OA_COMP_TRIM_VALUE)


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define Opamp1_LOWPOWER                   (Opamp1_LOW_POWER)
#define Opamp1_MEDPOWER                   (Opamp1_MED_POWER)
#define Opamp1_HIGHPOWER                  (Opamp1_HIGH_POWER)

/* PUMP ON/OFF defines */
#define Opamp1_PUMPON                     (Opamp1_PUMP_ON)
#define Opamp1_PUMPOFF                    (Opamp1_PUMP_OFF)

#define Opamp1_OA_CTRL                    (Opamp1_CTB_CTRL_REG)
#define Opamp1_OA_RES_CTRL                (Opamp1_OA_RES_CTRL_REG)

/* Bit Field  OA_CTRL */
#define Opamp1_OA_CTB_EN_SHIFT            (Opamp1_CTB_CTRL_ENABLED_SHIFT)
#define Opamp1_OA_PUMP_CTRL_SHIFT         (Opamp1_OA_PUMP_EN_SHIFT)
#define Opamp1_OA_PUMP_EN_MASK            (0x800u)
#define Opamp1_PUMP_PROTECT_MASK          (1u)


#endif    /* CY_OPAMP_Opamp1_H */


/* [] END OF FILE */
