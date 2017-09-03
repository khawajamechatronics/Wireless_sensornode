/*******************************************************************************
* File Name: ADC_SAR_PM.c
* Version 2.40
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SAR.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_BACKUP_STRUCT  ADC_SAR_backup =
{
    ADC_SAR_DISABLED
};


/*******************************************************************************
* Function Name: ADC_SAR_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_backup - modified.
*
*******************************************************************************/
void ADC_SAR_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    ADC_SAR_SAR_DFT_CTRL_REG |= ADC_SAR_ADFT_OVERRIDE;
    if((ADC_SAR_SAR_CTRL_REG  & ADC_SAR_ENABLE) != 0u)
    {
        if((ADC_SAR_SAR_SAMPLE_CTRL_REG & ADC_SAR_CONTINUOUS_EN) != 0u)
        {
            ADC_SAR_backup.enableState = ADC_SAR_ENABLED | ADC_SAR_STARTED;
        }
        else
        {
            ADC_SAR_backup.enableState = ADC_SAR_ENABLED;
        }
        ADC_SAR_StopConvert();
        ADC_SAR_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((ADC_SAR_SAR_CTRL_REG & ADC_SAR_BOOSTPUMP_EN) != 0u)
        {
            ADC_SAR_SAR_CTRL_REG &= (uint32)~ADC_SAR_BOOSTPUMP_EN;
            ADC_SAR_backup.enableState |= ADC_SAR_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        ADC_SAR_backup.enableState = ADC_SAR_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_SAR_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_backup - used.
*
*******************************************************************************/
void ADC_SAR_Wakeup(void)
{
    ADC_SAR_SAR_DFT_CTRL_REG &= (uint32)~ADC_SAR_ADFT_OVERRIDE;
    if(ADC_SAR_backup.enableState != ADC_SAR_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((ADC_SAR_backup.enableState & ADC_SAR_BOOSTPUMP_ENABLED) != 0u)
        {
            ADC_SAR_SAR_CTRL_REG |= ADC_SAR_BOOSTPUMP_EN;
        }
        ADC_SAR_Enable();
        if((ADC_SAR_backup.enableState & ADC_SAR_STARTED) != 0u)
        {
            ADC_SAR_StartConvert();
        }
    }
}
/* [] END OF FILE */
