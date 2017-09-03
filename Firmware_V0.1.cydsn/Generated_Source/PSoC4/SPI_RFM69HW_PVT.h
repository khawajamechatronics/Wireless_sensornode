/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_SPI_RFM69HW_H)
#define CY_SCB_PVT_SPI_RFM69HW_H

#include "SPI_RFM69HW.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_RFM69HW_SetI2CExtClkInterruptMode(interruptMask) SPI_RFM69HW_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_RFM69HW_ClearI2CExtClkInterruptSource(interruptMask) SPI_RFM69HW_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_RFM69HW_GetI2CExtClkInterruptSource()                (SPI_RFM69HW_INTR_I2C_EC_REG)
#define SPI_RFM69HW_GetI2CExtClkInterruptMode()                  (SPI_RFM69HW_INTR_I2C_EC_MASK_REG)
#define SPI_RFM69HW_GetI2CExtClkInterruptSourceMasked()          (SPI_RFM69HW_INTR_I2C_EC_MASKED_REG)

#if (!SPI_RFM69HW_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SPI_RFM69HW_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SPI_RFM69HW_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SPI_RFM69HW_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SPI_RFM69HW_CLEAR_INTR_SPI_EC(interruptMask)
    #define SPI_RFM69HW_GetExtSpiClkInterruptSource()                 (SPI_RFM69HW_INTR_SPI_EC_REG)
    #define SPI_RFM69HW_GetExtSpiClkInterruptMode()                   (SPI_RFM69HW_INTR_SPI_EC_MASK_REG)
    #define SPI_RFM69HW_GetExtSpiClkInterruptSourceMasked()           (SPI_RFM69HW_INTR_SPI_EC_MASKED_REG)
#endif /* (!SPI_RFM69HW_CY_SCBIP_V1) */

#if(SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_RFM69HW_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SPI_RFM69HW_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SPI_RFM69HW_CUSTOM_INTR_HANDLER)
    extern cyisraddress SPI_RFM69HW_customIntrHandler;
#endif /* !defined (CY_REMOVE_SPI_RFM69HW_CUSTOM_INTR_HANDLER) */
#endif /* (SPI_RFM69HW_SCB_IRQ_INTERNAL) */

extern SPI_RFM69HW_BACKUP_STRUCT SPI_RFM69HW_backup;

#if(SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SPI_RFM69HW_scbMode;
    extern uint8 SPI_RFM69HW_scbEnableWake;
    extern uint8 SPI_RFM69HW_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SPI_RFM69HW_mode;
    extern uint8 SPI_RFM69HW_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SPI_RFM69HW_rxBuffer;
    extern uint8   SPI_RFM69HW_rxDataBits;
    extern uint32  SPI_RFM69HW_rxBufferSize;

    extern volatile uint8 * SPI_RFM69HW_txBuffer;
    extern uint8   SPI_RFM69HW_txDataBits;
    extern uint32  SPI_RFM69HW_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SPI_RFM69HW_numberOfAddr;
    extern uint8 SPI_RFM69HW_subAddrSize;
#endif /* (SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SPI_RFM69HW_SCB_MODE_I2C_CONST_CFG || \
        SPI_RFM69HW_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SPI_RFM69HW_IntrTxMask;
#endif /* (! (SPI_RFM69HW_SCB_MODE_I2C_CONST_CFG || \
              SPI_RFM69HW_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SPI_RFM69HW_SCB_MODE_I2C_RUNTM_CFG     (SPI_RFM69HW_SCB_MODE_I2C      == SPI_RFM69HW_scbMode)
    #define SPI_RFM69HW_SCB_MODE_SPI_RUNTM_CFG     (SPI_RFM69HW_SCB_MODE_SPI      == SPI_RFM69HW_scbMode)
    #define SPI_RFM69HW_SCB_MODE_UART_RUNTM_CFG    (SPI_RFM69HW_SCB_MODE_UART     == SPI_RFM69HW_scbMode)
    #define SPI_RFM69HW_SCB_MODE_EZI2C_RUNTM_CFG   (SPI_RFM69HW_SCB_MODE_EZI2C    == SPI_RFM69HW_scbMode)
    #define SPI_RFM69HW_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SPI_RFM69HW_SCB_MODE_UNCONFIG == SPI_RFM69HW_scbMode)

    /* Defines wakeup enable */
    #define SPI_RFM69HW_SCB_WAKE_ENABLE_CHECK       (0u != SPI_RFM69HW_scbEnableWake)
#endif /* (SPI_RFM69HW_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SPI_RFM69HW_CY_SCBIP_V1)
    #define SPI_RFM69HW_SCB_PINS_NUMBER    (7u)
#else
    #define SPI_RFM69HW_SCB_PINS_NUMBER    (2u)
#endif /* (!SPI_RFM69HW_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SPI_RFM69HW_H) */


/* [] END OF FILE */
