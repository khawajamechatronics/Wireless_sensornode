/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "weathersensors.h"

//Local functions
uint16  wind_vane_lut[8] = {218,372,657,1647,2941,2600,2232,1112};
winddirection olddirection= north;
void lookupWindVane(winddirection *p_angle, uint16 ain)
{
    uint16  diff;
    int i=0;
    for( i=0; i<8; i+=1 )
    {
        diff = abs( wind_vane_lut[i] - ain );
        if( diff <= 18 )
        {
            *p_angle =  i;
            return;
        }
    }
    *p_angle = invalid;
    return;
}

/* Variable to store the number of times the Counter has overflown */
uint32 counter_overflowCount = 0u;

/* Variable to store the count value that is captured at the instance of the falling edge of the
 * capture signal */
uint16 capturedCount = 0u;

/* Variable to indicate the occurrence of the Capture Interrupt. The Counter generates interrupt on the
 * Terminal count and / or on the Capture event. The flag is set inside the Capture interrupt and cleared in the
 * main.c after processing */
uint8 CC_ISR_flag = FALSE;

/*******************************************************************************
* Function Name: Counter_ISR
********************************************************************************
* Summary:
*  ISR For Counter Interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR_PROTO(Counter_ISR)
{
    /* Variable to store the Counter Status register. This value is used to determine the event which caused the
     * Interrupt (either Terminal Count or Capture). */
    uint32 counter_status = 0u;

    /* Read the Counter Status register. The status register stores the info about type of interrupt. */
    counter_status = Counter_GetInterruptSource();

    /* Check if the Counter interrupt is caused by Terminal Count. This can be checked in the 
     * status register of the Counter. The Counter status register is read and the value is stored
     * in counter_status.
     * The defines Counter_INTR_MASK_TC and Counter_INTR_MASK_CC_MATCH can be seen in Counter.h file.
     * These defines indicate the type of interrupt*/
    if ((counter_status & Counter_INTR_MASK_TC) == Counter_INTR_MASK_TC)
    {
        /* This checks if the interrupt is due to Terminal Count */
        /* Increment the counter_overflowCount. This variable keeps track of the number of times the
         * counter had overflown before the capture signal occurred. This count is used to calculate the
         * time when the capture occurs */
        counter_overflowCount++;

        /*Clear Interrupt*/
        Counter_ClearInterrupt(counter_status & Counter_INTR_MASK_TC);        
    }    

    /* Check if the Counter interrupt is caused by Capture Event */
    if ((counter_status & Counter_INTR_MASK_CC_MATCH) == Counter_INTR_MASK_CC_MATCH)
    {
        /* Read the value captured by the Counter on the falling edge of the capture signal */
        capturedCount = Counter_ReadCapture();    
        Counter_WriteCounter(0);

        /* Set the ISR_flag to TRUE to indicate the occurrence of CC Interrupt */
        CC_ISR_flag = TRUE;  

        /*Clear the Interrupt*/
        Counter_ClearInterrupt(counter_status & Counter_INTR_MASK_CC_MATCH);
    }
}

//Public functions
void WEATHER_Setup()
{
    Counter_Start();
    TIMER0_ISR_StartEx(Counter_ISR);
    CC_ISR_flag = FALSE;
}


winddirection   WEATHER_getWindDirection()
{
    int16 vane_adc = 0;
    int16 resultmv=0;
    winddirection direction;
    
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    /******************************************************************************
    * Adjust data from ADC with respect to Vref value.
    * This adjustment is to be done if Vref is set to any other than
    * internal 1.024V.
    * For more detailed description see Functional Description section
    * of DieTemp P4 datasheet.
    *******************************************************************************/
    vane_adc = ADC_SAR_GetResult16(0);
    resultmv = ADC_SAR_CountsTo_mVolts(0, vane_adc);
    lookupWindVane(&direction, resultmv);
    if(direction == invalid)
        direction = olddirection;
    
    return direction;
}

uint8_t WEATHER_getWindSpeed()
{
    uint8_t windspeed;
    windspeed = 2400/capturedCount;
    char8 outstring[100];
    sprintf(outstring, "%d\r\n", windspeed);
    UART_UartPutString(outstring);
    
    return 0;
}

/* [] END OF FILE */
