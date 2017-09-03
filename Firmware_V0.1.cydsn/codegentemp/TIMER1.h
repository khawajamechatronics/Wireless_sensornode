/*******************************************************************************
* File Name: TIMER1.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the TIMER1
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_TIMER1_H)
#define CY_TCPWM_TIMER1_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} TIMER1_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  TIMER1_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define TIMER1_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define TIMER1_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define TIMER1_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define TIMER1_QUAD_ENCODING_MODES            (0lu)
#define TIMER1_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define TIMER1_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define TIMER1_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define TIMER1_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define TIMER1_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define TIMER1_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define TIMER1_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define TIMER1_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define TIMER1_TC_RUN_MODE                    (0lu)
#define TIMER1_TC_COUNTER_MODE                (0lu)
#define TIMER1_TC_COMP_CAP_MODE               (2lu)
#define TIMER1_TC_PRESCALER                   (0lu)

/* Signal modes */
#define TIMER1_TC_RELOAD_SIGNAL_MODE          (0lu)
#define TIMER1_TC_COUNT_SIGNAL_MODE           (3lu)
#define TIMER1_TC_START_SIGNAL_MODE           (0lu)
#define TIMER1_TC_STOP_SIGNAL_MODE            (0lu)
#define TIMER1_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define TIMER1_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define TIMER1_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define TIMER1_TC_START_SIGNAL_PRESENT        (0lu)
#define TIMER1_TC_STOP_SIGNAL_PRESENT         (0lu)
#define TIMER1_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define TIMER1_TC_INTERRUPT_MASK              (0lu)

/* PWM Mode */
/* Parameters */
#define TIMER1_PWM_KILL_EVENT                 (0lu)
#define TIMER1_PWM_STOP_EVENT                 (0lu)
#define TIMER1_PWM_MODE                       (4lu)
#define TIMER1_PWM_OUT_N_INVERT               (0lu)
#define TIMER1_PWM_OUT_INVERT                 (0lu)
#define TIMER1_PWM_ALIGN                      (0lu)
#define TIMER1_PWM_RUN_MODE                   (0lu)
#define TIMER1_PWM_DEAD_TIME_CYCLE            (0lu)
#define TIMER1_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define TIMER1_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define TIMER1_PWM_COUNT_SIGNAL_MODE          (3lu)
#define TIMER1_PWM_START_SIGNAL_MODE          (0lu)
#define TIMER1_PWM_STOP_SIGNAL_MODE           (0lu)
#define TIMER1_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define TIMER1_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define TIMER1_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define TIMER1_PWM_START_SIGNAL_PRESENT       (0lu)
#define TIMER1_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define TIMER1_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define TIMER1_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define TIMER1_TC_PERIOD_VALUE                (65535lu)
#define TIMER1_TC_COMPARE_VALUE               (65535lu)
#define TIMER1_TC_COMPARE_BUF_VALUE           (65535lu)
#define TIMER1_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define TIMER1_PWM_PERIOD_VALUE               (65535lu)
#define TIMER1_PWM_PERIOD_BUF_VALUE           (65535lu)
#define TIMER1_PWM_PERIOD_SWAP                (0lu)
#define TIMER1_PWM_COMPARE_VALUE              (65535lu)
#define TIMER1_PWM_COMPARE_BUF_VALUE          (65535lu)
#define TIMER1_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define TIMER1__LEFT 0
#define TIMER1__RIGHT 1
#define TIMER1__CENTER 2
#define TIMER1__ASYMMETRIC 3

#define TIMER1__X1 0
#define TIMER1__X2 1
#define TIMER1__X4 2

#define TIMER1__PWM 4
#define TIMER1__PWM_DT 5
#define TIMER1__PWM_PR 6

#define TIMER1__INVERSE 1
#define TIMER1__DIRECT 0

#define TIMER1__CAPTURE 2
#define TIMER1__COMPARE 0

#define TIMER1__TRIG_LEVEL 3
#define TIMER1__TRIG_RISING 0
#define TIMER1__TRIG_FALLING 1
#define TIMER1__TRIG_BOTH 2

