/*******************************************************************************
* File Name: RAIN_SENSOR.h  
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

#if !defined(CY_PINS_RAIN_SENSOR_H) /* Pins RAIN_SENSOR_H */
#define CY_PINS_RAIN_SENSOR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RAIN_SENSOR_aliases.h"


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
} RAIN_SENSOR_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RAIN_SENSOR_Read(void);
void    RAIN_SENSOR_Write(uint8 value);
uint8   RAIN_SENSOR_ReadDataReg(void);
#if defined(RAIN_SENSOR__PC) || (CY_PSOC4_4200L) 
    void    RAIN_SENSOR_SetDriveMode(uint8 mode);
#endif
void    RAIN_SENSOR_SetInterruptMode(uint16 position, uint16 mode);
uint8   RAIN_SENSOR_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RAIN_SENSOR_Sleep(void); 
void RAIN_SENSOR_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RAIN_SENSOR__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RAIN_SENSOR_DRIVE_MODE_BITS        (3)
    #define RAIN_SENSOR_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RAIN_SENSOR_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RAIN_SENSOR_SetDriveMode() function.
         *  @{
         */
        #define RAIN_SENSOR_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RAIN_SENSOR_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RAIN_SENSOR_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RAIN_SENSOR_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RAIN_SENSOR_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RAIN_SENSOR_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RAIN_SENSOR_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RAIN_SENSOR_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RAIN_SENSOR_MASK               RAIN_SENSOR__MASK
#define RAIN_SENSOR_SHIFT              RAIN_SENSOR__SHIFT
#define RAIN_SENSOR_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RAIN_SENSOR_SetInterruptMode() function.
     *  @{
     */
        #define RAIN_SENSOR_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RAIN_SENSOR_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RAIN_SENSOR_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RAIN_SENSOR_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RAIN_SENSOR__SIO)
    #define RAIN_SENSOR_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RAIN_SENSOR__PC) && (CY_PSOC4_4200L)
    #define RAIN_SENSOR_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RAIN_SENSOR_USBIO_DISABLE              ((uint32)(~RAIN_SENSOR_USBIO_ENABLE))
    #define RAIN_SENSOR_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RAIN_SENSOR_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RAIN_SENSOR_USBIO_ENTER_SLEEP          ((uint32)((1u << RAIN_SENSOR_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RAIN_SENSOR_USBIO_SUSPEND_DEL_SHIFT)))
    #define RAIN_SENSOR_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RAIN_SENSOR_USBIO_SUSPEND_SHIFT)))
    #define RAIN_SENSOR_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RAIN_SENSOR_USBIO_SUSPEND_DEL_SHIFT)))
    #define RAIN_SENSOR_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RAIN_SENSOR__PC)
    /* Port Configuration */
    #define RAIN_SENSOR_PC                 (* (reg32 *) RAIN_SENSOR__PC)
#endif
/* Pin State */
#define RAIN_SENSOR_PS                     (* (reg32 *) RAIN_SENSOR__PS)
/* Data Register */
#define RAIN_SENSOR_DR                     (* (reg32 *) RAIN_SENSOR__DR)
/* Input Buffer Disable Override */
#define RAIN_SENSOR_INP_DIS                (* (reg32 *) RAIN_SENSOR__PC2)

/* Interrupt configuration Registers */
#define RAIN_SENSOR_INTCFG                 (* (reg32 *) RAIN_SENSOR__INTCFG)
#define RAIN_SENSOR_INTSTAT                (* (reg32 *) RAIN_SENSOR__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RAIN_SENSOR_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RAIN_SENSOR__SIO)
    #define RAIN_SENSOR_SIO_REG            (* (reg32 *) RAIN_SENSOR__SIO)
#endif /* (RAIN_SENSOR__SIO_CFG) */

/* USBIO registers */
#if !defined(RAIN_SENSOR__PC) && (CY_PSOC4_4200L)
    #define RAIN_SENSOR_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RAIN_SENSOR_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RAIN_SENSOR_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RAIN_SENSOR_DRIVE_MODE_SHIFT       (0x00u)
#define RAIN_SENSOR_DRIVE_MODE_MASK        (0x07u << RAIN_SENSOR_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RAIN_SENSOR_H */


/* [] END OF FILE */
