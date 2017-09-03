/*******************************************************************************
* File Name: SPI_BME280_o_ss_s.h  
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

#if !defined(CY_PINS_SPI_BME280_o_ss_s_H) /* Pins SPI_BME280_o_ss_s_H */
#define CY_PINS_SPI_BME280_o_ss_s_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SPI_BME280_o_ss_s_aliases.h"


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
} SPI_BME280_o_ss_s_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SPI_BME280_o_ss_s_Read(void);
void    SPI_BME280_o_ss_s_Write(uint8 value);
uint8   SPI_BME280_o_ss_s_ReadDataReg(void);
#if defined(SPI_BME280_o_ss_s__PC) || (CY_PSOC4_4200L) 
    void    SPI_BME280_o_ss_s_SetDriveMode(uint8 mode);
#endif
void    SPI_BME280_o_ss_s_SetInterruptMode(uint16 position, uint16 mode);
uint8   SPI_BME280_o_ss_s_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SPI_BME280_o_ss_s_Sleep(void); 
void SPI_BME280_o_ss_s_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SPI_BME280_o_ss_s__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SPI_BME280_o_ss_s_DRIVE_MODE_BITS        (3)
    #define SPI_BME280_o_ss_s_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SPI_BME280_o_ss_s_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SPI_BME280_o_ss_s_SetDriveMode() function.
         *  @{
         */
        #define SPI_BME280_o_ss_s_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SPI_BME280_o_ss_s_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SPI_BME280_o_ss_s_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SPI_BME280_o_ss_s_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SPI_BME280_o_ss_s_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SPI_BME280_o_ss_s_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SPI_BME280_o_ss_s_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SPI_BME280_o_ss_s_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SPI_BME280_o_ss_s_MASK               SPI_BME280_o_ss_s__MASK
#define SPI_BME280_o_ss_s_SHIFT              SPI_BME280_o_ss_s__SHIFT
#define SPI_BME280_o_ss_s_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SPI_BME280_o_ss_s_SetInterruptMode() function.
     *  @{
     */
        #define SPI_BME280_o_ss_s_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SPI_BME280_o_ss_s_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SPI_BME280_o_ss_s_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SPI_BME280_o_ss_s_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SPI_BME280_o_ss_s__SIO)
    #define SPI_BME280_o_ss_s_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SPI_BME280_o_ss_s__PC) && (CY_PSOC4_4200L)
    #define SPI_BME280_o_ss_s_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SPI_BME280_o_ss_s_USBIO_DISABLE              ((uint32)(~SPI_BME280_o_ss_s_USBIO_ENABLE))
    #define SPI_BME280_o_ss_s_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SPI_BME280_o_ss_s_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SPI_BME280_o_ss_s_USBIO_ENTER_SLEEP          ((uint32)((1u << SPI_BME280_o_ss_s_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SPI_BME280_o_ss_s_USBIO_SUSPEND_DEL_SHIFT)))
    #define SPI_BME280_o_ss_s_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SPI_BME280_o_ss_s_USBIO_SUSPEND_SHIFT)))
    #define SPI_BME280_o_ss_s_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SPI_BME280_o_ss_s_USBIO_SUSPEND_DEL_SHIFT)))
    #define SPI_BME280_o_ss_s_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SPI_BME280_o_ss_s__PC)
    /* Port Configuration */
    #define SPI_BME280_o_ss_s_PC                 (* (reg32 *) SPI_BME280_o_ss_s__PC)
#endif
/* Pin State */
#define SPI_BME280_o_ss_s_PS                     (* (reg32 *) SPI_BME280_o_ss_s__PS)
/* Data Register */
#define SPI_BME280_o_ss_s_DR                     (* (reg32 *) SPI_BME280_o_ss_s__DR)
/* Input Buffer Disable Override */
#define SPI_BME280_o_ss_s_INP_DIS                (* (reg32 *) SPI_BME280_o_ss_s__PC2)

/* Interrupt configuration Registers */
#define SPI_BME280_o_ss_s_INTCFG                 (* (reg32 *) SPI_BME280_o_ss_s__INTCFG)
#define SPI_BME280_o_ss_s_INTSTAT                (* (reg32 *) SPI_BME280_o_ss_s__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SPI_BME280_o_ss_s_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SPI_BME280_o_ss_s__SIO)
    #define SPI_BME280_o_ss_s_SIO_REG            (* (reg32 *) SPI_BME280_o_ss_s__SIO)
#endif /* (SPI_BME280_o_ss_s__SIO_CFG) */

/* USBIO registers */
#if !defined(SPI_BME280_o_ss_s__PC) && (CY_PSOC4_4200L)
    #define SPI_BME280_o_ss_s_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SPI_BME280_o_ss_s_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SPI_BME280_o_ss_s_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SPI_BME280_o_ss_s_DRIVE_MODE_SHIFT       (0x00u)
#define SPI_BME280_o_ss_s_DRIVE_MODE_MASK        (0x07u << SPI_BME280_o_ss_s_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SPI_BME280_o_ss_s_H */


/* [] END OF FILE */
