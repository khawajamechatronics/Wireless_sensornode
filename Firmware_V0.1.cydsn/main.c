// Enable debug prints to serial monitor
#define MY_DEBUG


// Enable and select radio type attached
//#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Set LOW transmit power level as default, if you have an amplified NRF-module and
// power your radio separately with a good regulator you can turn up PA level.
#define MY_RF24_PA_LEVEL RF24_PA_LOW


// Enable serial gateway
#define MY_GATEWAY_SERIAL

// Define a lower baud rate for Arduino's running on 8 MHz (Arduino Pro Mini 3.3V & SenseBender)
#if F_CPU == 8000000L
#define MY_BAUD_RATE 38400
#endif

// Enable inclusion mode
#define MY_INCLUSION_MODE_FEATURE
// Enable Inclusion mode button on gateway
//#define MY_INCLUSION_BUTTON_FEATURE

// Inverses behavior of inclusion button (if using external pullup)
//#define MY_INCLUSION_BUTTON_EXTERNAL_PULLUP

// Set inclusion mode duration (in seconds)
#define MY_INCLUSION_MODE_DURATION 60
// Digital pin used for inclusion mode button
//#define MY_INCLUSION_MODE_BUTTON_PIN  3

// Set blinking period
#define MY_DEFAULT_LED_BLINK_PERIOD 300

// Inverses the behavior of leds
//#define MY_WITH_LEDS_BLINKING_INVERSE

// Flash leds on rx/tx/err
// Uncomment to override default HW configurations
#define MY_DEFAULT_ERR_LED_PIN 4  // Error led pin
#define MY_DEFAULT_RX_LED_PIN  6  // Receive led pin
#define MY_DEFAULT_TX_LED_PIN  5  // the PCB, on board LED

#include "project.h"
#include "MyLeds.h"
#include "MySensors.h"
#include "weathersensors.h"
#include "p1_meter.h"
#include "stdio.h"

#define USB
#ifdef USB
#define MOUSE_ENDPOINT      (1u)
#define MOUSE_DATA_LEN      (3u)
#define CURSOR_STEP         (5u)
#define CURSOR_STEP_POS     (1u)

/* Mouse packet array: button, X, Y */
uint8 mouseData[MOUSE_DATA_LEN] = {0u, 0u, 0u};
uint8 bSNstring[16u] = {0x0Eu, 0x03u, 'F', 0u, 'W', 0u, 'S', 0u, 'N', 0u, '0', 0u, '1', 0u};
#endif


//Mysensors Code
#define CHILD_ID_TEMP 0
#define CHILD_ID_HUM 1
#define CHILD_ID_BARO 2
#define CHILD_ID_POWER 3

const float ALTITUDE = 1;

const char *weather[] = { "stable", "sunny", "cloudy", "unstable", "thunderstorm", "unknown" };
enum FORECAST
{
  STABLE = 0,     // "Stable Weather Pattern"
  SUNNY = 1,      // "Slowly rising Good Weather", "Clear/Sunny "
  CLOUDY = 2,     // "Slowly falling L-Pressure ", "Cloudy/Rain "
  UNSTABLE = 3,   // "Quickly rising H-Press",     "Not Stable"
  THUNDERSTORM = 4, // "Quickly falling L-Press",    "Thunderstorm"
  UNKNOWN = 5     // "Unknown (More Time needed)
};

MyMessage msgHum;
MyMessage msgTemp;
MyMessage msgpressure;
MyMessage msgforecast;
MyMessage msgwatt;
MyMessage msgkwh;
MyMessage msgpc;

float       lastTemp = -1;
float       lastHum = -1;
float       lastPressure = -1;
float       lastkwh = 0;
float       lastwatt = 0;


int         lastForecast = -1;
#define LAST_SAMPLES_COUNT 5

//const int   LAST_SAMPLES_COUNT = 5;
float       lastPressureSamples[LAST_SAMPLES_COUNT];

// this CONVERSION_FACTOR is used to convert from Pa to kPa in the forecast algorithm
// get kPa/h by dividing hPa by 10 
#define CONVERSION_FACTOR (1.0/10.0)

