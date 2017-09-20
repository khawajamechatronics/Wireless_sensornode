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

//Local variables
//Rainsensor

//Winddirection sensor
uint16          wind_vane_lut[8] = {218,372,657,1647,2941,2600,2232,1112};
//Windspeed sensor
winddirection   olddirection= north;

/* Variable to store the number of times the Counter has overflown */
uint32 counter_overflowCount = 0u;
/* Variable to store the count value that is captured at the instance of the falling edge of the
 * capture signal */
uint16 capturedCount = 0u;
/* Variable to indicate the occurrence of the Capture Interrupt. The Counter generates interrupt on the
 * Terminal count and / or on the Capture event. The flag is set inside the Capture interrupt and cleared in the
 * main.c after processing */
uint8 CC_ISR_flag = FALSE;

//Rainsensor
uint16_t        RainSense_count=0;
uint16_t        RainSense_countmm=0;

#ifndef bmetest
//BME280
#define SPI2
bme280_dev dev;
int8_t result = BME280_OK;
#endif

//Local functions

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

#ifndef bmetest
int8_t get_sensor_data_normal_mode()
{
    bme280_dev *devp = &dev;
    int8_t rslt;
	uint8_t settings_sel;
	bme280_data comp_data;

	devp->settings.osr_h = BME280_OVERSAMPLING_4X;
	devp->settings.osr_p = BME280_OVERSAMPLING_4X;
	devp->settings.osr_t = BME280_OVERSAMPLING_4X;

	settings_sel = BME280_OSR_PRESS_SEL|BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, devp);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, devp);
	/* Give some delay for the sensor to go into normal mode */
	CyDelay(5);
	
	rslt = bme280_get_sensor_data(BME280_PRESS | BME280_HUM | BME280_TEMP, &comp_data, devp);
    //print_sensor_data(&comp_data);
	return rslt;
}

int8_t get_sensor_data_forced_mode()
{
    bme280_dev *devp = &dev;
    int8_t rslt;
	uint8_t settings_sel;
	bme280_data comp_data;


	devp->settings.osr_h = BME280_OVERSAMPLING_1X;
	devp->settings.osr_p = BME280_OVERSAMPLING_1X;
	devp->settings.osr_t = BME280_OVERSAMPLING_1X;

	settings_sel = BME280_OSR_PRESS_SEL|BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL;

	rslt = bme280_set_sensor_settings(settings_sel, devp);
	rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, devp);
	/* Give some delay for the sensor to go into force mode */
	CyDelay(5);

	rslt = bme280_get_sensor_data(BME280_PRESS | BME280_HUM | BME280_TEMP, &comp_data, devp);
    #ifdef debug
    char outstring[100];
    uint16 templ = comp_data.temperature;
    uint16 temph = comp_data.temperature /100;
    templ = templ - (temph *100);
    sprintf(outstring, "Temperature is:\t%d.%02d C\r\n",temph,templ);
    UART_UartPutString(outstring);
    
    uint16 humil = comp_data.humidity;
    uint16 humih = comp_data.humidity / 1000;
    humil = humil - (humih *1000);
    sprintf(outstring, "Humidity is:\t%d.%03d %%\r\n",humih,humil);
    UART_UartPutString(outstring);
    
    uint16 presh = comp_data.pressure / 100;
    uint16 presl = comp_data.pressure;
    presl = presl - (presh *100);
    sprintf(outstring, "Pressure is:\t%d.%02d Pa\r\n\r\n",presh,presl);
    UART_UartPutString(outstring);
    #endif
	return rslt;
}