#define TIMER1__INTR_MASK_TC 1
#define TIMER1__INTR_MASK_CC_MATCH 2
#define TIMER1__INTR_MASK_NONE 0
#define TIMER1__INTR_MASK_TC_CC 3

#define TIMER1__UNCONFIG 8
#define TIMER1__TIMER 1
#define TIMER1__QUAD 3
#define TIMER1__PWM_SEL 7

#define TIMER1__COUNT_UP 0
#define TIMER1__COUNT_DOWN 1
#define TIMER1__COUNT_UPDOWN0 2
#define TIMER1__COUNT_UPDOWN1 3


/* Prescaler */
#define TIMER1_PRESCALE_DIVBY1                ((uint32)(0u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY2                ((uint32)(1u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY4                ((uint32)(2u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY8                ((uint32)(3u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY16               ((uint32)(4u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY32               ((uint32)(5u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY64               ((uint32)(6u << TIMER1_PRESCALER_SHIFT))
#define TIMER1_PRESCALE_DIVBY128              ((uint32)(7u << TIMER1_PRESCALER_SHIFT))

/* TCPWM set modes */
#define TIMER1_MODE_TIMER_COMPARE             ((uint32)(TIMER1__COMPARE         <<  \
                                                                  TIMER1_MODE_SHIFT))
#define TIMER1_MODE_TIMER_CAPTURE             ((uint32)(TIMER1__CAPTURE         <<  \
                                                                  TIMER1_MODE_SHIFT))
#define TIMER1_MODE_QUAD                      ((uint32)(TIMER1__QUAD            <<  \
                                                                  TIMER1_MODE_SHIFT))
#define TIMER1_MODE_PWM                       ((uint32)(TIMER1__PWM             <<  \
                                                                  TIMER1_MODE_SHIFT))
#define TIMER1_MODE_PWM_DT                    ((uint32)(TIMER1__PWM_DT          <<  \
                                                                  TIMER1_MODE_SHIFT))
#define TIMER1_MODE_PWM_PR                    ((uint32)(TIMER1__PWM_PR          <<  \
                                                                  TIMER1_MODE_SHIFT))

/* Quad Modes */
#define TIMER1_MODE_X1                        ((uint32)(TIMER1__X1              <<  \
                                                                  TIMER1_QUAD_MODE_SHIFT))
#define TIMER1_MODE_X2                        ((uint32)(TIMER1__X2              <<  \
                                                                  TIMER1_QUAD_MODE_SHIFT))
#define TIMER1_MODE_X4                        ((uint32)(TIMER1__X4              <<  \
                                                                  TIMER1_QUAD_MODE_SHIFT))

/* Counter modes */
#define TIMER1_COUNT_UP                       ((uint32)(TIMER1__COUNT_UP        <<  \
                                                                  TIMER1_UPDOWN_SHIFT))
#define TIMER1_COUNT_DOWN                     ((uint32)(TIMER1__COUNT_DOWN      <<  \
                                                                  TIMER1_UPDOWN_SHIFT))
#define TIMER1_COUNT_UPDOWN0                  ((uint32)(TIMER1__COUNT_UPDOWN0   <<  \
                                                                  TIMER1_UPDOWN_SHIFT))
#define TIMER1_COUNT_UPDOWN1                  ((uint32)(TIMER1__COUNT_UPDOWN1   <<  \
                                                                  TIMER1_UPDOWN_SHIFT))

/* PWM output invert */
#define TIMER1_INVERT_LINE                    ((uint32)(TIMER1__INVERSE         <<  \
                                                                  TIMER1_INV_OUT_SHIFT))
#define TIMER1_INVERT_LINE_N                  ((uint32)(TIMER1__INVERSE         <<  \
                                                                  TIMER1_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define TIMER1_TRIG_RISING                    ((uint32)TIMER1__TRIG_RISING)
#define TIMER1_TRIG_FALLING                   ((uint32)TIMER1__TRIG_FALLING)
#define TIMER1_TRIG_BOTH                      ((uint32)TIMER1__TRIG_BOTH)
#define TIMER1_TRIG_LEVEL                     ((uint32)TIMER1__TRIG_LEVEL)

