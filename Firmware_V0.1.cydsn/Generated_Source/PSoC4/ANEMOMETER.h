/*******************************************************************************
* File Name: ANEMOMETER.h  
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

#if !defined(CY_PINS_ANEMOMETER_H) /* Pins ANEMOMETER_H */
#define CY_PINS_ANEMOMETER_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ANEMOMETER_aliases.h"


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
} ANEMOMETER_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   ANEMOMETER_Read(void);
void    ANEMOMETER_Write(uint8 value);
uint8   ANEMOMETER_ReadDataReg(void);
#if defined(ANEMOMETER__PC) || (CY_PSOC4_4200L) 
    void    ANEMOMETER_SetDriveMode(uint8 mode);
#endif
void    ANEMOMETER_SetInterruptMode(uint16 position, uint16 mode);
uint8   ANEMOMETER_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void ANEMOMETER_Sleep(void); 
void ANEMOMETER_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(ANEMOMETER__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define ANEMOMETER_DRIVE_MODE_BITS        (3)
    #define ANEMOMETER_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ANEMOMETER_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the ANEMOMETER_SetDriveMode() function.
         *  @{
         */
        #define ANEMOMETER_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define ANEMOMETER_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define ANEMOMETER_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define ANEMOMETER_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define ANEMOMETER_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define ANEMOMETER_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define ANEMOMETER_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define ANEMOMETER_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define ANEMOMETER_MASK               ANEMOMETER__MASK
#define ANEMOMETER_SHIFT              ANEMOMETER__SHIFT
#define ANEMOMETER_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ANEMOMETER_SetInterruptMode() function.
     *  @{
     */
        #define ANEMOMETER_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define ANEMOMETER_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define ANEMOMETER_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define ANEMOMETER_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(ANEMOMETER__SIO)
    #define ANEMOMETER_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(ANEMOMETER__PC) && (CY_PSOC4_4200L)
    #define ANEMOMETER_USBIO_ENABLE               ((uint32)0x80000000u)
    #define ANEMOMETER_USBIO_DISABLE              ((uint32)(~ANEMOMETER_USBIO_ENABLE))
    #define ANEMOMETER_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define ANEMOMETER_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define ANEMOMETER_USBIO_ENTER_SLEEP          ((uint32)((1u << ANEMOMETER_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << ANEMOMETER_USBIO_SUSPEND_DEL_SHIFT)))
    #define ANEMOMETER_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << ANEMOMETER_USBIO_SUSPEND_SHIFT)))
    #define ANEMOMETER_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << ANEMOMETER_USBIO_SUSPEND_DEL_SHIFT)))
    #define ANEMOMETER_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(ANEMOMETER__PC)
    /* Port Configuration */
    #define ANEMOMETER_PC                 (* (reg32 *) ANEMOMETER__PC)
#endif
/* Pin State */
#define ANEMOMETER_PS                     (* (reg32 *) ANEMOMETER__PS)
/* Data Register */
#define ANEMOMETER_DR                     (* (reg32 *) ANEMOMETER__DR)
/* Input Buffer Disable Override */
#define ANEMOMETER_INP_DIS                (* (reg32 *) ANEMOMETER__PC2)

/* Interrupt configuration Registers */
#define ANEMOMETER_INTCFG                 (* (reg32 *) ANEMOMETER__INTCFG)
#define ANEMOMETER_INTSTAT                (* (reg32 *) ANEMOMETER__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define ANEMOMETER_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(ANEMOMETER__SIO)
    #define ANEMOMETER_SIO_REG            (* (reg32 *) ANEMOMETER__SIO)
#endif /* (ANEMOMETER__SIO_CFG) */

/* USBIO registers */
#if !defined(ANEMOMETER__PC) && (CY_PSOC4_4200L)
    #define ANEMOMETER_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define ANEMOMETER_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define ANEMOMETER_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define ANEMOMETER_DRIVE_MODE_SHIFT       (0x00u)
#define ANEMOMETER_DRIVE_MODE_MASK        (0x07u << ANEMOMETER_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins ANEMOMETER_H */


/* [] END OF FILE */