int         minuteCount = 0;
bool        firstRound = true;
// average value is used in forecast algorithm.
float       pressureAvg;
// average after 2 hours is used as reference value for the next iteration.
float       pressureAvg2;

float       dP_dt;

uint8_t     nNoUpdatesTemp;
uint8_t     nNoUpdatesHum;
bool        metric = true;
uint8_t     dummy;
bme280_data bmedata;
P1_data     meterdata;

float getLastPressureSamplesAverage()
{
  float lastPressureSamplesAverage = 0;
  for (int i = 0; i < LAST_SAMPLES_COUNT; i++)
  {
    lastPressureSamplesAverage += lastPressureSamples[i];
  }
  lastPressureSamplesAverage /= LAST_SAMPLES_COUNT;

  return lastPressureSamplesAverage;
}

// Algorithm found here
// http://www.freescale.com/files/sensors/doc/app_note/AN3914.pdf
// Pressure in hPa -->  forecast done by calculating kPa/h
int sample(float pressure)
{
  // Calculate the average of the last n minutes.
  int index = minuteCount % LAST_SAMPLES_COUNT;
  lastPressureSamples[index] = pressure;

  minuteCount++;
  if (minuteCount > 185)
  {
    minuteCount = 6;
  }

  if (minuteCount == 5)
  {
    pressureAvg = getLastPressureSamplesAverage();
  }
  else if (minuteCount == 35)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) // first time initial 3 hour
    {
      dP_dt = change * 2; // note this is for t = 0.5hour
    }
    else
    {
      dP_dt = change / 1.5; // divide by 1.5 as this is the difference in time from 0 value.
    }
  }
  else if (minuteCount == 65)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) //first time initial 3 hour
    {
      dP_dt = change; //note this is for t = 1 hour
    }
    else
    {
      dP_dt = change / 2; //divide by 2 as this is the difference in time from 0 value
    }
  }
  else if (minuteCount == 95)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) // first time initial 3 hour
    {
      dP_dt = change / 1.5; // note this is for t = 1.5 hour
    }
    else
    {
      dP_dt = change / 2.5; // divide by 2.5 as this is the difference in time from 0 value
    }
  }
  else if (minuteCount == 125)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    pressureAvg2 = lastPressureAvg; // store for later use.
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) // first time initial 3 hour
    {
      dP_dt = change / 2; // note this is for t = 2 hour
    }
    else
    {
      dP_dt = change / 3; // divide by 3 as this is the difference in time from 0 value
    }
  }
  else if (minuteCount == 155)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) // first time initial 3 hour
    {
      dP_dt = change / 2.5; // note this is for t = 2.5 hour
    }
    else
    {
      dP_dt = change / 3.5; // divide by 3.5 as this is the difference in time from 0 value
    }
  }
  else if (minuteCount == 185)
  {
    float lastPressureAvg = getLastPressureSamplesAverage();
    float change = (lastPressureAvg - pressureAvg) * CONVERSION_FACTOR;
    if (firstRound) // first time initial 3 hour
    {
      dP_dt = change / 3; // note this is for t = 3 hour
    }
    else
    {
      dP_dt = change / 4; // divide by 4 as this is the difference in time from 0 value
    }
    pressureAvg = pressureAvg2; // Equating the pressure at 0 to the pressure at 2 hour after 3 hours have past.
    firstRound = false; // flag to let you know that this is on the past 3 hour mark. Initialized to 0 outside main loop.
  }

  int forecast = UNKNOWN;
  if (minuteCount < 35 && firstRound) //if time is less than 35 min on the first 3 hour interval.
  {
    forecast = UNKNOWN;
  }
  else if (dP_dt < (-0.25))
  {
    forecast = THUNDERSTORM;
  }
  else if (dP_dt > 0.25)
  {
    forecast = UNSTABLE;
  }
  else if ((dP_dt > (-0.25)) && (dP_dt < (-0.05)))
  {
    forecast = CLOUDY;
  }
  else if ((dP_dt > 0.05) && (dP_dt < 0.25))
  {
    forecast = SUNNY;
  }
  else if ((dP_dt >(-0.05)) && (dP_dt < 0.05))
  {
    forecast = STABLE;
  }
  else
  {
    forecast = UNKNOWN;
  }

  // uncomment when debugging
  //Serial.print(F("Forecast at minute "));
  //Serial.print(minuteCount);
  //Serial.print(F(" dP/dt = "));
  //Serial.print(dP_dt);
  //Serial.print(F("kPa/h --> "));
  //Serial.println(weather[forecast]);

  return forecast;
}


