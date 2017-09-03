/*******************************************************************************
* File Name: EEPROM_PULLUP.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_EEPROM_PULLUP_H) /* Pins EEPROM_PULLUP_H */
#define CY_PINS_EEPROM_PULLUP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "EEPROM_PULLUP_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} EEPROM_PULLUP_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   EEPROM_PULLUP_Read(void);
void    EEPROM_PULLUP_Write(uint8 value);
uint8   EEPROM_PULLUP_ReadDataReg(void);
#if defined(EEPROM_PULLUP__PC) || (CY_PSOC4_4200L) 
    void    EEPROM_PULLUP_SetDriveMode(uint8 mode);
#endif
void    EEPROM_PULLUP_SetInterruptMode(uint16 position, uint16 mode);
uint8   EEPROM_PULLUP_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void EEPROM_PULLUP_Sleep(void); 
void EEPROM_PULLUP_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(EEPROM_PULLUP__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define EEPROM_PULLUP_DRIVE_MODE_BITS        (3)
    #define EEPROM_PULLUP_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - EEPROM_PULLUP_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the EEPROM_PULLUP_SetDriveMode() function.
         *  @{
         */
        #define EEPROM_PULLUP_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define EEPROM_PULLUP_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define EEPROM_PULLUP_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define EEPROM_PULLUP_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define EEPROM_PULLUP_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define EEPROM_PULLUP_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define EEPROM_PULLUP_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define EEPROM_PULLUP_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define EEPROM_PULLUP_MASK               EEPROM_PULLUP__MASK
#define EEPROM_PULLUP_SHIFT              EEPROM_PULLUP__SHIFT
#define EEPROM_PULLUP_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in EEPROM_PULLUP_SetInterruptMode() function.
     *  @{
     */
        #define EEPROM_PULLUP_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define EEPROM_PULLUP_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define EEPROM_PULLUP_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define EEPROM_PULLUP_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(EEPROM_PULLUP__SIO)
    #define EEPROM_PULLUP_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(EEPROM_PULLUP__PC) && (CY_PSOC4_4200L)
    #define EEPROM_PULLUP_USBIO_ENABLE               ((uint32)0x80000000u)
    #define EEPROM_PULLUP_USBIO_DISABLE              ((uint32)(~EEPROM_PULLUP_USBIO_ENABLE))
    #define EEPROM_PULLUP_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define EEPROM_PULLUP_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define EEPROM_PULLUP_USBIO_ENTER_SLEEP          ((uint32)((1u << EEPROM_PULLUP_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << EEPROM_PULLUP_USBIO_SUSPEND_DEL_SHIFT)))
    #define EEPROM_PULLUP_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << EEPROM_PULLUP_USBIO_SUSPEND_SHIFT)))
    #define EEPROM_PULLUP_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << EEPROM_PULLUP_USBIO_SUSPEND_DEL_SHIFT)))
    #define EEPROM_PULLUP_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(EEPROM_PULLUP__PC)
    /* Port Configuration */
    #define EEPROM_PULLUP_PC                 (* (reg32 *) EEPROM_PULLUP__PC)
#endif
/* Pin State */
#define EEPROM_PULLUP_PS                     (* (reg32 *) EEPROM_PULLUP__PS)
/* Data Register */
#define EEPROM_PULLUP_DR                     (* (reg32 *) EEPROM_PULLUP__DR)
/* Input Buffer Disable Override */
#define EEPROM_PULLUP_INP_DIS                (* (reg32 *) EEPROM_PULLUP__PC2)

/* Interrupt configuration Registers */
#define EEPROM_PULLUP_INTCFG                 (* (reg32 *) EEPROM_PULLUP__INTCFG)
#define EEPROM_PULLUP_INTSTAT                (* (reg32 *) EEPROM_PULLUP__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define EEPROM_PULLUP_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(EEPROM_PULLUP__SIO)
    #define EEPROM_PULLUP_SIO_REG            (* (reg32 *) EEPROM_PULLUP__SIO)
#endif /* (EEPROM_PULLUP__SIO_CFG) */

/* USBIO registers */
#if !defined(EEPROM_PULLUP__PC) && (CY_PSOC4_4200L)
    #define EEPROM_PULLUP_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define EEPROM_PULLUP_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define EEPROM_PULLUP_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define EEPROM_PULLUP_DRIVE_MODE_SHIFT       (0x00u)
#define EEPROM_PULLUP_DRIVE_MODE_MASK        (0x07u << EEPROM_PULLUP_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins EEPROM_PULLUP_H */


/* [] END OF FILE */