int8_t BME_SpiRead( uint8_t dev_id,volatile uint8_t reg_addr,volatile uint8_t *data,volatile uint16_t len)
{
    if(reg_addr == 0x74)
    {
        CyDelay(1);
    }
    len = len +1;
    uint8 packetsize = len;
    uint8 dummyBuffer[30];
    volatile uint8 tmpBuffer[30];
    uint32 i=0;
    for(i=0;i<30;i++)
    {
        dummyBuffer[i] = 0xFFu;
        tmpBuffer[i] = 0x00u;
    }
    i=0;
    dummyBuffer[0] = (reg_addr | 0x80);
    SPIM_ClearRxBuffer();
    while(len--)
    {
        SPIM_WriteTxData(dummyBuffer[i]);
        i++;
    }
    while (packetsize != SPIM_GetRxBufferSize())
    {
    }
    SPIM_ClearTxBuffer();
    
    i=0u;
    while(0u != SPIM_GetRxBufferSize())
    {
        tmpBuffer[i] = SPIM_ReadRxData();
        if(i > 0)
        {
            *data = tmpBuffer[i];
            data++;
        }
        i++;
        
    }
    return 0;
}

int8_t BME_SpiWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    static uint8_t mTxBuffer[2];
    mTxBuffer[0] = (reg_addr & (~0x80));
    mTxBuffer[1] = *data;
    SPIM_WriteTxData(mTxBuffer[0]);
    SPIM_WriteTxData(mTxBuffer[1]);
    while(0 != SPIM_GetTxBufferSize());
    return 0;
}

void BME_DelayMs(uint32_t delay)
{
    CyDelay(delay);
}
#endif
// Interrupts

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

CY_ISR( RainSense_Int_Handler )
{
    RainSense_count++;
    RainSense_ClearInterrupt();
}

//Public functions
void WEATHER_Setup()
{
    //Wind speed sensor
    Counter_Start();
    TIMER0_ISR_StartEx(Counter_ISR);
    CC_ISR_flag = FALSE;
    
    //Rain sensor
    RainSense_Int_StartEx( RainSense_Int_Handler );
    
    //BME280
    dev.id = 0;
    dev.interface = BME280_SPI_INTF;
    dev.read = &BME_SpiRead;
    dev.write = &BME_SpiWrite;
    dev.delay_ms = (void *)CyDelay;
    #ifdef SPI1
        SPI_BME280_Start();
    #endif
    #ifdef SPI2
        SPIM_Start();
    #endif
    result = bme280_init(&dev);
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
    windspeed = WINDSPEED_MULTIPLIER / capturedCount;    
    return windspeed;
}

uint16_t WEATHER_getRainGauge()
{
    RainSense_countmm = (RainSense_count * RAINGAUGE_MULTIPLIER) / 1000;
    return RainSense_countmm;
}

uint8_t WEATHER_getBME280(bme280_data *comp_data)
{
    bme280_dev *devp = &dev;
    int8_t rslt;
	uint8_t settings_sel;
	//bme280_data comp_data;

    //devp->settings.standby_time = BME280_STANDBY_TIME_1_MS;
    devp->settings.filter = BME280_FILTER_COEFF_16;
	devp->settings.osr_h = BME280_OVERSAMPLING_1X;
	devp->settings.osr_p = BME280_OVERSAMPLING_1X;
	devp->settings.osr_t = BME280_OVERSAMPLING_1X;

	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_STANDBY_SEL | BME280_FILTER_SEL;

	rslt = bme280_set_sensor_settings(settings_sel, devp);
	rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, devp);
	/* Give some delay for the sensor to go into force mode */
	CyDelay(5);
    
	rslt = bme280_get_sensor_data(BME280_ALL, comp_data, devp);
    
    #ifdef debug
    char outstring[100];
    uint16 templ = comp_data.temperature;
    uint16 temph = comp_data.temperature /100;
    templ = templ - (temph *100);
    sprintf(outstring, "Temperature is:\t%d.%02d C\r\n",temph,templ);
    UART_UartPutString(outstring);
    
    uint16 humil = comp_data.humidity;
    uint16 humih = comp_data.humidity / 1000;
    humil = humil - (humih *1000);
    sprintf(outstring, "Humidity is:\t%d.%03d %%\r\n",humih,humil);
    UART_UartPutString(outstring);
    
    uint16 presh = comp_data.pressure / 100;
    uint16 presl = comp_data.pressure;
    presl = presl - (presh *100);
    sprintf(outstring, "Pressure is:\t%d.%02d Pa\r\n\r\n",presh,presl);
    UART_UartPutString(outstring);
    #endif
	return rslt;
}



/* [] END OF FILE */
