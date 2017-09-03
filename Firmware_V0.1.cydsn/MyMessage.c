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
#include "MyMessage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MyMessage_MyMessage(MyMessage* message, uint8_t _sensor, uint8_t _type)
{
	MyMessage_clear(message);
    if(_sensor != 255)
	    message->sensor = _sensor;
	if(_type != 255)
        message->type   = _type;
}

void MyMessage_clear(MyMessage* message)
{
	message->last                = 0u;
	message->sender              = 0u;
	message->destination         = 0u;       // Gateway is default destination
	message->version_length      = 0u;
	message->command_ack_payload = 0u;
	message->type                = 0u;
	message->sensor              = 0u;
	(void)memset(message->payload_data.data, 0u, sizeof(message->payload_data));

	// set message protocol version
    BF_SET(message->version_length, PROTOCOL_VERSION, 0, 2);
}

bool MyMessage_isAck(MyMessage* message) //const
{
	return BF_GET(message->command_ack_payload, 4, 1);
}

uint8_t MyMessage_getCommand(MyMessage* message) //const
{
	return BF_GET(message->command_ack_payload, 0, 3);
}

/* Getters for payload converted to desired form */
void* MyMessage_getCustom(MyMessage* message) //const
{
	return (void *)message->payload_data.data;
}

const char* MyMessage_getString1(MyMessage* message) //const
{
	uint8_t payloadType = BF_GET(message->command_ack_payload, 5, 3);
	if (payloadType == P_STRING) {
		return message->payload_data.data;
	} else {
		return NULL;
	}
}

// handles single character hex (0 - 15)
char MyMessage_i2h(uint8_t i) //const
{
	uint8_t k = i & 0x0F;
	if (k <= 9) {
		return '0' + k;
	} else {
		return 'A' + k - 10;
	}
}

char* MyMessage_getCustomString(MyMessage* message, char *buffer) //const
{
    uint8_t i;
	for (i = 0; i < BF_GET(message->version_length, 3, 5); i++) {
		buffer[i * 2] = MyMessage_i2h(message->payload_data.data[i] >> 4);
		buffer[(i * 2) + 1] = MyMessage_i2h(message->payload_data.data[i]);
	}
	buffer[BF_GET(message->version_length, 3, 5) * 2] = '\0';
	return buffer;
}

char* MyMessage_getStream(MyMessage* message, char *buffer) //const
{
	uint8_t cmd = BF_GET(message->command_ack_payload, 0, 3);
	if ((cmd == C_STREAM) && (buffer != NULL)) {
		return  MyMessage_getCustomString(message, buffer);
	} else {
		return NULL;
	}
}

char* MyMessage_getString(MyMessage* message, char *buffer) //const
{
	if( buffer != NULL)
    {
        switch(BF_GET(message->command_ack_payload, 5, 3))
        {
            case P_STRING:
                strncpy(buffer, message->payload_data.data, BF_GET(message->version_length, 3, 5)); 
                buffer[BF_GET(message->version_length, 3, 5)] = 0;
                break;
            case P_BYTE:
                itoa(message->payload_data.bValue, buffer, 10);
                break;
            case P_INT16:
                itoa(message->payload_data.iValue, buffer, 10);
                break;
            case P_UINT16:
                utoa(message->payload_data.uiValue, buffer, 10);
                break;
            case P_LONG32:
                ltoa(message->payload_data.lValue, buffer, 10);
                break;
            case P_ULONG32:
                utoa(message->payload_data.ulValue, buffer, 10);
                break;
            case P_FLOAT32:
                dtostrf(message->payload_data.fValue,2,MIN(message->payload_data.fPrecision, (uint8_t)8),buffer);
                break;
            case P_CUSTOM:
                return MyMessage_getCustomString(message, buffer);
                break;
            default:  
                return 0;
                break;
        }
        return buffer;
    }
    else
    {
        return NULL;
    }
}

bool MyMessage_getBool(MyMessage* message) //const
{
	return MyMessage_getByte(message);
}

