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

#if !defined(CY_SCB_PVT_SPI_BME280_H)
#define CY_SCB_PVT_SPI_BME280_H

#include "SPI_BME280.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_BME280_SetI2CExtClkInterruptMode(interruptMask) SPI_BME280_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_BME280_ClearI2CExtClkInterruptSource(interruptMask) SPI_BME280_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_BME280_GetI2CExtClkInterruptSource()                (SPI_BME280_INTR_I2C_EC_REG)
#define SPI_BME280_GetI2CExtClkInterruptMode()                  (SPI_BME280_INTR_I2C_EC_MASK_REG)
#define SPI_BME280_GetI2CExtClkInterruptSourceMasked()          (SPI_BME280_INTR_I2C_EC_MASKED_REG)

#if (!SPI_BME280_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SPI_BME280_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SPI_BME280_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SPI_BME280_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SPI_BME280_CLEAR_INTR_SPI_EC(interruptMask)
    #define SPI_BME280_GetExtSpiClkInterruptSource()                 (SPI_BME280_INTR_SPI_EC_REG)
    #define SPI_BME280_GetExtSpiClkInterruptMode()                   (SPI_BME280_INTR_SPI_EC_MASK_REG)
    #define SPI_BME280_GetExtSpiClkInterruptSourceMasked()           (SPI_BME280_INTR_SPI_EC_MASKED_REG)
#endif /* (!SPI_BME280_CY_SCBIP_V1) */

#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_BME280_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SPI_BME280_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SPI_BME280_CUSTOM_INTR_HANDLER)
    extern cyisraddress SPI_BME280_customIntrHandler;
#endif /* !defined (CY_REMOVE_SPI_BME280_CUSTOM_INTR_HANDLER) */
#endif /* (SPI_BME280_SCB_IRQ_INTERNAL) */

extern SPI_BME280_BACKUP_STRUCT SPI_BME280_backup;

#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SPI_BME280_scbMode;
    extern uint8 SPI_BME280_scbEnableWake;
    extern uint8 SPI_BME280_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SPI_BME280_mode;
    extern uint8 SPI_BME280_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SPI_BME280_rxBuffer;
    extern uint8   SPI_BME280_rxDataBits;
    extern uint32  SPI_BME280_rxBufferSize;

    extern volatile uint8 * SPI_BME280_txBuffer;
    extern uint8   SPI_BME280_txDataBits;
    extern uint32  SPI_BME280_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SPI_BME280_numberOfAddr;
    extern uint8 SPI_BME280_subAddrSize;
#endif /* (SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SPI_BME280_SCB_MODE_I2C_CONST_CFG || \
        SPI_BME280_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SPI_BME280_IntrTxMask;
#endif /* (! (SPI_BME280_SCB_MODE_I2C_CONST_CFG || \
              SPI_BME280_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SPI_BME280_SCB_MODE_I2C_RUNTM_CFG     (SPI_BME280_SCB_MODE_I2C      == SPI_BME280_scbMode)
    #define SPI_BME280_SCB_MODE_SPI_RUNTM_CFG     (SPI_BME280_SCB_MODE_SPI      == SPI_BME280_scbMode)
    #define SPI_BME280_SCB_MODE_UART_RUNTM_CFG    (SPI_BME280_SCB_MODE_UART     == SPI_BME280_scbMode)
    #define SPI_BME280_SCB_MODE_EZI2C_RUNTM_CFG   (SPI_BME280_SCB_MODE_EZI2C    == SPI_BME280_scbMode)
    #define SPI_BME280_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SPI_BME280_SCB_MODE_UNCONFIG == SPI_BME280_scbMode)

    /* Defines wakeup enable */
    #define SPI_BME280_SCB_WAKE_ENABLE_CHECK       (0u != SPI_BME280_scbEnableWake)
#endif /* (SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SPI_BME280_CY_SCBIP_V1)
    #define SPI_BME280_SCB_PINS_NUMBER    (7u)
#else
    #define SPI_BME280_SCB_PINS_NUMBER    (2u)
#endif /* (!SPI_BME280_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SPI_BME280_H) */


/* [] END OF FILE */
