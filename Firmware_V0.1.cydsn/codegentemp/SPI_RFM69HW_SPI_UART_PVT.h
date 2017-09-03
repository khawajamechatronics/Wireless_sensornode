/***************************************************************************//**
* \file SPI_RFM69HW_SPI_UART_PVT.h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_SPI_RFM69HW_H)
#define CY_SCB_SPI_UART_PVT_SPI_RFM69HW_H

#include "SPI_RFM69HW_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (SPI_RFM69HW_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_RFM69HW_rxBufferHead;
    extern volatile uint32  SPI_RFM69HW_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   SPI_RFM69HW_rxBufferOverflow;
    /** @} globals */
#endif /* (SPI_RFM69HW_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SPI_RFM69HW_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_RFM69HW_txBufferHead;
    extern volatile uint32  SPI_RFM69HW_txBufferTail;
#endif /* (SPI_RFM69HW_INTERNAL_TX_SW_BUFFER_CONST) */

#if (SPI_RFM69HW_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SPI_RFM69HW_rxBufferInternal[SPI_RFM69HW_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SPI_RFM69HW_INTERNAL_RX_SW_BUFFER) */

#if (SPI_RFM69HW_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SPI_RFM69HW_txBufferInternal[SPI_RFM69HW_TX_BUFFER_SIZE];
#endif /* (SPI_RFM69HW_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void SPI_RFM69HW_SpiPostEnable(void);
void SPI_RFM69HW_SpiStop(void);

#if (SPI_RFM69HW_SCB_MODE_SPI_CONST_CFG)
    void SPI_RFM69HW_SpiInit(void);
#endif /* (SPI_RFM69HW_SCB_MODE_SPI_CONST_CFG) */

#if (SPI_RFM69HW_SPI_WAKE_ENABLE_CONST)
    void SPI_RFM69HW_SpiSaveConfig(void);
    void SPI_RFM69HW_SpiRestoreConfig(void);
#endif /* (SPI_RFM69HW_SPI_WAKE_ENABLE_CONST) */

void SPI_RFM69HW_UartPostEnable(void);
void SPI_RFM69HW_UartStop(void);

#if (SPI_RFM69HW_SCB_MODE_UART_CONST_CFG)
    void SPI_RFM69HW_UartInit(void);
#endif /* (SPI_RFM69HW_SCB_MODE_UART_CONST_CFG) */

#if (SPI_RFM69HW_UART_WAKE_ENABLE_CONST)
    void SPI_RFM69HW_UartSaveConfig(void);
    void SPI_RFM69HW_UartRestoreConfig(void);
#endif /* (SPI_RFM69HW_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in SPI_RFM69HW_SetPins() */
#define SPI_RFM69HW_UART_RX_PIN_ENABLE    (SPI_RFM69HW_UART_RX)
#define SPI_RFM69HW_UART_TX_PIN_ENABLE    (SPI_RFM69HW_UART_TX)

/* UART RTS and CTS position to be used in  SPI_RFM69HW_SetPins() */
#define SPI_RFM69HW_UART_RTS_PIN_ENABLE    (0x10u)
#define SPI_RFM69HW_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define SPI_RFM69HW_SpiUartEnableIntRx(intSourceMask)  SPI_RFM69HW_SetRxInterruptMode(intSourceMask)
#define SPI_RFM69HW_SpiUartEnableIntTx(intSourceMask)  SPI_RFM69HW_SetTxInterruptMode(intSourceMask)
uint32  SPI_RFM69HW_SpiUartDisableIntRx(void);
uint32  SPI_RFM69HW_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_SPI_RFM69HW_H) */


/* [] END OF FILE */