/* Interrupt mask */
#define TIMER1_INTR_MASK_TC                   ((uint32)TIMER1__INTR_MASK_TC)
#define TIMER1_INTR_MASK_CC_MATCH             ((uint32)TIMER1__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define TIMER1_CC_MATCH_SET                   (0x00u)
#define TIMER1_CC_MATCH_CLEAR                 (0x01u)
#define TIMER1_CC_MATCH_INVERT                (0x02u)
#define TIMER1_CC_MATCH_NO_CHANGE             (0x03u)
#define TIMER1_OVERLOW_SET                    (0x00u)
#define TIMER1_OVERLOW_CLEAR                  (0x04u)
#define TIMER1_OVERLOW_INVERT                 (0x08u)
#define TIMER1_OVERLOW_NO_CHANGE              (0x0Cu)
#define TIMER1_UNDERFLOW_SET                  (0x00u)
#define TIMER1_UNDERFLOW_CLEAR                (0x10u)
#define TIMER1_UNDERFLOW_INVERT               (0x20u)
#define TIMER1_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define TIMER1_PWM_MODE_LEFT                  (TIMER1_CC_MATCH_CLEAR        |   \
                                                         TIMER1_OVERLOW_SET           |   \
                                                         TIMER1_UNDERFLOW_NO_CHANGE)
#define TIMER1_PWM_MODE_RIGHT                 (TIMER1_CC_MATCH_SET          |   \
                                                         TIMER1_OVERLOW_NO_CHANGE     |   \
                                                         TIMER1_UNDERFLOW_CLEAR)
#define TIMER1_PWM_MODE_ASYM                  (TIMER1_CC_MATCH_INVERT       |   \
                                                         TIMER1_OVERLOW_SET           |   \
                                                         TIMER1_UNDERFLOW_CLEAR)

#if (TIMER1_CY_TCPWM_V2)
    #if(TIMER1_CY_TCPWM_4000)
        #define TIMER1_PWM_MODE_CENTER                (TIMER1_CC_MATCH_INVERT       |   \
                                                                 TIMER1_OVERLOW_NO_CHANGE     |   \
                                                                 TIMER1_UNDERFLOW_CLEAR)
    #else
        #define TIMER1_PWM_MODE_CENTER                (TIMER1_CC_MATCH_INVERT       |   \
                                                                 TIMER1_OVERLOW_SET           |   \
                                                                 TIMER1_UNDERFLOW_CLEAR)
    #endif /* (TIMER1_CY_TCPWM_4000) */
#else
    #define TIMER1_PWM_MODE_CENTER                (TIMER1_CC_MATCH_INVERT       |   \
                                                             TIMER1_OVERLOW_NO_CHANGE     |   \
                                                             TIMER1_UNDERFLOW_CLEAR)
#endif /* (TIMER1_CY_TCPWM_NEW) */

/* Command operations without condition */
#define TIMER1_CMD_CAPTURE                    (0u)
#define TIMER1_CMD_RELOAD                     (8u)
#define TIMER1_CMD_STOP                       (16u)
#define TIMER1_CMD_START                      (24u)

/* Status */
#define TIMER1_STATUS_DOWN                    (1u)
#define TIMER1_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   TIMER1_Init(void);
void   TIMER1_Enable(void);
void   TIMER1_Start(void);
void   TIMER1_Stop(void);

void   TIMER1_SetMode(uint32 mode);
void   TIMER1_SetCounterMode(uint32 counterMode);
void   TIMER1_SetPWMMode(uint32 modeMask);
void   TIMER1_SetQDMode(uint32 qdMode);

void   TIMER1_SetPrescaler(uint32 prescaler);
void   TIMER1_TriggerCommand(uint32 mask, uint32 command);
void   TIMER1_SetOneShot(uint32 oneShotEnable);
uint32 TIMER1_ReadStatus(void);

void   TIMER1_SetPWMSyncKill(uint32 syncKillEnable);
void   TIMER1_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   TIMER1_SetPWMDeadTime(uint32 deadTime);
void   TIMER1_SetPWMInvert(uint32 mask);

