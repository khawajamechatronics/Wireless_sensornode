/***************************************************************************//**
* \file SPI_BME280_SPI_UART_PVT.h
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

#if !defined(CY_SCB_SPI_UART_PVT_SPI_BME280_H)
#define CY_SCB_SPI_UART_PVT_SPI_BME280_H

#include "SPI_BME280_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (SPI_BME280_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_BME280_rxBufferHead;
    extern volatile uint32  SPI_BME280_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   SPI_BME280_rxBufferOverflow;
    /** @} globals */
#endif /* (SPI_BME280_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SPI_BME280_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_BME280_txBufferHead;
    extern volatile uint32  SPI_BME280_txBufferTail;
#endif /* (SPI_BME280_INTERNAL_TX_SW_BUFFER_CONST) */

#if (SPI_BME280_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SPI_BME280_rxBufferInternal[SPI_BME280_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SPI_BME280_INTERNAL_RX_SW_BUFFER) */

#if (SPI_BME280_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SPI_BME280_txBufferInternal[SPI_BME280_TX_BUFFER_SIZE];
#endif /* (SPI_BME280_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void SPI_BME280_SpiPostEnable(void);
void SPI_BME280_SpiStop(void);

#if (SPI_BME280_SCB_MODE_SPI_CONST_CFG)
    void SPI_BME280_SpiInit(void);
#endif /* (SPI_BME280_SCB_MODE_SPI_CONST_CFG) */

#if (SPI_BME280_SPI_WAKE_ENABLE_CONST)
    void SPI_BME280_SpiSaveConfig(void);
    void SPI_BME280_SpiRestoreConfig(void);
#endif /* (SPI_BME280_SPI_WAKE_ENABLE_CONST) */

void SPI_BME280_UartPostEnable(void);
void SPI_BME280_UartStop(void);

#if (SPI_BME280_SCB_MODE_UART_CONST_CFG)
    void SPI_BME280_UartInit(void);
#endif /* (SPI_BME280_SCB_MODE_UART_CONST_CFG) */

#if (SPI_BME280_UART_WAKE_ENABLE_CONST)
    void SPI_BME280_UartSaveConfig(void);
    void SPI_BME280_UartRestoreConfig(void);
#endif /* (SPI_BME280_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in SPI_BME280_SetPins() */
#define SPI_BME280_UART_RX_PIN_ENABLE    (SPI_BME280_UART_RX)
#define SPI_BME280_UART_TX_PIN_ENABLE    (SPI_BME280_UART_TX)

/* UART RTS and CTS position to be used in  SPI_BME280_SetPins() */
#define SPI_BME280_UART_RTS_PIN_ENABLE    (0x10u)
#define SPI_BME280_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define SPI_BME280_SpiUartEnableIntRx(intSourceMask)  SPI_BME280_SetRxInterruptMode(intSourceMask)
#define SPI_BME280_SpiUartEnableIntTx(intSourceMask)  SPI_BME280_SetTxInterruptMode(intSourceMask)
uint32  SPI_BME280_SpiUartDisableIntRx(void);
uint32  SPI_BME280_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_SPI_BME280_H) */


/* [] END OF FILE */
