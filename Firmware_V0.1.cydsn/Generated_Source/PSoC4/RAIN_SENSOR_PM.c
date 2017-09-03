/*******************************************************************************
* File Name: RAIN_SENSOR.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "RAIN_SENSOR.h"

static RAIN_SENSOR_BACKUP_STRUCT  RAIN_SENSOR_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: RAIN_SENSOR_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet RAIN_SENSOR_SUT.c usage_RAIN_SENSOR_Sleep_Wakeup
*******************************************************************************/
void RAIN_SENSOR_Sleep(void)
{
    #if defined(RAIN_SENSOR__PC)
        RAIN_SENSOR_backup.pcState = RAIN_SENSOR_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            RAIN_SENSOR_backup.usbState = RAIN_SENSOR_CR1_REG;
            RAIN_SENSOR_USB_POWER_REG |= RAIN_SENSOR_USBIO_ENTER_SLEEP;
            RAIN_SENSOR_CR1_REG &= RAIN_SENSOR_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RAIN_SENSOR__SIO)
        RAIN_SENSOR_backup.sioState = RAIN_SENSOR_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        RAIN_SENSOR_SIO_REG &= (uint32)(~RAIN_SENSOR_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: RAIN_SENSOR_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to RAIN_SENSOR_Sleep() for an example usage.
*******************************************************************************/
void RAIN_SENSOR_Wakeup(void)
{
    #if defined(RAIN_SENSOR__PC)
        RAIN_SENSOR_PC = RAIN_SENSOR_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            RAIN_SENSOR_USB_POWER_REG &= RAIN_SENSOR_USBIO_EXIT_SLEEP_PH1;
            RAIN_SENSOR_CR1_REG = RAIN_SENSOR_backup.usbState;
            RAIN_SENSOR_USB_POWER_REG &= RAIN_SENSOR_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RAIN_SENSOR__SIO)
        RAIN_SENSOR_SIO_REG = RAIN_SENSOR_backup.sioState;
    #endif
}


/* [] END OF FILE */