void   TIMER1_SetInterruptMode(uint32 interruptMask);
uint32 TIMER1_GetInterruptSourceMasked(void);
uint32 TIMER1_GetInterruptSource(void);
void   TIMER1_ClearInterrupt(uint32 interruptMask);
void   TIMER1_SetInterrupt(uint32 interruptMask);

void   TIMER1_WriteCounter(uint32 count);
uint32 TIMER1_ReadCounter(void);

uint32 TIMER1_ReadCapture(void);
uint32 TIMER1_ReadCaptureBuf(void);

void   TIMER1_WritePeriod(uint32 period);
uint32 TIMER1_ReadPeriod(void);
void   TIMER1_WritePeriodBuf(uint32 periodBuf);
uint32 TIMER1_ReadPeriodBuf(void);

void   TIMER1_WriteCompare(uint32 compare);
uint32 TIMER1_ReadCompare(void);
void   TIMER1_WriteCompareBuf(uint32 compareBuf);
uint32 TIMER1_ReadCompareBuf(void);

void   TIMER1_SetPeriodSwap(uint32 swapEnable);
void   TIMER1_SetCompareSwap(uint32 swapEnable);

void   TIMER1_SetCaptureMode(uint32 triggerMode);
void   TIMER1_SetReloadMode(uint32 triggerMode);
void   TIMER1_SetStartMode(uint32 triggerMode);
void   TIMER1_SetStopMode(uint32 triggerMode);
void   TIMER1_SetCountMode(uint32 triggerMode);

void   TIMER1_SaveConfig(void);
void   TIMER1_RestoreConfig(void);
void   TIMER1_Sleep(void);
void   TIMER1_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define TIMER1_BLOCK_CONTROL_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define TIMER1_BLOCK_CONTROL_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define TIMER1_COMMAND_REG                    (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define TIMER1_COMMAND_PTR                    ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define TIMER1_INTRRUPT_CAUSE_REG             (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define TIMER1_INTRRUPT_CAUSE_PTR             ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define TIMER1_CONTROL_REG                    (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__CTRL )
#define TIMER1_CONTROL_PTR                    ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__CTRL )
#define TIMER1_STATUS_REG                     (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__STATUS )
#define TIMER1_STATUS_PTR                     ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__STATUS )
#define TIMER1_COUNTER_REG                    (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__COUNTER )
#define TIMER1_COUNTER_PTR                    ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__COUNTER )
#define TIMER1_COMP_CAP_REG                   (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__CC )
#define TIMER1_COMP_CAP_PTR                   ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__CC )
#define TIMER1_COMP_CAP_BUF_REG               (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__CC_BUFF )
#define TIMER1_COMP_CAP_BUF_PTR               ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__CC_BUFF )
#define TIMER1_PERIOD_REG                     (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__PERIOD )
#define TIMER1_PERIOD_PTR                     ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__PERIOD )
#define TIMER1_PERIOD_BUF_REG                 (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define TIMER1_PERIOD_BUF_PTR                 ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define TIMER1_TRIG_CONTROL0_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define TIMER1_TRIG_CONTROL0_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define TIMER1_TRIG_CONTROL1_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define TIMER1_TRIG_CONTROL1_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define TIMER1_TRIG_CONTROL2_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define TIMER1_TRIG_CONTROL2_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define TIMER1_INTERRUPT_REQ_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR )
#define TIMER1_INTERRUPT_REQ_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR )
#define TIMER1_INTERRUPT_SET_REG              (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_SET )
#define TIMER1_INTERRUPT_SET_PTR              ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_SET )
#define TIMER1_INTERRUPT_MASK_REG             (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_MASK )
#define TIMER1_INTERRUPT_MASK_PTR             ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_MASK )
#define TIMER1_INTERRUPT_MASKED_REG           (*(reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_MASKED )
#define TIMER1_INTERRUPT_MASKED_PTR           ( (reg32 *) TIMER1_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define TIMER1_MASK                           ((uint32)TIMER1_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define TIMER1_RELOAD_CC_SHIFT                (0u)
#define TIMER1_RELOAD_PERIOD_SHIFT            (1u)
#define TIMER1_PWM_SYNC_KILL_SHIFT            (2u)
#define TIMER1_PWM_STOP_KILL_SHIFT            (3u)
#define TIMER1_PRESCALER_SHIFT                (8u)
#define TIMER1_UPDOWN_SHIFT                   (16u)
#define TIMER1_ONESHOT_SHIFT                  (18u)
#define TIMER1_QUAD_MODE_SHIFT                (20u)
#define TIMER1_INV_OUT_SHIFT                  (20u)
#define TIMER1_INV_COMPL_OUT_SHIFT            (21u)
#define TIMER1_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define TIMER1_RELOAD_CC_MASK                 ((uint32)(TIMER1_1BIT_MASK        <<  \
                                                                            TIMER1_RELOAD_CC_SHIFT))
