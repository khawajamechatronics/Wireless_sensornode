/*******************************************************************************
* File Name: Counter.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Counter
*  component
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

#include "Counter.h"

uint8 Counter_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Counter configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Counter__QUAD == Counter_CONFIG)
        Counter_CONTROL_REG = Counter_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Counter_TRIG_CONTROL1_REG  = Counter_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Counter_SetInterruptMode(Counter_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Counter_SetCounterMode(Counter_COUNT_DOWN);
        Counter_WritePeriod(Counter_QUAD_PERIOD_INIT_VALUE);
        Counter_WriteCounter(Counter_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Counter__QUAD == Counter_CONFIG) */

    #if (Counter__TIMER == Counter_CONFIG)
        Counter_CONTROL_REG = Counter_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Counter_TRIG_CONTROL1_REG  = Counter_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Counter_SetInterruptMode(Counter_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Counter_WritePeriod(Counter_TC_PERIOD_VALUE );

        #if (Counter__COMPARE == Counter_TC_COMP_CAP_MODE)
            Counter_WriteCompare(Counter_TC_COMPARE_VALUE);

            #if (1u == Counter_TC_COMPARE_SWAP)
                Counter_SetCompareSwap(1u);
                Counter_WriteCompareBuf(Counter_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Counter_TC_COMPARE_SWAP) */
        #endif  /* (Counter__COMPARE == Counter_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Counter_CY_TCPWM_V2 && Counter_TIMER_UPDOWN_CNT_USED && !Counter_CY_TCPWM_4000)
            Counter_WriteCounter(1u);
        #elif(Counter__COUNT_DOWN == Counter_TC_COUNTER_MODE)
            Counter_WriteCounter(Counter_TC_PERIOD_VALUE);
        #else
            Counter_WriteCounter(0u);
        #endif /* (Counter_CY_TCPWM_V2 && Counter_TIMER_UPDOWN_CNT_USED && !Counter_CY_TCPWM_4000) */
    #endif  /* (Counter__TIMER == Counter_CONFIG) */

    #if (Counter__PWM_SEL == Counter_CONFIG)
        Counter_CONTROL_REG = Counter_CTRL_PWM_BASE_CONFIG;

        #if (Counter__PWM_PR == Counter_PWM_MODE)
            Counter_CONTROL_REG |= Counter_CTRL_PWM_RUN_MODE;
            Counter_WriteCounter(Counter_PWM_PR_INIT_VALUE);
        #else
            Counter_CONTROL_REG |= Counter_CTRL_PWM_ALIGN | Counter_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Counter_CY_TCPWM_V2 && Counter_PWM_UPDOWN_CNT_USED && !Counter_CY_TCPWM_4000)
                Counter_WriteCounter(1u);
            #elif (Counter__RIGHT == Counter_PWM_ALIGN)
                Counter_WriteCounter(Counter_PWM_PERIOD_VALUE);
            #else 
                Counter_WriteCounter(0u);
            #endif  /* (Counter_CY_TCPWM_V2 && Counter_PWM_UPDOWN_CNT_USED && !Counter_CY_TCPWM_4000) */
        #endif  /* (Counter__PWM_PR == Counter_PWM_MODE) */

        #if (Counter__PWM_DT == Counter_PWM_MODE)
            Counter_CONTROL_REG |= Counter_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Counter__PWM_DT == Counter_PWM_MODE) */

        #if (Counter__PWM == Counter_PWM_MODE)
            Counter_CONTROL_REG |= Counter_CTRL_PWM_PRESCALER;
        #endif  /* (Counter__PWM == Counter_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Counter_TRIG_CONTROL1_REG  = Counter_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Counter_SetInterruptMode(Counter_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Counter__PWM_PR == Counter_PWM_MODE)
            Counter_TRIG_CONTROL2_REG =
                    (Counter_CC_MATCH_NO_CHANGE    |
                    Counter_OVERLOW_NO_CHANGE      |
                    Counter_UNDERFLOW_NO_CHANGE);
        #else
            #if (Counter__LEFT == Counter_PWM_ALIGN)
                Counter_TRIG_CONTROL2_REG = Counter_PWM_MODE_LEFT;
            #endif  /* ( Counter_PWM_LEFT == Counter_PWM_ALIGN) */

            #if (Counter__RIGHT == Counter_PWM_ALIGN)
                Counter_TRIG_CONTROL2_REG = Counter_PWM_MODE_RIGHT;
            #endif  /* ( Counter_PWM_RIGHT == Counter_PWM_ALIGN) */

            #if (Counter__CENTER == Counter_PWM_ALIGN)
                Counter_TRIG_CONTROL2_REG = Counter_PWM_MODE_CENTER;
            #endif  /* ( Counter_PWM_CENTER == Counter_PWM_ALIGN) */

            #if (Counter__ASYMMETRIC == Counter_PWM_ALIGN)
                Counter_TRIG_CONTROL2_REG = Counter_PWM_MODE_ASYM;
            #endif  /* (Counter__ASYMMETRIC == Counter_PWM_ALIGN) */
        #endif  /* (Counter__PWM_PR == Counter_PWM_MODE) */

        /* Set other values from customizer */
        Counter_WritePeriod(Counter_PWM_PERIOD_VALUE );
        Counter_WriteCompare(Counter_PWM_COMPARE_VALUE);

        #if (1u == Counter_PWM_COMPARE_SWAP)
            Counter_SetCompareSwap(1u);
            Counter_WriteCompareBuf(Counter_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Counter_PWM_COMPARE_SWAP) */

        #if (1u == Counter_PWM_PERIOD_SWAP)
            Counter_SetPeriodSwap(1u);
            Counter_WritePeriodBuf(Counter_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Counter_PWM_PERIOD_SWAP) */
    #endif  /* (Counter__PWM_SEL == Counter_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Counter_Enable