void setup()
{
    // Setup  locally attached sensor
    WEATHER_Setup();
    msgTemp.sensor = CHILD_ID_TEMP;
    msgTemp.type = V_TEMP;
    msgHum.sensor = CHILD_ID_HUM;
    msgHum.type = V_HUM;
    msgpressure.sensor = CHILD_ID_BARO;
    msgpressure.type = V_PRESSURE;
    msgforecast.sensor = CHILD_ID_BARO;
    msgforecast.type = V_FORECAST;
    msgkwh.sensor = CHILD_ID_POWER;
    msgkwh.type = V_KWH;
    msgwatt.sensor = CHILD_ID_POWER;
    msgwatt.type = V_WATT;
    
}

void presentation()
{
    sendSketchInfo("BME280 Sensor", "1.6",false);
    present(CHILD_ID_HUM, S_HUM,"",0);
    present(CHILD_ID_TEMP, S_TEMP,"",0);
    present(CHILD_ID_BARO, S_BARO,"",0);
    present(CHILD_ID_POWER, S_POWER,"",0);
}


void loop()
{
    //P1_getdata(&meterdata);
    float watt = meterdata.currentConsumption;
    uint32_t kwh_int = meterdata.lowTariff + meterdata.highTariff;
    float kwh = kwh_int;
    kwh = kwh / 1000;
    
    if(watt != lastwatt)
    {
        MyMessage_set(&msgwatt,&dummy,P_FLOAT32,0,watt);
        send(&msgwatt,false);
        lastwatt = watt;        
    }
    if(kwh != lastkwh)
    {
        MyMessage_set(&msgkwh,&dummy,P_FLOAT32,3,kwh);
        send(&msgkwh,false);
        lastkwh = kwh; 
    }
    WEATHER_getBME280(&bmedata);
    float temp = bmedata.temperature;;
    float hum = bmedata.humidity;
    float pressure_local = bmedata.pressure;
    float pressure = pressure_local / 100;//pressure_local/pow((1.0 - ( ALTITUDE / 44330.0 )), 5.255); // Adjust to sea level pressure using user altitude
    int forecast = sample(pressure);
    
    temp = temp / 100;
    if(temp != lastTemp)
    {
        MyMessage_set(&msgTemp,&dummy,P_FLOAT32,2,temp);
        send(&msgTemp,false);
        lastTemp = temp;
    }
    
    hum = bmedata.humidity / 1000;
    if(hum != lastHum)
    {
        MyMessage_set(&msgHum,&dummy,P_FLOAT32,2,hum);
        send(&msgHum,false);
        lastHum = hum;
    }
    if (pressure != lastPressure) 
    {
        MyMessage_set(&msgpressure,&dummy,P_FLOAT32,2,pressure);
        send(&msgpressure,false);
        lastPressure = pressure;
    }
    if (forecast != lastForecast)
    {
        MyMessage_set(&msgforecast,&weather[forecast],P_STRING,0,0);
        send(&msgforecast,false);
        lastForecast = forecast;
    }
    
    wait(5000);
    // Send  locally attached sensor data here
}

void (*serialEventRun)() = NULL;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    _begin(); //Startup MySensors library
    for(;;) 
    {
        _process(); //process incoming data
        if (1) 
        {
            loop(); // Call sketch loop
        }
        if (serialEventRun) 
        {
            serialEventRun();
        }
    }    
}

/* [] END OF FILE */