#define TIMER1_RELOAD_PERIOD_MASK             ((uint32)(TIMER1_1BIT_MASK        <<  \
                                                                            TIMER1_RELOAD_PERIOD_SHIFT))
#define TIMER1_PWM_SYNC_KILL_MASK             ((uint32)(TIMER1_1BIT_MASK        <<  \
                                                                            TIMER1_PWM_SYNC_KILL_SHIFT))
#define TIMER1_PWM_STOP_KILL_MASK             ((uint32)(TIMER1_1BIT_MASK        <<  \
                                                                            TIMER1_PWM_STOP_KILL_SHIFT))
#define TIMER1_PRESCALER_MASK                 ((uint32)(TIMER1_8BIT_MASK        <<  \
                                                                            TIMER1_PRESCALER_SHIFT))
#define TIMER1_UPDOWN_MASK                    ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                            TIMER1_UPDOWN_SHIFT))
#define TIMER1_ONESHOT_MASK                   ((uint32)(TIMER1_1BIT_MASK        <<  \
                                                                            TIMER1_ONESHOT_SHIFT))
#define TIMER1_QUAD_MODE_MASK                 ((uint32)(TIMER1_3BIT_MASK        <<  \
                                                                            TIMER1_QUAD_MODE_SHIFT))
#define TIMER1_INV_OUT_MASK                   ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                            TIMER1_INV_OUT_SHIFT))
#define TIMER1_MODE_MASK                      ((uint32)(TIMER1_3BIT_MASK        <<  \
                                                                            TIMER1_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define TIMER1_CAPTURE_SHIFT                  (0u)
#define TIMER1_COUNT_SHIFT                    (2u)
#define TIMER1_RELOAD_SHIFT                   (4u)
#define TIMER1_STOP_SHIFT                     (6u)
#define TIMER1_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define TIMER1_CAPTURE_MASK                   ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                  TIMER1_CAPTURE_SHIFT))
#define TIMER1_COUNT_MASK                     ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                  TIMER1_COUNT_SHIFT))
#define TIMER1_RELOAD_MASK                    ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                  TIMER1_RELOAD_SHIFT))
#define TIMER1_STOP_MASK                      ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                  TIMER1_STOP_SHIFT))
#define TIMER1_START_MASK                     ((uint32)(TIMER1_2BIT_MASK        <<  \
                                                                  TIMER1_START_SHIFT))

/* MASK */
#define TIMER1_1BIT_MASK                      ((uint32)0x01u)
#define TIMER1_2BIT_MASK                      ((uint32)0x03u)
#define TIMER1_3BIT_MASK                      ((uint32)0x07u)
#define TIMER1_6BIT_MASK                      ((uint32)0x3Fu)
#define TIMER1_8BIT_MASK                      ((uint32)0xFFu)
#define TIMER1_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define TIMER1_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define TIMER1_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(TIMER1_QUAD_ENCODING_MODES     << TIMER1_QUAD_MODE_SHIFT))       |\
         ((uint32)(TIMER1_CONFIG                  << TIMER1_MODE_SHIFT)))

#define TIMER1_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(TIMER1_PWM_STOP_EVENT          << TIMER1_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(TIMER1_PWM_OUT_INVERT          << TIMER1_INV_OUT_SHIFT))         |\
         ((uint32)(TIMER1_PWM_OUT_N_INVERT        << TIMER1_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(TIMER1_PWM_MODE                << TIMER1_MODE_SHIFT)))

