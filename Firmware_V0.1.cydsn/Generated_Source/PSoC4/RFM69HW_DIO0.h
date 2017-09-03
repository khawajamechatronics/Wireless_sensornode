/*******************************************************************************
* File Name: RFM69HW_DIO0.h  
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

#if !defined(CY_PINS_RFM69HW_DIO0_H) /* Pins RFM69HW_DIO0_H */
#define CY_PINS_RFM69HW_DIO0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RFM69HW_DIO0_aliases.h"


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
} RFM69HW_DIO0_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RFM69HW_DIO0_Read(void);
void    RFM69HW_DIO0_Write(uint8 value);
uint8   RFM69HW_DIO0_ReadDataReg(void);
#if defined(RFM69HW_DIO0__PC) || (CY_PSOC4_4200L) 
    void    RFM69HW_DIO0_SetDriveMode(uint8 mode);
#endif
void    RFM69HW_DIO0_SetInterruptMode(uint16 position, uint16 mode);
uint8   RFM69HW_DIO0_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RFM69HW_DIO0_Sleep(void); 
void RFM69HW_DIO0_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RFM69HW_DIO0__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RFM69HW_DIO0_DRIVE_MODE_BITS        (3)
    #define RFM69HW_DIO0_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RFM69HW_DIO0_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RFM69HW_DIO0_SetDriveMode() function.
         *  @{
         */
        #define RFM69HW_DIO0_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RFM69HW_DIO0_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RFM69HW_DIO0_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RFM69HW_DIO0_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RFM69HW_DIO0_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RFM69HW_DIO0_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RFM69HW_DIO0_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RFM69HW_DIO0_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RFM69HW_DIO0_MASK               RFM69HW_DIO0__MASK
#define RFM69HW_DIO0_SHIFT              RFM69HW_DIO0__SHIFT
#define RFM69HW_DIO0_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RFM69HW_DIO0_SetInterruptMode() function.
     *  @{
     */
        #define RFM69HW_DIO0_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RFM69HW_DIO0_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RFM69HW_DIO0_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RFM69HW_DIO0_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RFM69HW_DIO0__SIO)
    #define RFM69HW_DIO0_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RFM69HW_DIO0__PC) && (CY_PSOC4_4200L)
    #define RFM69HW_DIO0_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RFM69HW_DIO0_USBIO_DISABLE              ((uint32)(~RFM69HW_DIO0_USBIO_ENABLE))
    #define RFM69HW_DIO0_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RFM69HW_DIO0_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RFM69HW_DIO0_USBIO_ENTER_SLEEP          ((uint32)((1u << RFM69HW_DIO0_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RFM69HW_DIO0_USBIO_SUSPEND_DEL_SHIFT)))
    #define RFM69HW_DIO0_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RFM69HW_DIO0_USBIO_SUSPEND_SHIFT)))
    #define RFM69HW_DIO0_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RFM69HW_DIO0_USBIO_SUSPEND_DEL_SHIFT)))
    #define RFM69HW_DIO0_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RFM69HW_DIO0__PC)
    /* Port Configuration */
    #define RFM69HW_DIO0_PC                 (* (reg32 *) RFM69HW_DIO0__PC)
#endif
/* Pin State */
#define RFM69HW_DIO0_PS                     (* (reg32 *) RFM69HW_DIO0__PS)
/* Data Register */
#define RFM69HW_DIO0_DR                     (* (reg32 *) RFM69HW_DIO0__DR)
/* Input Buffer Disable Override */
#define RFM69HW_DIO0_INP_DIS                (* (reg32 *) RFM69HW_DIO0__PC2)

/* Interrupt configuration Registers */
#define RFM69HW_DIO0_INTCFG                 (* (reg32 *) RFM69HW_DIO0__INTCFG)
#define RFM69HW_DIO0_INTSTAT                (* (reg32 *) RFM69HW_DIO0__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RFM69HW_DIO0_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RFM69HW_DIO0__SIO)
    #define RFM69HW_DIO0_SIO_REG            (* (reg32 *) RFM69HW_DIO0__SIO)
#endif /* (RFM69HW_DIO0__SIO_CFG) */

/* USBIO registers */
#if !defined(RFM69HW_DIO0__PC) && (CY_PSOC4_4200L)
    #define RFM69HW_DIO0_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RFM69HW_DIO0_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RFM69HW_DIO0_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RFM69HW_DIO0_DRIVE_MODE_SHIFT       (0x00u)
#define RFM69HW_DIO0_DRIVE_MODE_MASK        (0x07u << RFM69HW_DIO0_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RFM69HW_DIO0_H */


/* [] END OF FILE */
