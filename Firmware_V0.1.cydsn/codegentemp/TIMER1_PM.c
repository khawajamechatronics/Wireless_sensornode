/*******************************************************************************
* File Name: TIMER1_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TIMER1.h"

static TIMER1_BACKUP_STRUCT TIMER1_backup;


/*******************************************************************************
* Function Name: TIMER1_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TIMER1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: TIMER1_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TIMER1_Sleep(void)
{
    if(0u != (TIMER1_BLOCK_CONTROL_REG & TIMER1_MASK))
    {
        TIMER1_backup.enableState = 1u;
    }
    else
    {
        TIMER1_backup.enableState = 0u;
    }

    TIMER1_Stop();
    TIMER1_SaveConfig();
}


/*******************************************************************************
* Function Name: TIMER1_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TIMER1_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: TIMER1_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TIMER1_Wakeup(void)
{
    TIMER1_RestoreConfig();

    if(0u != TIMER1_backup.enableState)
    {
        TIMER1_Enable();
    }
}


/* [] END OF FILE */
