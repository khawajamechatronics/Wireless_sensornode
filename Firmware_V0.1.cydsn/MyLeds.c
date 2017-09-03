/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include "MyLeds.h"

#define LED_ON_OFF_RATIO        (4)       // Power of 2 please
#ifndef MY_DEFAULT_LED_BLINK_PERIOD
    #define MY_DEFAULT_LED_BLINK_PERIOD 300
#endif
#define LED_PROCESS_INTERVAL_MS (MY_DEFAULT_LED_BLINK_PERIOD/LED_ON_OFF_RATIO)

// these variables don't need to be volatile, since we are not using interrupts
static uint8_t countRx;
static uint8_t countTx;
static uint8_t countErr;
static unsigned long prevTime;

inline void ledsInit()
{
}

void ledsProcess()
{
    uint16_t hwMillis = TIMER1_ReadCounter();
    // Just return if it is not the time...
	if ((hwMillis-prevTime) < LED_PROCESS_INTERVAL_MS) {
		return;
	}
    prevTime = TIMER1_ReadCounter();

	uint8_t state;

	// For an On/Off ratio of 4, the pattern repeated will be [on, on, on, off]
	// until the counter becomes 0.
	if (countRx) {
		--countRx;
	}
	state = (countRx & (LED_ON_OFF_RATIO-1)) ? LED_ON : LED_OFF;
	LED_1_Write(state);

	if (countTx) {
		--countTx;
	}
	state = (countTx & (LED_ON_OFF_RATIO-1)) ? LED_ON : LED_OFF;
	LED_2_Write(state);

	if (countErr) {
		--countErr;
	}
	state = (countErr & (LED_ON_OFF_RATIO-1)) ? LED_ON : LED_OFF;
	LED_3_Write(state);

}

void ledsBlinkRx(uint8_t cnt)
{
	if (!countRx) {
		countRx = cnt*LED_ON_OFF_RATIO;
	}
	ledsProcess();
}

void ledsBlinkTx(uint8_t cnt)
{
	if(!countTx) {
		countTx = cnt*LED_ON_OFF_RATIO;
	}
	ledsProcess();
}

void ledsBlinkErr(uint8_t cnt)
{
	if(!countErr) {
		countErr = cnt*LED_ON_OFF_RATIO;
	}
	ledsProcess();
}

bool ledsBlinking()
{
	return countRx || countTx || countErr;
}


