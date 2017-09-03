/*******************************************************************************
* File Name: BME_MOSI.h  
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

#if !defined(CY_PINS_BME_MOSI_H) /* Pins BME_MOSI_H */
#define CY_PINS_BME_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "BME_MOSI_aliases.h"


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
} BME_MOSI_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   BME_MOSI_Read(void);
void    BME_MOSI_Write(uint8 value);
uint8   BME_MOSI_ReadDataReg(void);
#if defined(BME_MOSI__PC) || (CY_PSOC4_4200L) 
    void    BME_MOSI_SetDriveMode(uint8 mode);
#endif
void    BME_MOSI_SetInterruptMode(uint16 position, uint16 mode);
uint8   BME_MOSI_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void BME_MOSI_Sleep(void); 
void BME_MOSI_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(BME_MOSI__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define BME_MOSI_DRIVE_MODE_BITS        (3)
    #define BME_MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - BME_MOSI_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the BME_MOSI_SetDriveMode() function.
         *  @{
         */
        #define BME_MOSI_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define BME_MOSI_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define BME_MOSI_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define BME_MOSI_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define BME_MOSI_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define BME_MOSI_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define BME_MOSI_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define BME_MOSI_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define BME_MOSI_MASK               BME_MOSI__MASK
#define BME_MOSI_SHIFT              BME_MOSI__SHIFT
#define BME_MOSI_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BME_MOSI_SetInterruptMode() function.
     *  @{
     */
        #define BME_MOSI_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define BME_MOSI_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define BME_MOSI_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define BME_MOSI_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(BME_MOSI__SIO)
    #define BME_MOSI_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(BME_MOSI__PC) && (CY_PSOC4_4200L)
    #define BME_MOSI_USBIO_ENABLE               ((uint32)0x80000000u)
    #define BME_MOSI_USBIO_DISABLE              ((uint32)(~BME_MOSI_USBIO_ENABLE))
    #define BME_MOSI_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define BME_MOSI_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define BME_MOSI_USBIO_ENTER_SLEEP          ((uint32)((1u << BME_MOSI_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << BME_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define BME_MOSI_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << BME_MOSI_USBIO_SUSPEND_SHIFT)))
    #define BME_MOSI_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << BME_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define BME_MOSI_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(BME_MOSI__PC)
    /* Port Configuration */
    #define BME_MOSI_PC                 (* (reg32 *) BME_MOSI__PC)
#endif
/* Pin State */
#define BME_MOSI_PS                     (* (reg32 *) BME_MOSI__PS)
/* Data Register */
#define BME_MOSI_DR                     (* (reg32 *) BME_MOSI__DR)
/* Input Buffer Disable Override */
#define BME_MOSI_INP_DIS                (* (reg32 *) BME_MOSI__PC2)

/* Interrupt configuration Registers */
#define BME_MOSI_INTCFG                 (* (reg32 *) BME_MOSI__INTCFG)
#define BME_MOSI_INTSTAT                (* (reg32 *) BME_MOSI__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define BME_MOSI_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(BME_MOSI__SIO)
    #define BME_MOSI_SIO_REG            (* (reg32 *) BME_MOSI__SIO)
#endif /* (BME_MOSI__SIO_CFG) */

/* USBIO registers */
#if !defined(BME_MOSI__PC) && (CY_PSOC4_4200L)
    #define BME_MOSI_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define BME_MOSI_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define BME_MOSI_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define BME_MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define BME_MOSI_DRIVE_MODE_MASK        (0x07u << BME_MOSI_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins BME_MOSI_H */


/* [] END OF FILE */
