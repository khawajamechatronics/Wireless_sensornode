/*******************************************************************************
* File Name: IDAC_3.h
* Version 1.10
*
* Description:
*  This file provides constants and parameter values for the IDAC_P4
*  component.
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_IDAC_IDAC_3_H)
#define CY_IDAC_IDAC_3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Internal Type defines
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} IDAC_3_BACKUP_STRUCT;


extern uint32 IDAC_3_initVar;

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* IDAC polarity setting constants */
#define IDAC_3__SOURCE 0
#define IDAC_3__SINK 1


/* IDAC range setting constants */
#define IDAC_3__LOWRANGE 0
#define IDAC_3__HIGHRANGE 1


/* IDAC polarity setting definitions */
#define IDAC_3_MODE_SOURCE    (IDAC_3__SOURCE)
#define IDAC_3_MODE_SINK      (IDAC_3__SINK)

/* IDAC RANGE setting definitions */
#define IDAC_3_MODE_LOWRANGE  (IDAC_3__LOWRANGE)
#define IDAC_3_MODE_HIGHRANGE (IDAC_3__HIGHRANGE)

/***************************************
*   Conditional Compilation Parameters
****************************************/

#define IDAC_3_IDAC_RESOLUTION    (7u)
#define IDAC_3_IDAC_RANGE         (0u)
#define IDAC_3_IDAC_POLARITY      (1u)


/***************************************
*    Initial Parameter Constants
***************************************/
#define IDAC_3_IDAC_INIT_VALUE    (120u)




/***************************************
*        Function Prototypes
***************************************/

void IDAC_3_Init(void);
void IDAC_3_Enable(void);
void IDAC_3_Start(void);
void IDAC_3_Stop(void);
void IDAC_3_SetValue(uint32  value);
void IDAC_3_SaveConfig(void);
void IDAC_3_Sleep(void);
void IDAC_3_RestoreConfig(void);
void IDAC_3_Wakeup(void);


/***************************************
*            API Constants
***************************************/

#define IDAC_3_IDAC_EN_MODE           (3u)
#define IDAC_3_IDAC_CSD_EN            (1u)
#define IDAC_3_IDAC_CSD_EN_POSITION   (31u)

#define IDAC_3_IDAC_VALUE_POSITION    (IDAC_3_cy_psoc4_idac__CSD_IDAC_SHIFT)

#define IDAC_3_IDAC_MODE_SHIFT        (8u)
#define IDAC_3_IDAC_MODE_POSITION     ((uint32)IDAC_3_cy_psoc4_idac__CSD_IDAC_SHIFT +\
                                                 IDAC_3_IDAC_MODE_SHIFT)

#define IDAC_3_IDAC_RANGE_SHIFT       (10u)
#define IDAC_3_IDAC_RANGE_POSITION    ((uint32)IDAC_3_cy_psoc4_idac__CSD_IDAC_SHIFT +\
                                                 IDAC_3_IDAC_RANGE_SHIFT)

#define IDAC_3_IDAC_POLARITY_POSITION ((uint32)IDAC_3_cy_psoc4_idac__POLARITY_SHIFT)

#define IDAC_3_IDAC_TRIM1_POSITION    ((uint32)IDAC_3_cy_psoc4_idac__CSD_TRIM1_SHIFT)
#define IDAC_3_IDAC_TRIM2_POSITION    ((uint32)IDAC_3_cy_psoc4_idac__CSD_TRIM2_SHIFT)

#define IDAC_3_IDAC_CDS_EN_MASK       (0x80000000u)

#if(IDAC_3_IDAC_RESOLUTION == 8u)
  #define IDAC_3_IDAC_VALUE_MASK      (0xFFu)
#else
  #define IDAC_3_IDAC_VALUE_MASK      (0x7Fu)
#endif /* (IDAC_3_IDAC_RESOLUTION == 8u) */

#define IDAC_3_IDAC_MODE_MASK         (3u)
#define IDAC_3_IDAC_RANGE_MASK        (1u)
#define IDAC_3_IDAC_POLARITY_MASK     (1u)

#define IDAC_3_CSD_IDAC_TRIM1_MASK    (0x0000000FuL << IDAC_3_IDAC_TRIM1_POSITION)
#define IDAC_3_CSD_IDAC_TRIM2_MASK    (0x0000000FuL << IDAC_3_IDAC_TRIM2_POSITION)


/***************************************
*        Registers
***************************************/

#define IDAC_3_IDAC_CONTROL_REG    (*(reg32 *)IDAC_3_cy_psoc4_idac__CSD_IDAC)
#define IDAC_3_IDAC_CONTROL_PTR    ( (reg32 *)IDAC_3_cy_psoc4_idac__CSD_IDAC)

#define IDAC_3_IDAC_POLARITY_CONTROL_REG    (*(reg32 *)IDAC_3_cy_psoc4_idac__CONTROL)
#define IDAC_3_IDAC_POLARITY_CONTROL_PTR    ( (reg32 *)IDAC_3_cy_psoc4_idac__CONTROL)

#define IDAC_3_CSD_TRIM1_REG       (*(reg32 *)IDAC_3_cy_psoc4_idac__CSD_TRIM1)
#define IDAC_3_CSD_TRIM1_PTR       ( (reg32 *)IDAC_3_cy_psoc4_idac__CSD_TRIM1)

#define IDAC_3_CSD_TRIM2_REG       (*(reg32 *)IDAC_3_cy_psoc4_idac__CSD_TRIM2)
#define IDAC_3_CSD_TRIM2_PTR       ( (reg32 *)IDAC_3_cy_psoc4_idac__CSD_TRIM2)

#if (CY_PSOC4_4100M || CY_PSOC4_4200M)
    #if(IDAC_3_cy_psoc4_idac__IDAC_NUMBER > 2u)
        #define IDAC_3_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD1_TRIM1_HVIDAC)
        #define IDAC_3_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD1_TRIM1_HVIDAC)
        
        #define IDAC_3_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD1_TRIM2_HVIDAC)
        #define IDAC_3_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD1_TRIM2_HVIDAC)
    #else
        #define IDAC_3_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
        #define IDAC_3_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
        
        #define IDAC_3_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
        #define IDAC_3_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
    #endif /* (IDAC_3_cy_psoc4_idac__IDAC_NUMBER > 2u) */
#else
    #define IDAC_3_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
    #define IDAC_3_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
    
    #define IDAC_3_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
    #define IDAC_3_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
#endif /* (CY_PSOC4_4100M || CY_PSOC4_4200M) */

#endif /* CY_IDAC_IDAC_3_H */

/* [] END OF FILE */
