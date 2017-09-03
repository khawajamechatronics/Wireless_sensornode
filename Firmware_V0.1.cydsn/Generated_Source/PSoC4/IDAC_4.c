/*******************************************************************************
* File Name: IDAC_4.c
* Version 1.10
*
* Description:
*  This file provides the source code of APIs for the IDAC_P4 component.
*
*******************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "IDAC_4.h"

uint32 IDAC_4_initVar = 0u;


/*******************************************************************************
* Function Name: IDAC_4_Init
********************************************************************************
*
* Summary:
*  Initializes IDAC registers with initial values provided from customizer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC_4_Init(void)
{
    uint32 regVal;
    uint8 enableInterrupts;

    /* Set initial configuration */
    enableInterrupts = CyEnterCriticalSection();
    
    #if(IDAC_4_MODE_SOURCE == IDAC_4_IDAC_POLARITY)
        regVal  = IDAC_4_CSD_TRIM1_REG & ~(IDAC_4_CSD_IDAC_TRIM1_MASK);
        regVal |=  (IDAC_4_SFLASH_TRIM1_REG & IDAC_4_CSD_IDAC_TRIM1_MASK);
        IDAC_4_CSD_TRIM1_REG = regVal;
    #else
        regVal  = IDAC_4_CSD_TRIM2_REG & ~(IDAC_4_CSD_IDAC_TRIM2_MASK);
        regVal |=  (IDAC_4_SFLASH_TRIM2_REG & IDAC_4_CSD_IDAC_TRIM2_MASK);
        IDAC_4_CSD_TRIM2_REG = regVal;
    #endif /* (IDAC_4_MODE_SOURCE == IDAC_4_IDAC_POLARITY) */

    /* clear previous values */
    IDAC_4_IDAC_CONTROL_REG &= ((uint32)~((uint32)IDAC_4_IDAC_VALUE_MASK <<
        IDAC_4_IDAC_VALUE_POSITION)) | ((uint32)~((uint32)IDAC_4_IDAC_MODE_MASK <<
        IDAC_4_IDAC_MODE_POSITION))  | ((uint32)~((uint32)IDAC_4_IDAC_RANGE_MASK  <<
        IDAC_4_IDAC_RANGE_POSITION));

    IDAC_4_IDAC_POLARITY_CONTROL_REG &= (~(uint32)((uint32)IDAC_4_IDAC_POLARITY_MASK <<
        IDAC_4_IDAC_POLARITY_POSITION));

    /* set new configuration */
    IDAC_4_IDAC_CONTROL_REG |= (((uint32)IDAC_4_IDAC_INIT_VALUE <<
        IDAC_4_IDAC_VALUE_POSITION) | ((uint32)IDAC_4_IDAC_RANGE <<
        IDAC_4_IDAC_RANGE_POSITION));

    IDAC_4_IDAC_POLARITY_CONTROL_REG |= ((uint32)IDAC_4_IDAC_POLARITY <<
                                                           IDAC_4_IDAC_POLARITY_POSITION);

    CyExitCriticalSection(enableInterrupts);

}


/*******************************************************************************
* Function Name: IDAC_4_Enable
********************************************************************************
*
* Summary:
*  Enables IDAC operations.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC_4_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    /* Enable the IDAC */
    IDAC_4_IDAC_CONTROL_REG |= ((uint32)IDAC_4_IDAC_EN_MODE <<
                                                  IDAC_4_IDAC_MODE_POSITION);
    IDAC_4_IDAC_POLARITY_CONTROL_REG |= ((uint32)IDAC_4_IDAC_CSD_EN <<
                                                           IDAC_4_IDAC_CSD_EN_POSITION);
    CyExitCriticalSection(enableInterrupts);

}


/*******************************************************************************
* Function Name: IDAC_4_Start
********************************************************************************
*
* Summary:
*  Starts the IDAC hardware.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  IDAC_4_initVar
*
*******************************************************************************/
void IDAC_4_Start(void)
{
    if(0u == IDAC_4_initVar)
    {
        IDAC_4_Init();
        IDAC_4_initVar = 1u;
    }

    IDAC_4_Enable();

}


/*******************************************************************************
* Function Name: IDAC_4_Stop
********************************************************************************
*
* Summary:
*  Stops the IDAC hardware.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC_4_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    /* Disable the IDAC */
    IDAC_4_IDAC_CONTROL_REG &= ((uint32)~((uint32)IDAC_4_IDAC_MODE_MASK <<
        IDAC_4_IDAC_MODE_POSITION));
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: IDAC_4_SetValue
********************************************************************************
*
* Summary:
*  Sets the IDAC value.
*
* Parameters:
*  uint32 value
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC_4_SetValue(uint32 value)
{
    uint8 enableInterrupts;
    uint32 newRegisterValue;

    enableInterrupts = CyEnterCriticalSection();

    #if(IDAC_4_IDAC_VALUE_POSITION != 0u)
        newRegisterValue = ((IDAC_4_IDAC_CONTROL_REG & (~(uint32)((uint32)IDAC_4_IDAC_VALUE_MASK <<
        IDAC_4_IDAC_VALUE_POSITION))) | (value << IDAC_4_IDAC_VALUE_POSITION));
    #else
        newRegisterValue = ((IDAC_4_IDAC_CONTROL_REG & (~(uint32)IDAC_4_IDAC_VALUE_MASK)) | value);
    #endif /* IDAC_4_IDAC_VALUE_POSITION != 0u */

    IDAC_4_IDAC_CONTROL_REG = newRegisterValue;

    CyExitCriticalSection(enableInterrupts);
}

/* [] END OF FILE */