********************************************************************************
*
* Summary:
*  Enables the Counter.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Counter_BLOCK_CONTROL_REG |= Counter_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Counter__PWM_SEL == Counter_CONFIG)
        #if (0u == Counter_PWM_START_SIGNAL_PRESENT)
            Counter_TriggerCommand(Counter_MASK, Counter_CMD_START);
        #endif /* (0u == Counter_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Counter__PWM_SEL == Counter_CONFIG) */

    #if (Counter__TIMER == Counter_CONFIG)
        #if (0u == Counter_TC_START_SIGNAL_PRESENT)
            Counter_TriggerCommand(Counter_MASK, Counter_CMD_START);
        #endif /* (0u == Counter_TC_START_SIGNAL_PRESENT) */
    #endif /* (Counter__TIMER == Counter_CONFIG) */
    
    #if (Counter__QUAD == Counter_CONFIG)
        #if (0u != Counter_QUAD_AUTO_START)
            Counter_TriggerCommand(Counter_MASK, Counter_CMD_RELOAD);
        #endif /* (0u != Counter_QUAD_AUTO_START) */
    #endif  /* (Counter__QUAD == Counter_CONFIG) */
}


/*******************************************************************************
* Function Name: Counter_Start
********************************************************************************
*
* Summary:
*  Initializes the Counter with default customizer
*  values when called the first time and enables the Counter.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Counter_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Counter_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Counter_Start() routine.
*
*******************************************************************************/
void Counter_Start(void)
{
    if (0u == Counter_initVar)
    {
        Counter_Init();
        Counter_initVar = 1u;
    }

    Counter_Enable();
}


