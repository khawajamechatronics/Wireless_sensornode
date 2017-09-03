/*******************************************************************************
* File Name: TEST_MOSI.h  
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

#if !defined(CY_PINS_TEST_MOSI_H) /* Pins TEST_MOSI_H */
#define CY_PINS_TEST_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "TEST_MOSI_aliases.h"


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
} TEST_MOSI_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   TEST_MOSI_Read(void);
void    TEST_MOSI_Write(uint8 value);
uint8   TEST_MOSI_ReadDataReg(void);
#if defined(TEST_MOSI__PC) || (CY_PSOC4_4200L) 
    void    TEST_MOSI_SetDriveMode(uint8 mode);
#endif
void    TEST_MOSI_SetInterruptMode(uint16 position, uint16 mode);
uint8   TEST_MOSI_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void TEST_MOSI_Sleep(void); 
void TEST_MOSI_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(TEST_MOSI__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define TEST_MOSI_DRIVE_MODE_BITS        (3)
    #define TEST_MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - TEST_MOSI_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the TEST_MOSI_SetDriveMode() function.
         *  @{
         */
        #define TEST_MOSI_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define TEST_MOSI_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define TEST_MOSI_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define TEST_MOSI_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define TEST_MOSI_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define TEST_MOSI_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define TEST_MOSI_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define TEST_MOSI_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define TEST_MOSI_MASK               TEST_MOSI__MASK
#define TEST_MOSI_SHIFT              TEST_MOSI__SHIFT
#define TEST_MOSI_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TEST_MOSI_SetInterruptMode() function.
     *  @{
     */
        #define TEST_MOSI_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define TEST_MOSI_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define TEST_MOSI_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define TEST_MOSI_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(TEST_MOSI__SIO)
    #define TEST_MOSI_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(TEST_MOSI__PC) && (CY_PSOC4_4200L)
    #define TEST_MOSI_USBIO_ENABLE               ((uint32)0x80000000u)
    #define TEST_MOSI_USBIO_DISABLE              ((uint32)(~TEST_MOSI_USBIO_ENABLE))
    #define TEST_MOSI_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define TEST_MOSI_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define TEST_MOSI_USBIO_ENTER_SLEEP          ((uint32)((1u << TEST_MOSI_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << TEST_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define TEST_MOSI_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << TEST_MOSI_USBIO_SUSPEND_SHIFT)))
    #define TEST_MOSI_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << TEST_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define TEST_MOSI_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(TEST_MOSI__PC)
    /* Port Configuration */
    #define TEST_MOSI_PC                 (* (reg32 *) TEST_MOSI__PC)
#endif
/* Pin State */
#define TEST_MOSI_PS                     (* (reg32 *) TEST_MOSI__PS)
/* Data Register */
#define TEST_MOSI_DR                     (* (reg32 *) TEST_MOSI__DR)
/* Input Buffer Disable Override */
#define TEST_MOSI_INP_DIS                (* (reg32 *) TEST_MOSI__PC2)

/* Interrupt configuration Registers */
#define TEST_MOSI_INTCFG                 (* (reg32 *) TEST_MOSI__INTCFG)
#define TEST_MOSI_INTSTAT                (* (reg32 *) TEST_MOSI__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define TEST_MOSI_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(TEST_MOSI__SIO)
    #define TEST_MOSI_SIO_REG            (* (reg32 *) TEST_MOSI__SIO)
#endif /* (TEST_MOSI__SIO_CFG) */

/* USBIO registers */
#if !defined(TEST_MOSI__PC) && (CY_PSOC4_4200L)
    #define TEST_MOSI_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define TEST_MOSI_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define TEST_MOSI_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define TEST_MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define TEST_MOSI_DRIVE_MODE_MASK        (0x07u << TEST_MOSI_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins TEST_MOSI_H */


/* [] END OF FILE */
