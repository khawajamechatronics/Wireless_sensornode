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

#include "MyInclusionMode.h"

// global variables
extern MyMessage _msgTmp;

unsigned long _inclusionStartTime;
bool _inclusionMode;

inline void inclusionInit()
{
	_inclusionMode = false;
#if defined(MY_INCLUSION_BUTTON_FEATURE)
	// Setup digital in that triggers inclusion mode
	hwPinMode(MY_INCLUSION_MODE_BUTTON_PIN, INPUT);
	hwDigitalWrite(MY_INCLUSION_MODE_BUTTON_PIN, HIGH);
#endif

}


void inclusionModeSet(bool newMode)
{
	if (newMode != _inclusionMode) {
		_inclusionMode = newMode;
		// Send back mode change to controller
        buildGw(&_msgTmp, I_INCLUSION_MODE);
        uint8_t temp = (_inclusionMode?1:0);
        MyMessage_set(&_msgTmp, (void*)&temp, P_BYTE, 0, 0);
		gatewayTransportSend(&_msgTmp);
		if (_inclusionMode) {
			_inclusionStartTime = MyHwPsoc_hwMillis();
		}
	}
}

inline void inclusionProcess()
{
#ifdef MY_INCLUSION_BUTTON_FEATURE
	if (!_inclusionMode && hwDigitalRead(MY_INCLUSION_MODE_BUTTON_PIN) == MY_INCLUSION_BUTTON_PRESSED) {
		// Start inclusion mode
		inclusionModeSet(true);
	}
#endif

	if (_inclusionMode && MyHwPsoc_hwMillis()-_inclusionStartTime>MY_INCLUSION_MODE_DURATION*1000L) {
		// inclusionTimeInMinutes minute(s) has passed.. stop inclusion mode
		inclusionModeSet(false);
	}
}