uint8_t MyMessage_getByte(MyMessage* message) //const
{
	switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_BYTE:
            return message->payload_data.data[0];
            break;
        case P_STRING:
            return atoi(message->payload_data.data);
            break;
        default:  
            return 0;
            break;
    }
}

float MyMessage_getFloat(MyMessage* message) //const
{
	switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_FLOAT32:
            return message->payload_data.fValue;
            break;
        case P_STRING:
            return atof(message->payload_data.data);
            break;
        default:  
            return 0;
            break;
    }
}

int32_t MyMessage_getLong(MyMessage* message) //const
{
	switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_LONG32:
            return message->payload_data.lValue;
            break;
        case P_STRING:
            return atol(message->payload_data.data);
            break;
        default:  
            return 0;
            break;
    }
}

uint32_t MyMessage_getULong(MyMessage* message) //const
{
    switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_ULONG32:
            return message->payload_data.ulValue;
            break;
        case P_STRING:
            return atol(message->payload_data.data);
            break;
        default:  
            return 0;
            break;
    }
}

int16_t MyMessage_getInt(MyMessage* message) //const
{
	switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_INT16:
            return message->payload_data.iValue;
            break;
        case P_STRING:
            return atoi(message->payload_data.data);
            break;
        default:
            return 0;
            break;
    }
}

uint16_t MyMessage_getUInt(MyMessage* message) //const
{
	switch(BF_GET(message->command_ack_payload, 5, 3))
    {
        case P_UINT16:
            return message->payload_data.uiValue;
            break;
        case P_STRING:
            return atoi(message->payload_data.data);
            break;
        default:
            return 0;
            break;
    }
}

void MyMessage_setType(MyMessage* message, uint8_t _type)
{
	message->type = _type;
}

void MyMessage_setSensor(MyMessage* message, uint8_t _sensor)
{
	message->sensor = _sensor;
}

void MyMessage_setDestination(MyMessage* message, uint8_t _destination)
{
	message->destination = _destination;
}

//Takes message pointer, and set data with datatype as type, length is used with pointer and float
void MyMessage_set( MyMessage* message, void* data, mysensor_payload datatype, uint8_t length, float fdata) //length is also used as precision in float
{
    uint8_t payloadLength;
    BF_SET(message->command_ack_payload, datatype, 5, 3);
    switch(datatype)
    {
        case P_CUSTOM:
            payloadLength = (data == NULL) ? 0 : MIN(length, (uint8_t)MAX_PAYLOAD);
            BF_SET(message->version_length, payloadLength, 3, 5);
            memcpy(message->payload_data.data, data, payloadLength);
            break;
        case P_STRING:
            payloadLength = (data == NULL) ? 0 : MIN(strlen((const char*)data), (size_t)MAX_PAYLOAD);
            BF_SET(message->version_length, payloadLength, 3, 5);
            if (payloadLength) {
                strncpy(message->payload_data.data,(const char*)data, payloadLength);
            }
            message->payload_data.data[payloadLength] = 0;
            break;
        case P_BYTE:
            BF_SET(message->version_length, 1, 3, 5);
            message->payload_data.data[0] = (char)data;
            break;
        case P_FLOAT32:
            BF_SET(message->version_length, 5, 3, 5); // 32 bit float + persi
            message->payload_data.fValue = fdata;
            message->payload_data.fPrecision = length;
            break;
        case P_ULONG32:
            BF_SET(message->version_length, 4, 3, 5);
            message->payload_data.ulValue = (uint32_t)data;
            break;
        case P_LONG32:
            BF_SET(message->version_length, 4, 3, 5);
            message->payload_data.lValue = (int32_t)data;
            break;
        case P_UINT16:
            BF_SET(message->version_length, 2, 3, 5);
            message->payload_data.uiValue = (uint16_t)data;
            break;
        case P_INT16:
            BF_SET(message->version_length, 2, 3, 5);
            message->payload_data.iValue = (int16_t)data;
            break;
        default:
            break;
    }
}
