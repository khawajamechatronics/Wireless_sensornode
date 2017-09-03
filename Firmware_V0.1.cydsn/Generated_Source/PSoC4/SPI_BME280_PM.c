/***************************************************************************//**
* \file SPI_BME280_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_BME280.h"
#include "SPI_BME280_PVT.h"

#if(SPI_BME280_SCB_MODE_I2C_INC)
    #include "SPI_BME280_I2C_PVT.h"
#endif /* (SPI_BME280_SCB_MODE_I2C_INC) */

#if(SPI_BME280_SCB_MODE_EZI2C_INC)
    #include "SPI_BME280_EZI2C_PVT.h"
#endif /* (SPI_BME280_SCB_MODE_EZI2C_INC) */

#if(SPI_BME280_SCB_MODE_SPI_INC || SPI_BME280_SCB_MODE_UART_INC)
    #include "SPI_BME280_SPI_UART_PVT.h"
#endif /* (SPI_BME280_SCB_MODE_SPI_INC || SPI_BME280_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG || \
   (SPI_BME280_SCB_MODE_I2C_CONST_CFG   && (!SPI_BME280_I2C_WAKE_ENABLE_CONST))   || \
   (SPI_BME280_SCB_MODE_EZI2C_CONST_CFG && (!SPI_BME280_EZI2C_WAKE_ENABLE_CONST)) || \
   (SPI_BME280_SCB_MODE_SPI_CONST_CFG   && (!SPI_BME280_SPI_WAKE_ENABLE_CONST))   || \
   (SPI_BME280_SCB_MODE_UART_CONST_CFG  && (!SPI_BME280_UART_WAKE_ENABLE_CONST)))

    SPI_BME280_BACKUP_STRUCT SPI_BME280_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: SPI_BME280_Sleep
****************************************************************************//**
*
*  Prepares the SPI_BME280 component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the SPI_BME280_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void SPI_BME280_Sleep(void)
{
#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SPI_BME280_SCB_WAKE_ENABLE_CHECK)
    {
        if(SPI_BME280_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_BME280_I2CSaveConfig();
        }
        else if(SPI_BME280_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_BME280_EzI2CSaveConfig();
        }
    #if(!SPI_BME280_CY_SCBIP_V1)
        else if(SPI_BME280_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_BME280_SpiSaveConfig();
        }
        else if(SPI_BME280_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_BME280_UartSaveConfig();
        }
    #endif /* (!SPI_BME280_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        SPI_BME280_backup.enableState = (uint8) SPI_BME280_GET_CTRL_ENABLED;

        if(0u != SPI_BME280_backup.enableState)
        {
            SPI_BME280_Stop();
        }
    }

#else

    #if (SPI_BME280_SCB_MODE_I2C_CONST_CFG && SPI_BME280_I2C_WAKE_ENABLE_CONST)
        SPI_BME280_I2CSaveConfig();

    #elif (SPI_BME280_SCB_MODE_EZI2C_CONST_CFG && SPI_BME280_EZI2C_WAKE_ENABLE_CONST)
        SPI_BME280_EzI2CSaveConfig();

    #elif (SPI_BME280_SCB_MODE_SPI_CONST_CFG && SPI_BME280_SPI_WAKE_ENABLE_CONST)
        SPI_BME280_SpiSaveConfig();

    #elif (SPI_BME280_SCB_MODE_UART_CONST_CFG && SPI_BME280_UART_WAKE_ENABLE_CONST)
        SPI_BME280_UartSaveConfig();

    #else

        SPI_BME280_backup.enableState = (uint8) SPI_BME280_GET_CTRL_ENABLED;

        if(0u != SPI_BME280_backup.enableState)
        {
            SPI_BME280_Stop();
        }

    #endif /* defined (SPI_BME280_SCB_MODE_I2C_CONST_CFG) && (SPI_BME280_I2C_WAKE_ENABLE_CONST) */

#endif /* (SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_BME280_Wakeup
****************************************************************************//**
*
*  Prepares the SPI_BME280 component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the SPI_BME280_Wakeup() function without first calling the 
*   SPI_BME280_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void SPI_BME280_Wakeup(void)
{
#if(SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SPI_BME280_SCB_WAKE_ENABLE_CHECK)
    {
        if(SPI_BME280_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_BME280_I2CRestoreConfig();
        }
        else if(SPI_BME280_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_BME280_EzI2CRestoreConfig();
        }
    #if(!SPI_BME280_CY_SCBIP_V1)
        else if(SPI_BME280_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_BME280_SpiRestoreConfig();
        }
        else if(SPI_BME280_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_BME280_UartRestoreConfig();
        }
    #endif /* (!SPI_BME280_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != SPI_BME280_backup.enableState)
        {
            SPI_BME280_Enable();
        }
    }

#else

    #if (SPI_BME280_SCB_MODE_I2C_CONST_CFG  && SPI_BME280_I2C_WAKE_ENABLE_CONST)
        SPI_BME280_I2CRestoreConfig();

    #elif (SPI_BME280_SCB_MODE_EZI2C_CONST_CFG && SPI_BME280_EZI2C_WAKE_ENABLE_CONST)
        SPI_BME280_EzI2CRestoreConfig();

    #elif (SPI_BME280_SCB_MODE_SPI_CONST_CFG && SPI_BME280_SPI_WAKE_ENABLE_CONST)
        SPI_BME280_SpiRestoreConfig();

    #elif (SPI_BME280_SCB_MODE_UART_CONST_CFG && SPI_BME280_UART_WAKE_ENABLE_CONST)
        SPI_BME280_UartRestoreConfig();

    #else

        if(0u != SPI_BME280_backup.enableState)
        {
            SPI_BME280_Enable();
        }

    #endif /* (SPI_BME280_I2C_WAKE_ENABLE_CONST) */

#endif /* (SPI_BME280_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
