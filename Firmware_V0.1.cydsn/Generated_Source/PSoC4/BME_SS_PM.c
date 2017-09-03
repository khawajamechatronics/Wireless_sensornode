/*******************************************************************************
* File Name: BME_SS.c  
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
#include "BME_SS.h"

static BME_SS_BACKUP_STRUCT  BME_SS_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: BME_SS_Sleep
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
*  \snippet BME_SS_SUT.c usage_BME_SS_Sleep_Wakeup
*******************************************************************************/
void BME_SS_Sleep(void)
{
    #if defined(BME_SS__PC)
        BME_SS_backup.pcState = BME_SS_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            BME_SS_backup.usbState = BME_SS_CR1_REG;
            BME_SS_USB_POWER_REG |= BME_SS_USBIO_ENTER_SLEEP;
            BME_SS_CR1_REG &= BME_SS_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BME_SS__SIO)
        BME_SS_backup.sioState = BME_SS_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        BME_SS_SIO_REG &= (uint32)(~BME_SS_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: BME_SS_Wakeup
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
*  Refer to BME_SS_Sleep() for an example usage.
*******************************************************************************/
void BME_SS_Wakeup(void)
{
    #if defined(BME_SS__PC)
        BME_SS_PC = BME_SS_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            BME_SS_USB_POWER_REG &= BME_SS_USBIO_EXIT_SLEEP_PH1;
            BME_SS_CR1_REG = BME_SS_backup.usbState;
            BME_SS_USB_POWER_REG &= BME_SS_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BME_SS__SIO)
        BME_SS_SIO_REG = BME_SS_backup.sioState;
    #endif
}


/* [] END OF FILE */