/*******************************************************************************
* Function Name: Counter_Stop
********************************************************************************
*
* Summary:
*  Disables the Counter.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_BLOCK_CONTROL_REG &= (uint32)~Counter_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Counter. This function is used when
*  configured as a generic Counter and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Counter to operate in
*   Values:
*   - Counter_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Counter_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Counter_MODE_QUAD - Quadrature decoder
*         - Counter_MODE_PWM - PWM
*         - Counter_MODE_PWM_DT - PWM with dead time
*         - Counter_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_MODE_MASK;
    Counter_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Counter_MODE_X1 - Counts on phi 1 rising
*         - Counter_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Counter_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_QUAD_MODE_MASK;
    Counter_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Counter_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Counter_PRESCALE_DIVBY2    - Divide by 2
*         - Counter_PRESCALE_DIVBY4    - Divide by 4
*         - Counter_PRESCALE_DIVBY8    - Divide by 8
*         - Counter_PRESCALE_DIVBY16   - Divide by 16
*         - Counter_PRESCALE_DIVBY32   - Divide by 32
*         - Counter_PRESCALE_DIVBY64   - Divide by 64
*         - Counter_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_PRESCALER_MASK;
    Counter_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Counter runs
*  continuously or stops when terminal count is reached.  By default the
*  Counter operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_ONESHOT_MASK;
    Counter_CONTROL_REG |= ((uint32)((oneShotEnable & Counter_1BIT_MASK) <<
                                                               Counter_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPWMMode(uint32 modeMask)
{
    Counter_TRIG_CONTROL2_REG = (modeMask & Counter_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Counter_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_PWM_SYNC_KILL_MASK;
    Counter_CONTROL_REG |= ((uint32)((syncKillEnable & Counter_1BIT_MASK)  <<
                                               Counter_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_PWM_STOP_KILL_MASK;
    Counter_CONTROL_REG |= ((uint32)((stopOnKillEnable & Counter_1BIT_MASK)  <<
                                                         Counter_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_PRESCALER_MASK;
    Counter_CONTROL_REG |= ((uint32)((deadTime & Counter_8BIT_MASK) <<
                                                          Counter_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - Counter_INVERT_LINE   - Inverts the line output
*         - Counter_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_INV_OUT_MASK;
    Counter_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Counter_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void Counter_WriteCounter(uint32 count)
{
    Counter_COUNTER_REG = (count & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 Counter_ReadCounter(void)
{
    return (Counter_COUNTER_REG & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Counter_COUNT_UP       - Counts up
*     - Counter_COUNT_DOWN     - Counts down
*     - Counter_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Counter_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_UPDOWN_MASK;
    Counter_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Counter_WritePeriod(uint32 period)
{
    Counter_PERIOD_REG = (period & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Counter_ReadPeriod(void)
{
    return (Counter_PERIOD_REG & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_RELOAD_CC_MASK;
    Counter_CONTROL_REG |= (swapEnable & Counter_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Counter_WritePeriodBuf(uint32 periodBuf)
{
    Counter_PERIOD_BUF_REG = (periodBuf & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Counter_ReadPeriodBuf(void)
{
    return (Counter_PERIOD_BUF_REG & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_CONTROL_REG &= (uint32)~Counter_RELOAD_PERIOD_MASK;
    Counter_CONTROL_REG |= ((uint32)((swapEnable & Counter_1BIT_MASK) <<
                                                            Counter_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Counter_WriteCompare(uint32 compare)
{
    #if (Counter_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Counter_CY_TCPWM_4000) */

    #if (Counter_CY_TCPWM_4000)
        currentMode = ((Counter_CONTROL_REG & Counter_UPDOWN_MASK) >> Counter_UPDOWN_SHIFT);

        if (((uint32)Counter__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Counter__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Counter_CY_TCPWM_4000) */
    
    Counter_COMP_CAP_REG = (compare & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Counter_ReadCompare(void)
{
    #if (Counter_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Counter_CY_TCPWM_4000) */

    #if (Counter_CY_TCPWM_4000)
        currentMode = ((Counter_CONTROL_REG & Counter_UPDOWN_MASK) >> Counter_UPDOWN_SHIFT);
        
        regVal = Counter_COMP_CAP_REG;
        
        if (((uint32)Counter__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Counter__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Counter_16BIT_MASK);
    #else
        return (Counter_COMP_CAP_REG & Counter_16BIT_MASK);
    #endif /* (Counter_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Counter_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Counter_WriteCompareBuf(uint32 compareBuf)
{
    #if (Counter_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Counter_CY_TCPWM_4000) */

    #if (Counter_CY_TCPWM_4000)
        currentMode = ((Counter_CONTROL_REG & Counter_UPDOWN_MASK) >> Counter_UPDOWN_SHIFT);

        if (((uint32)Counter__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Counter__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Counter_CY_TCPWM_4000) */
    
    Counter_COMP_CAP_BUF_REG = (compareBuf & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Counter_ReadCompareBuf(void)
{
    #if (Counter_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Counter_CY_TCPWM_4000) */

    #if (Counter_CY_TCPWM_4000)
        currentMode = ((Counter_CONTROL_REG & Counter_UPDOWN_MASK) >> Counter_UPDOWN_SHIFT);

        regVal = Counter_COMP_CAP_BUF_REG;
        
        if (((uint32)Counter__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Counter__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Counter_16BIT_MASK);
    #else
        return (Counter_COMP_CAP_BUF_REG & Counter_16BIT_MASK);
    #endif /* (Counter_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Counter_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 Counter_ReadCapture(void)
{
    return (Counter_COMP_CAP_REG & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 Counter_ReadCaptureBuf(void)
{
    return (Counter_COMP_CAP_BUF_REG & Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_TRIG_LEVEL     - Level
*     - Counter_TRIG_RISING    - Rising edge
*     - Counter_TRIG_FALLING   - Falling edge
*     - Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_TRIG_CONTROL1_REG &= (uint32)~Counter_CAPTURE_MASK;
    Counter_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_TRIG_LEVEL     - Level
*     - Counter_TRIG_RISING    - Rising edge
*     - Counter_TRIG_FALLING   - Falling edge
*     - Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_TRIG_CONTROL1_REG &= (uint32)~Counter_RELOAD_MASK;
    Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_TRIG_LEVEL     - Level
*     - Counter_TRIG_RISING    - Rising edge
*     - Counter_TRIG_FALLING   - Falling edge
*     - Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_TRIG_CONTROL1_REG &= (uint32)~Counter_START_MASK;
    Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_TRIG_LEVEL     - Level
*     - Counter_TRIG_RISING    - Rising edge
*     - Counter_TRIG_FALLING   - Falling edge
*     - Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_TRIG_CONTROL1_REG &= (uint32)~Counter_STOP_MASK;
    Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_TRIG_LEVEL     - Level
*     - Counter_TRIG_RISING    - Rising edge
*     - Counter_TRIG_FALLING   - Falling edge
*     - Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_TRIG_CONTROL1_REG &= (uint32)~Counter_COUNT_MASK;
    Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - Counter_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Counter_CMD_RELOAD     - Trigger Reload/Index command
*     - Counter_CMD_STOP       - Trigger Stop/Kill command
*     - Counter_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Counter_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Counter.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Counter_STATUS_DOWN    - Set if counting down
*     - Counter_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Counter_ReadStatus(void)
{
    return ((Counter_STATUS_REG >> Counter_RUNNING_STATUS_SHIFT) |
            (Counter_STATUS_REG & Counter_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Counter_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Counter_INTR_MASK_TC       - Terminal count mask
*     - Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetInterruptMode(uint32 interruptMask)
{
    Counter_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Counter_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - Counter_INTR_MASK_TC       - Terminal count mask
*     - Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Counter_GetInterruptSourceMasked(void)
{
    return (Counter_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Counter_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - Counter_INTR_MASK_TC       - Terminal count mask
*     - Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Counter_GetInterruptSource(void)
{
    return (Counter_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Counter_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Counter_INTR_MASK_TC       - Terminal count mask
*     - Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_ClearInterrupt(uint32 interruptMask)
{
    Counter_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Counter_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Counter_INTR_MASK_TC       - Terminal count mask
*     - Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_SetInterrupt(uint32 interruptMask)
{
    Counter_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