#define TIMER1_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(TIMER1_PWM_RUN_MODE         << TIMER1_ONESHOT_SHIFT))
            
#define TIMER1_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(TIMER1_PWM_ALIGN            << TIMER1_UPDOWN_SHIFT))

#define TIMER1_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(TIMER1_PWM_KILL_EVENT      << TIMER1_PWM_SYNC_KILL_SHIFT))

#define TIMER1_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(TIMER1_PWM_DEAD_TIME_CYCLE  << TIMER1_PRESCALER_SHIFT))

#define TIMER1_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(TIMER1_PWM_PRESCALER        << TIMER1_PRESCALER_SHIFT))

#define TIMER1_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(TIMER1_TC_PRESCALER            << TIMER1_PRESCALER_SHIFT))       |\
         ((uint32)(TIMER1_TC_COUNTER_MODE         << TIMER1_UPDOWN_SHIFT))          |\
         ((uint32)(TIMER1_TC_RUN_MODE             << TIMER1_ONESHOT_SHIFT))         |\
         ((uint32)(TIMER1_TC_COMP_CAP_MODE        << TIMER1_MODE_SHIFT)))
        
#define TIMER1_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(TIMER1_QUAD_PHIA_SIGNAL_MODE   << TIMER1_COUNT_SHIFT))           |\
         ((uint32)(TIMER1_QUAD_INDEX_SIGNAL_MODE  << TIMER1_RELOAD_SHIFT))          |\
         ((uint32)(TIMER1_QUAD_STOP_SIGNAL_MODE   << TIMER1_STOP_SHIFT))            |\
         ((uint32)(TIMER1_QUAD_PHIB_SIGNAL_MODE   << TIMER1_START_SHIFT)))

#define TIMER1_PWM_SIGNALS_MODES                                                              \
        (((uint32)(TIMER1_PWM_SWITCH_SIGNAL_MODE  << TIMER1_CAPTURE_SHIFT))         |\
         ((uint32)(TIMER1_PWM_COUNT_SIGNAL_MODE   << TIMER1_COUNT_SHIFT))           |\
         ((uint32)(TIMER1_PWM_RELOAD_SIGNAL_MODE  << TIMER1_RELOAD_SHIFT))          |\
         ((uint32)(TIMER1_PWM_STOP_SIGNAL_MODE    << TIMER1_STOP_SHIFT))            |\
         ((uint32)(TIMER1_PWM_START_SIGNAL_MODE   << TIMER1_START_SHIFT)))

#define TIMER1_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(TIMER1_TC_CAPTURE_SIGNAL_MODE  << TIMER1_CAPTURE_SHIFT))         |\
         ((uint32)(TIMER1_TC_COUNT_SIGNAL_MODE    << TIMER1_COUNT_SHIFT))           |\
         ((uint32)(TIMER1_TC_RELOAD_SIGNAL_MODE   << TIMER1_RELOAD_SHIFT))          |\
         ((uint32)(TIMER1_TC_STOP_SIGNAL_MODE     << TIMER1_STOP_SHIFT))            |\
         ((uint32)(TIMER1_TC_START_SIGNAL_MODE    << TIMER1_START_SHIFT)))
        
#define TIMER1_TIMER_UPDOWN_CNT_USED                                                          \
                ((TIMER1__COUNT_UPDOWN0 == TIMER1_TC_COUNTER_MODE)                  ||\
                 (TIMER1__COUNT_UPDOWN1 == TIMER1_TC_COUNTER_MODE))

#define TIMER1_PWM_UPDOWN_CNT_USED                                                            \
                ((TIMER1__CENTER == TIMER1_PWM_ALIGN)                               ||\
                 (TIMER1__ASYMMETRIC == TIMER1_PWM_ALIGN))               
        
#define TIMER1_PWM_PR_INIT_VALUE              (1u)
#define TIMER1_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_TIMER1_H */

/* [] END OF FILE */
