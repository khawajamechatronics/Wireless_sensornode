/*******************************************************************************
* File Name: EEPROM_WC.c  
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
#include "EEPROM_WC.h"

static EEPROM_WC_BACKUP_STRUCT  EEPROM_WC_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: EEPROM_WC_Sleep
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
*  \snippet EEPROM_WC_SUT.c usage_EEPROM_WC_Sleep_Wakeup
*******************************************************************************/
void EEPROM_WC_Sleep(void)
{
    #if defined(EEPROM_WC__PC)
        EEPROM_WC_backup.pcState = EEPROM_WC_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            EEPROM_WC_backup.usbState = EEPROM_WC_CR1_REG;
            EEPROM_WC_USB_POWER_REG |= EEPROM_WC_USBIO_ENTER_SLEEP;
            EEPROM_WC_CR1_REG &= EEPROM_WC_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(EEPROM_WC__SIO)
        EEPROM_WC_backup.sioState = EEPROM_WC_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        EEPROM_WC_SIO_REG &= (uint32)(~EEPROM_WC_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: EEPROM_WC_Wakeup
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
*  Refer to EEPROM_WC_Sleep() for an example usage.
*******************************************************************************/
void EEPROM_WC_Wakeup(void)
{
    #if defined(EEPROM_WC__PC)
        EEPROM_WC_PC = EEPROM_WC_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            EEPROM_WC_USB_POWER_REG &= EEPROM_WC_USBIO_EXIT_SLEEP_PH1;
            EEPROM_WC_CR1_REG = EEPROM_WC_backup.usbState;
            EEPROM_WC_USB_POWER_REG &= EEPROM_WC_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(EEPROM_WC__SIO)
        EEPROM_WC_SIO_REG = EEPROM_WC_backup.sioState;
    #endif
}


/* [] END OF FILE */
