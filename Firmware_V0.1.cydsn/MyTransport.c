/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2016 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include "MyTransport.h"

// SM: transitions and update states
static transportState_t stInit = { stInitTransition, stInitUpdate };
static transportState_t stParent = { stParentTransition, stParentUpdate };
static transportState_t stID = { stIDTransition, stIDUpdate };
static transportState_t stUplink = { stUplinkTransition, stUplinkUpdate };
static transportState_t stReady = { stReadyTransition, stReadyUpdate };
static transportState_t stFailure = { stFailureTransition, stFailureUpdate };

// transport SM variables
static transportSM_t _transportSM;

// transport configuration
static transportConfig_t _transportConfig;

// callback transportOk
transportCallback_t transportReady_cb = NULL;

// global variables
extern MyMessage _msg;		// incoming message
extern MyMessage _msgTmp;	// outgoing message

#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
static routingTable_t _transportRoutingTable;		//!< routing table
static uint32_t _lastRoutingTableSave;			//!< last routing table dump
#endif

// regular sanity check, activated by default on GW and repeater nodes
#if defined(MY_TRANSPORT_SANITY_CHECK)
static uint32_t _lastSanityCheck;		//!< last sanity check
#endif

// regular network discovery, sends I_DISCOVER_REQUESTS to update routing table
// sufficient to have GW triggering requests to also update repeater nodes
#if defined(MY_GATEWAY_FEATURE)
static uint32_t _lastNetworkDiscovery;	//! last network discovery
#endif

// stInit: initialise transport HW
void stInitTransition(void)
{
	TRANSPORT_DEBUG(PSTR("TSM:INIT\n"));
	// initialise status variables
	_transportSM.pingActive = false;
	_transportSM.transportActive = false;
	_transportSM.lastUplinkCheck = 0ul;

#if defined(MY_TRANSPORT_SANITY_CHECK)
	_lastSanityCheck = MyHwPsoc_hwMillis();
#endif
#if defined(MY_GATEWAY_FEATURE)
	_lastNetworkDiscovery = 0ul;
#endif
#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	_lastRoutingTableSave = hwMillis();
#endif

	// Read node settings (ID, parent ID, GW distance) from EEPROM
    MyHwPsoc_ReadConfigBlock(   (void*)&_transportConfig, 
                                (void*)EEPROM_NODE_ID_ADDRESS,
	                            sizeof(transportConfig_t));
}

void stInitUpdate(void)
{
	// initialise radio
	if (!transportInit()) {
		TRANSPORT_DEBUG(PSTR("!TSM:INIT:TSP FAIL\n"));
		setIndication(INDICATION_ERR_INIT_TRANSPORT);
		transportSwitchSM(&stFailure);
	} else {
		TRANSPORT_DEBUG(PSTR("TSM:INIT:TSP OK\n"));
		_transportSM.transportActive = true;
#if defined(MY_GATEWAY_FEATURE)
		// Set configuration for gateway
		TRANSPORT_DEBUG(PSTR("TSM:INIT:GW MODE\n"));
		_transportConfig.parentNodeId = GATEWAY_ADDRESS;
		_transportConfig.distanceGW = 0u;
		_transportConfig.nodeId = GATEWAY_ADDRESS;
		transportSetAddress(GATEWAY_ADDRESS);
		// GW mode: skip FPAR,ID,UPL states
		transportSwitchSM(&stReady);
#else
		if (MY_NODE_ID != AUTO) {
			TRANSPORT_DEBUG(PSTR("TSM:INIT:STATID=%d\n"),(uint8_t)MY_NODE_ID);
			// Set static ID
			_transportConfig.nodeId = (uint8_t)MY_NODE_ID;
			// Save static ID to eeprom (for bootloader)
			hwWriteConfig(EEPROM_NODE_ID_ADDRESS, (uint8_t)MY_NODE_ID);
		}
		// assign ID if set
		if (_transportConfig.nodeId == AUTO || transportAssignNodeID(_transportConfig.nodeId)) {
			// if node ID valid (>0 and <255), proceed to next state
			transportSwitchSM(stParent);
		} else {
			// ID invalid (0 or 255)
			transportSwitchSM(stFailure);
		}
#endif
	}
}

// stParent: find parent
void stParentTransition(void)
{
	TRANSPORT_DEBUG(PSTR("TSM:FPAR\n"));	// find parent
	setIndication(INDICATION_FIND_PARENT);
	_transportSM.uplinkOk = false;
	_transportSM.preferredParentFound = false;
#if defined(MY_PARENT_NODE_IS_STATIC)
	TRANSPORT_DEBUG(PSTR("TSM:FPAR:STATP=%d\n"), (uint8_t)MY_PARENT_NODE_ID);	// static parent
	_transportSM.findingParentNode = false;
	_transportConfig.distanceGW = 1u;	// assumption, CHKUPL:GWDC will update this variable
	_transportConfig.parentNodeId = (uint8_t)MY_PARENT_NODE_ID;
	// save parent ID to eeprom (for bootloader)
	hwWriteConfig(EEPROM_PARENT_NODE_ID_ADDRESS, (uint8_t)MY_PARENT_NODE_ID);
#else
	_transportSM.findingParentNode = true;
	_transportConfig.distanceGW = DISTANCE_INVALID;	// Set distance to max and invalidate parent node ID
	_transportConfig.parentNodeId = AUTO;
	// Broadcast find parent request
    build(&_msgTmp, BROADCAST_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,I_FIND_PARENT_REQUEST, false);
	MyMessage_set(&_msgTmp, "", P_STRING, 0, 0);
    (void)transportRouteMessage(&_msgTmp);
#endif
}

// stParentUpdate
void stParentUpdate(void)
{
#if defined(MY_PARENT_NODE_IS_STATIC)
	// skipping find parent
	setIndication(INDICATION_GOT_PARENT);
	transportSwitchSM(stID);
#else
	if (transportTimeInState() > MY_TRANSPORT_STATE_TIMEOUT_MS || _transportSM.preferredParentFound) {
		// timeout or preferred parent found
		if (_transportConfig.parentNodeId != AUTO) {
			// parent assigned
			TRANSPORT_DEBUG(PSTR("TSM:FPAR:OK\n"));	// find parent ok
			_transportSM.findingParentNode = false;
			setIndication(INDICATION_GOT_PARENT);
			// go to next state
			transportSwitchSM(&stID);
		} else {
			// timeout w/o reply or valid parent
			if (_transportSM.stateRetries < MY_TRANSPORT_STATE_RETRIES) {
				// retries left
				TRANSPORT_DEBUG(PSTR("!TSM:FPAR:NO REPLY\n"));		// find parent, no reply
				// reenter state
				transportSwitchSM(&stParent);
			} else {
				// no retries left, finding parent failed
				TRANSPORT_DEBUG(PSTR("!TSM:FPAR:FAIL\n"));
				setIndication(INDICATION_ERR_FIND_PARENT);
				transportSwitchSM(&stFailure);
			}
		}
	}
#endif
}

// stID: verify and request ID if necessary
void stIDTransition(void)
{
	TRANSPORT_DEBUG(PSTR("TSM:ID\n"));	// verify/request node ID
	if (_transportConfig.nodeId == AUTO) {
		// send ID request
		setIndication(INDICATION_REQ_NODEID);
		TRANSPORT_DEBUG(PSTR("TSM:ID:REQ\n"));	// request node ID
        build(&_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,I_ID_REQUEST,false);
		MyMessage_set(&_msgTmp, "", P_STRING, 0, 0);
        (void)transportRouteMessage(&_msgTmp);
	}
}

void stIDUpdate(void)
{
	if (_transportConfig.nodeId != AUTO) {
		// current node ID is valid
		TRANSPORT_DEBUG(PSTR("TSM:ID:OK\n"));
		setIndication(INDICATION_GOT_NODEID);
		// proceed to next state
		transportSwitchSM(&stUplink);
	} else if (transportTimeInState() > MY_TRANSPORT_STATE_TIMEOUT_MS) {
		// timeout
		if (_transportSM.stateRetries < MY_TRANSPORT_STATE_RETRIES) {
			// retries left: reenter state
			transportSwitchSM(&stID);
		} else {
			// no retries left
			TRANSPORT_DEBUG(PSTR("!TSM:ID:FAIL\n"));
			setIndication(INDICATION_ERR_GET_NODEID);
			transportSwitchSM(&stFailure);
		}
	}
}

void stUplinkTransition(void)
{
#if !defined(MY_TRANSPORT_UPLINK_CHECK_DISABLED)
	TRANSPORT_DEBUG(PSTR("TSM:UPL\n"));
	setIndication(INDICATION_CHECK_UPLINK);
	_transportSM.pingResponse = INVALID_HOPS;
	_transportSM.pingActive = true;
    build(&_msgTmp,GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,I_PING,false);
    uint8_t tmp = 0x01;
	MyMessage_set(&_msgTmp, &tmp, P_BYTE, 0, 0); 
    (void)transportRouteMessage(&_msgTmp);
#endif
}

void stUplinkUpdate(void)
{
#if !defined(MY_TRANSPORT_UPLINK_CHECK_DISABLED)
	if (_transportSM.pingResponse != INVALID_HOPS) {
		_transportSM.lastUplinkCheck = MyHwPsoc_hwMillis();
		// uplink ok, i.e. GW replied
		TRANSPORT_DEBUG(PSTR("TSM:UPL:OK\n"));	// uplink ok
		if (_transportSM.pingResponse != _transportConfig.distanceGW) {
			TRANSPORT_DEBUG(PSTR("TSM:UPL:DGWC,O=%d,N=%d\n"), _transportConfig.distanceGW,
			                _transportSM.pingResponse);	// distance to GW changed
			_transportConfig.distanceGW = _transportSM.pingResponse;
		}
		transportSwitchSM(&stReady);		// proceed to next state
	} else if (transportTimeInState() > MY_TRANSPORT_STATE_TIMEOUT_MS) {
		// timeout
		if (_transportSM.stateRetries < MY_TRANSPORT_STATE_RETRIES) {
			// retries left: reenter state
			transportSwitchSM(&stUplink);
		} else {
			// no retries left
			TRANSPORT_DEBUG(PSTR("!TSM:UPL:FAIL\n"));	// uplink check failed
			_transportSM.pingActive = false;
			setIndication(INDICATION_ERR_CHECK_UPLINK);
			transportSwitchSM(&stParent);	// go back to stParent
		}
	}
#else
	TRANSPORT_DEBUG(PSTR("TSM:UPL:DISABLED\n"));	// uplink check disabled
	transportSwitchSM(stReady);
#endif
}

void stReadyTransition(void)
{
	// transport is ready and fully operational
	TRANSPORT_DEBUG(PSTR("TSM:READY:ID=%d,PAR=%d,DIS=%d\n"), _transportConfig.nodeId,
	                _transportConfig.parentNodeId, _transportConfig.distanceGW);
	_transportSM.uplinkOk = true;
	_transportSM.failureCounter = 0u;			// reset failure counter
	_transportSM.failedUplinkTransmissions = 0u;	// reset failed uplink TX counter
	// callback
	if (transportReady_cb) {
		transportReady_cb();
	}
}

// stReadyUpdate: monitors link
void stReadyUpdate(void)
{
#if defined(MY_GATEWAY_FEATURE)
	if (MyHwPsoc_hwMillis() - _lastNetworkDiscovery > MY_TRANSPORT_DISCOVERY_INTERVAL_MS) {
		_lastNetworkDiscovery = MyHwPsoc_hwMillis();
		TRANSPORT_DEBUG(PSTR("TSM:READY:NWD REQ\n"));	// send transport network discovery
		build(&_msgTmp, BROADCAST_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_DISCOVER_REQUEST, false);
        MyMessage_set(&_msgTmp, "", P_STRING, 0, 0);
        (void)transportRouteMessage(&_msgTmp);
	}
#else
	if (_transportSM.failedUplinkTransmissions > MY_TRANSPORT_MAX_TX_FAILURES) {
		// too many uplink transmissions failed, find new parent (if non-static)
#if !defined(MY_PARENT_NODE_IS_STATIC)
		TRANSPORT_DEBUG(PSTR("!TSM:READY:UPL FAIL,SNP\n"));		// uplink failed, search new parent
		transportSwitchSM(stParent);
#else
		TRANSPORT_DEBUG(PSTR("!TSM:READY:UPL FAIL,STATP\n"));	// uplink failed, static parent
		// reset counter
		_transportSM.failedUplinkTransmissions = 0u;
#endif
	}
#endif

#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	if (hwMillis() - _lastRoutingTableSave > MY_ROUTING_TABLE_SAVE_INTERVAL_MS) {
		_lastRoutingTableSave = hwMillis();
		transportSaveRoutingTable();
	}
#endif
}

// stFailure: entered upon HW init failure or max retries exceeded
void stFailureTransition(void)
{
	if (_transportSM.failureCounter < MY_TRANSPORT_MAX_TSM_FAILURES) {
		_transportSM.failureCounter++;		// increment consecutive TSM failure counter
	}
	TRANSPORT_DEBUG(PSTR("TSM:FAIL:CNT=%d\n"),_transportSM.failureCounter);
	_transportSM.uplinkOk = false;			// uplink nok
	_transportSM.transportActive = false;	// transport inactive
	setIndication(INDICATION_ERR_INIT_TRANSPORT);
#if defined(MY_SENSOR_NETWORK)
	TRANSPORT_DEBUG(PSTR("TSM:FAIL:PDT\n"));	// power down transport, no need until re-init
	transportPowerDown();
#endif
}

void stFailureUpdate(void)
{
	if (transportTimeInState() > ( isTransportExtendedFailure()? MY_TRANSPORT_TIMEOUT_EXT_FAILURE_STATE:
	                               MY_TRANSPORT_TIMEOUT_FAILURE_STATE) ) {
		TRANSPORT_DEBUG(PSTR("TSM:FAIL:RE-INIT\n"));	// attempt to re-initialise transport
		transportSwitchSM(&stInit);
	}
}

void transportSwitchSM(transportState_t* newState)
{
    //TODO: Check Function below
	if (_transportSM.currentState != newState) {
		_transportSM.stateRetries = 0u;	// state change, reset retry counter
		_transportSM.currentState = newState;	// change state
	} else {
		_transportSM.stateRetries++;	// increment retries
	}
	if (_transportSM.currentState->Transition) {
		_transportSM.currentState->Transition();	// State transition
	}
	_transportSM.stateEnter = MyHwPsoc_hwMillis();	// save time
}

uint32_t transportTimeInState(void)
{
	return MyHwPsoc_hwMillis() - _transportSM.stateEnter;
}

void transportUpdateSM(void)
{
	if (_transportSM.currentState->Update) {
		_transportSM.currentState->Update();
	}
}

bool isTransportReady(void)
{
	return _transportSM.uplinkOk;
}

bool isTransportExtendedFailure(void)
{
	return _transportSM.failureCounter == MY_TRANSPORT_MAX_TSM_FAILURES;
}

bool isTransportSearchingParent(void)
{
	return _transportSM.findingParentNode;
}

bool isMessageReceived(void)
{
	return _transportSM.msgReceived;
}

void resetMessageReceived(void)
{
	_transportSM.msgReceived = false;
}


void transportInitialise(void)
{
	_transportSM.failureCounter = 0u;	// reset failure counter
	transportLoadRoutingTable();		// load routing table to RAM (if feature enabled)
	// intial state
	_transportSM.currentState = NULL;
	transportSwitchSM(&stInit);
}

bool transportWaitUntilReady(const uint32_t waitingMS)
{
	// check if transport ready
	TRANSPORT_DEBUG(PSTR("TSF:WUR:MS=%lu\n"), waitingMS);	// timeout
	uint32_t enterMS = MyHwPsoc_hwMillis();
	bool result = false;
	while (!result && ( MyHwPsoc_hwMillis() - enterMS < waitingMS || !waitingMS)) {
		transportProcess();
		result = isTransportReady();
		doYield();
	}
	return result;
}

// update TSM and process incoming messages
void transportProcess(void)
{
	// update state machine
	transportUpdateSM();
	// process transport FIFO
	transportProcessFIFO();
}


bool transportCheckUplink(const bool force)
{
	if (!force && (MyHwPsoc_hwMillis() - _transportSM.lastUplinkCheck) < MY_TRANSPORT_CHKUPL_INTERVAL_MS) {
		TRANSPORT_DEBUG(PSTR("TSF:CKU:OK,FCTRL\n"));	// flood control
		return true;
	}
	// ping GW
	const uint8_t hopsCount = transportPingNode(GATEWAY_ADDRESS);
	// verify hops
	if (hopsCount != INVALID_HOPS) {
		// update
		_transportSM.lastUplinkCheck = MyHwPsoc_hwMillis();
		TRANSPORT_DEBUG(PSTR("TSF:CKU:OK\n"));
		// did distance to GW change upstream, eg. re-routing of uplink nodes
		if (hopsCount != _transportConfig.distanceGW) {
			TRANSPORT_DEBUG(PSTR("TSF:CKU:DGWC,O=%d,N=%d\n"), _transportConfig.distanceGW,
			                hopsCount);	// distance to GW changed
			_transportConfig.distanceGW = hopsCount;
		}
		return true;
	} else {
		TRANSPORT_DEBUG(PSTR("TSF:CKU:FAIL\n"));
		return false;
	}
}

bool transportAssignNodeID(const uint8_t newNodeId)
{
	// verify if ID valid
	if (newNodeId != GATEWAY_ADDRESS && newNodeId != AUTO) {
		_transportConfig.nodeId = newNodeId;
		transportSetAddress(newNodeId);
		// Write ID to EEPROM
		MyHwPsoc_WriteConfig(EEPROM_NODE_ID_ADDRESS, newNodeId);
		TRANSPORT_DEBUG(PSTR("TSF:SID:OK,ID=%d\n"),newNodeId);	// Node ID assigned
		return true;
	} else {
		TRANSPORT_DEBUG(PSTR("!TSF:SID:FAIL,ID=%d\n"),newNodeId);	// ID is invalid, cannot assign ID
		setIndication(INDICATION_ERR_NET_FULL);
		_transportConfig.nodeId = AUTO;
		return false;
	}
}

bool transportRouteMessage(MyMessage *message)
{
	const uint8_t destination = message->destination;
	uint8_t route = _transportConfig.parentNodeId;	// by default, all traffic is routed via parent node

	if (_transportSM.findingParentNode && destination != BROADCAST_ADDRESS) {
		TRANSPORT_DEBUG(PSTR("!TSF:RTE:FPAR ACTIVE\n")); // find parent active, message not sent
		// request to send a non-BC message while finding parent active, abort
		return false;
	}

	if (destination == GATEWAY_ADDRESS) {
		route = _transportConfig.parentNodeId;		// message to GW always routes via parent
	} else if (destination == BROADCAST_ADDRESS) {
		route = BROADCAST_ADDRESS;		// message to BC does not require routing
	} else {
#if defined(MY_REPEATER_FEATURE)
		// destination not GW & not BC, get route
		route = transportGetRoute(destination);
		if (route == AUTO) {
			TRANSPORT_DEBUG(PSTR("!TSF:RTE:%d UNKNOWN\n"), destination);	// route unknown
#if !defined(MY_GATEWAY_FEATURE)
			if (message.last != _transportConfig.parentNodeId) {
				// message not from parent, i.e. child node - route it to parent
				route = _transportConfig.parentNodeId;
			} else {
				// route unknown and msg received from parent, send it to destination assuming in rx radius
				route = destination;
			}
#else
			// if GW, all unknown destinations are directly addressed
			route = destination;
#endif
		}
#else
		route = _transportConfig.parentNodeId;	// not a repeater, all traffic routed via parent
#endif
	}
	// send message
	const bool result = transportSendWrite(route, message);
#if !defined(MY_GATEWAY_FEATURE)
	// update counter
	if (route == _transportConfig.parentNodeId) {
		if (!result) {
			setIndication(INDICATION_ERR_TX);
			_transportSM.failedUplinkTransmissions++;
		} else {
			_transportSM.failedUplinkTransmissions = 0u;
		}
	}
#else
	if(!result) {
		setIndication(INDICATION_ERR_TX);
	}
#endif

	return result;
}

bool transportSendRoute(MyMessage *message)
{
	bool result = false;
	if (isTransportReady()) {
		result = transportRouteMessage(message);
	} else {
		// TNR: transport not ready
		TRANSPORT_DEBUG(PSTR("!TSF:SND:TNR\n"));
	}
	return result;
}

// only be used inside transport
bool transportWait(const uint32_t waitingMS, const uint8_t cmd, const uint8_t msgType)
{
	const uint32_t enterMS = MyHwPsoc_hwMillis();
	// invalidate msg type
	_msg.type = !msgType;
	bool expectedResponse = false;
	while ((MyHwPsoc_hwMillis() - enterMS < waitingMS) && !expectedResponse) {
		// process incoming messages
		transportProcessFIFO();
		doYield();
		expectedResponse = (mGetCommand((&_msg)) == cmd && _msg.type == msgType);
	}
	return expectedResponse;
}

uint8_t transportPingNode(const uint8_t targetId)
{
	if(!_transportSM.pingActive) {
		TRANSPORT_DEBUG(PSTR("TSF:PNG:SEND,TO=%d\n"), targetId);
		if(targetId == _transportConfig.nodeId) {
			// pinging self
			_transportSM.pingResponse = 0u;
		} else {
			_transportSM.pingActive = true;
			_transportSM.pingResponse = INVALID_HOPS;
            build(&_msgTmp, targetId, NODE_SENSOR_ID, C_INTERNAL, I_PING, false);
            uint8_t tmp = 0x01;
            MyMessage_set(&_msgTmp, &tmp, P_BYTE, 0, 0);
			(void)transportRouteMessage(&_msgTmp);
			// Wait for ping reply or timeout
			(void)transportWait(2000, C_INTERNAL, I_PONG);
		}
		// make sure missing I_PONG msg does not block pinging function by leaving pignActive=true
		_transportSM.pingActive = false;
		return _transportSM.pingResponse;
	} else {
		TRANSPORT_DEBUG(PSTR("!TSF:PNG:ACTIVE\n"));	// ping active, cannot start new ping
		return INVALID_HOPS;
	}
}

uint32_t transportGetHeartbeat(void)
{
	return transportTimeInState();
}

void transportProcessMessage(void)
{
	// Manage signing timeout
	(void)signerCheckTimer();
	// receive message
	setIndication(INDICATION_RX);
	uint8_t payloadLength = transportReceive((uint8_t *)&_msg);
	// get message length and limit size

	const uint8_t msgLength = min(mGetLength((&_msg)), (uint8_t)MAX_PAYLOAD);
	// calculate expected length
	const uint8_t expectedMessageLength = HEADER_SIZE + (mGetSigned((&_msg)) ? MAX_PAYLOAD : msgLength);
#if defined(MY_RF24_ENABLE_ENCRYPTION)
	// payload length = a multiple of blocksize length for decrypted messages, i.e. cannot be used for payload length check
	payloadLength = expectedMessageLength;
#endif
	const uint8_t command = mGetCommand((&_msg));
	const uint8_t type = _msg.type;
	const uint8_t sender = _msg.sender;
	const uint8_t last = _msg.last;
	const uint8_t destination = _msg.destination;

	TRANSPORT_DEBUG(    PSTR("TSF:MSG:READ,%d-%d-%d,s=%d,c=%d,t=%d,pt=%d,l=%d,sg=%d:%s\n"),
	                    sender, 
                        last, 
                        destination, 
                        _msg.sensor, 
                        command, 
                        type, 
                        mGetPayloadType((&_msg)), 
                        msgLength,
	                    mGetSigned((&_msg)), 
                        MyMessage_getString(&_msg, _convBuf));

	// Reject payloads with incorrect length
	if (payloadLength != expectedMessageLength) {
		setIndication(INDICATION_ERR_LENGTH);
		TRANSPORT_DEBUG(PSTR("!TSF:MSG:LEN,%d!=%d\n"), payloadLength,
		                expectedMessageLength); // invalid payload length
		return;
	}

	// Reject messages with incorrect protocol version
	if (mGetVersion((&_msg)) != PROTOCOL_VERSION) {
		setIndication(INDICATION_ERR_VERSION);
		TRANSPORT_DEBUG(    PSTR("!TSF:MSG:PVER,%d=%d\n"), 
                            mGetVersion((&_msg)),
		                    PROTOCOL_VERSION);	// protocol version mismatch
		return;
	}

	// Reject messages that do not pass verification
	if (!signerVerifyMsg((&_msg))) {
		setIndication(INDICATION_ERR_SIGN);
		TRANSPORT_DEBUG(PSTR("!TSF:MSG:SIGN VERIFY FAIL\n"));
		return;
	}

	// update routing table if msg not from parent
#if defined(MY_REPEATER_FEATURE)
#if !defined(MY_GATEWAY_FEATURE)
	if (last != _transportConfig.parentNodeId) {
#else
	// GW doesn't have parent
	{
#endif
		// Message is from one of the child nodes and not sent from this node. Add it to routing table.
		if (sender != _transportConfig.nodeId)
		{
			transportSetRoute(sender, last);
		}
	}
#endif // MY_REPEATER_FEATURE

	// set message received flag
	_transportSM.msgReceived = true;

	// Is message addressed to this node?
	if (destination == _transportConfig.nodeId) {
		// prevent buffer overflow by limiting max. possible message length (5 bits=31 bytes max) to MAX_PAYLOAD (25 bytes)
		mSetLength((&_msg), min(mGetLength((&_msg)),(uint8_t)MAX_PAYLOAD));
		// null terminate data
		_msg.payload_data.data[msgLength] = 0u;
		// Check if sender requests an ack back.
		if (mGetRequestAck((&_msg))) {
			TRANSPORT_DEBUG(PSTR("TSF:MSG:ACK REQ\n"));	// ACK requested
			_msgTmp = _msg;	// Copy message
			mSetRequestAck((&_msgTmp),
			               false); // Reply without ack flag (otherwise we would end up in an eternal loop)
			mSetAck((&_msgTmp), true); // set ACK flag
			_msgTmp.sender = _transportConfig.nodeId;
			_msgTmp.destination = sender;
			// send ACK, use transportSendRoute since ACK reply is not internal, i.e. if !transportOK do not reply
			(void)transportSendRoute((&_msgTmp));
		}
		if(!mGetAck((&_msg))) {
			// only process if not ACK
			if (command == C_INTERNAL) {
				// Process signing related internal messages
				if (signerProcessInternal((&_msg))) {
					return; // Signer processing indicated no further action needed
				}
#if !defined(MY_GATEWAY_FEATURE)
				if (type == I_ID_RESPONSE) {
#if (MY_NODE_ID == AUTO)
					// only active if node ID dynamic
					(void)transportAssignNodeID(_msg.getByte());
#endif
					return; // no further processing required
				}
				if (type == I_FIND_PARENT_RESPONSE) {
#if !defined(MY_GATEWAY_FEATURE) && !defined(MY_PARENT_NODE_IS_STATIC)
					if (_transportSM.findingParentNode) {	// only process if find parent active
						// Reply to a I_FIND_PARENT_REQUEST message. Check if the distance is shorter than we already have.
						uint8_t distance = _msg.getByte();
						if (isValidDistance(distance)) {
							distance++;	// Distance to gateway is one more for us w.r.t. parent
							// update settings if distance shorter or preferred parent found
							if (((isValidDistance(distance) && distance < _transportConfig.distanceGW) || (!_autoFindParent &&
							        sender == (uint8_t)MY_PARENT_NODE_ID)) && !_transportSM.preferredParentFound) {
								// Found a neighbor closer to GW than previously found
								if (!_autoFindParent && sender == (uint8_t)MY_PARENT_NODE_ID) {
									_transportSM.preferredParentFound = true;
									TRANSPORT_DEBUG(PSTR("TSF:MSG:FPAR PREF\n"));	// find parent, preferred parent found
								}
								_transportConfig.distanceGW = distance;
								_transportConfig.parentNodeId = sender;
								TRANSPORT_DEBUG(PSTR("TSF:MSG:FPAR OK,ID=%d,D=%d\n"), _transportConfig.parentNodeId,
								                _transportConfig.distanceGW);
							}
						}
					} else {
						TRANSPORT_DEBUG(PSTR("!TSF:MSG:FPAR INACTIVE\n"));	// find parent response received, but inactive
					}
					return;
#endif
				}
#endif
				// general
				if (type == I_PING) {
					TRANSPORT_DEBUG(PSTR("TSF:MSG:PINGED,ID=%d,HP=%d\n"), sender,MyMessage_getByte(&_msg)); // node pinged
#if defined(MY_GATEWAY_FEATURE) && (F_CPU>16000000)
					// delay for fast GW and slow nodes
					delay(5);
#endif
                    uint8_t tmp = 1;
                    build(&_msgTmp, sender, NODE_SENSOR_ID, C_INTERNAL, I_PONG, false);
					MyMessage_set(&_msgTmp, &tmp, P_BYTE, 0, 0);
                    (void)transportRouteMessage(&_msgTmp);
					return; // no further processing required
				}
				if (type == I_PONG) {
					if (_transportSM.pingActive) {
						_transportSM.pingActive = false;
						_transportSM.pingResponse = MyMessage_getByte(&_msg);
						TRANSPORT_DEBUG(PSTR("TSF:MSG:PONG RECV,HP=%d\n"), _transportSM.pingResponse); // pong received
					} else {
						TRANSPORT_DEBUG(PSTR("!TSF:MSG:PONG RECV,INACTIVE\n")); // pong received, but !pingActive
					}
					return; // no further processing required
				}
				if (_processInternalMessages()) {
					return; // no further processing required
				}
			} else if (command == C_STREAM) {
#if defined(MY_OTA_FIRMWARE_FEATURE)
				if(firmwareOTAUpdateProcess()) {
					return; // OTA FW update processing indicated no further action needed
				}
#endif
			}
		} else {
			TRANSPORT_DEBUG(
			    PSTR("TSF:MSG:ACK\n")); // received message is ACK, no internal processing, handover to msg callback
		}
#if defined(MY_GATEWAY_FEATURE)
		// Hand over message to controller
		(void)gatewayTransportSend(&_msg);
#endif
		// Call incoming message callback if available
		if (receive) {
			receive(&_msg);
		}
	} else if (destination == BROADCAST_ADDRESS) {
		TRANSPORT_DEBUG(PSTR("TSF:MSG:BC\n"));	// broadcast msg
		if (command == C_INTERNAL) {
			if (isTransportReady()) {
				// only reply if node is fully operational
				if (type == I_FIND_PARENT_REQUEST) {
#if defined(MY_REPEATER_FEATURE)
					if (sender != _transportConfig.parentNodeId) {	// no circular reference
						TRANSPORT_DEBUG(PSTR("TSF:MSG:FPAR REQ,ID=%d\n"), sender);	// FPAR: find parent request
						// check if uplink functional - node can only be parent node if link to GW functional
						// this also prevents circular references in case GW ooo
						if (transportCheckUplink(false)) {
							_transportSM.lastUplinkCheck = MyHwPsoc_hwMillis();
							TRANSPORT_DEBUG(PSTR("TSF:MSG:GWL OK\n")); // GW uplink ok
							// random delay minimizes collisions
							CyDelay(MyHwPsoc_hwMillis() & 0x3ff);
                            build(&_msgTmp, sender, NODE_SENSOR_ID, C_INTERNAL, I_FIND_PARENT_RESPONSE, false);
                            MyMessage_set(&_msgTmp, &_transportConfig.distanceGW, P_BYTE, 0, 0);
							(void)transportRouteMessage(&_msgTmp);
						} else {
							TRANSPORT_DEBUG(PSTR("!TSF:MSG:GWL FAIL\n")); // GW uplink fail, do not respond to parent request
						}
					}
#endif
					return; // no further processing required, do not forward
				}
			} // isTransportReady
			if (type == I_FIND_PARENT_RESPONSE) {
				return;	// no further processing required, do not forward
			}
#if !defined(MY_GATEWAY_FEATURE)
			if (type == I_DISCOVER_REQUEST) {
				if (last == _transportConfig.parentNodeId) {
					// random wait to minimize collisions
					delay(hwMillis() & 0x3ff);
					(void)transportRouteMessage(build(_msgTmp, sender, NODE_SENSOR_ID, C_INTERNAL,
					                                  I_DISCOVER_RESPONSE).set(_transportConfig.parentNodeId));
					// no return here (for fwd if repeater)
				}
			}
#endif
		}
		// controlled BC relay
#if defined(MY_REPEATER_FEATURE)
		// controlled BC repeating: forward only if message received from parent and sender not self to prevent circular fwds
		if(last == _transportConfig.parentNodeId && sender != _transportConfig.nodeId &&
		        isTransportReady()) {
			TRANSPORT_DEBUG(PSTR("TSF:MSG:FWD BC MSG\n")); // controlled broadcast msg forwarding
			(void)transportRouteMessage(&_msg);
		}
#endif

		// Callback for BC, only for non-internal messages
		if (command != C_INTERNAL) {
#if !defined(MY_GATEWAY_FEATURE)
			// only proceed if message received from parent
			if (last != _transportConfig.parentNodeId) {
				return;
			}
#endif
#if defined(MY_GATEWAY_FEATURE)
			// Hand over message to controller
			(void)gatewayTransportSend(&_msg);
#endif
			if (receive) {
				receive(&_msg);
			}
		}

	} else {
		// msg not to us and not BC, relay msg
#if defined(MY_REPEATER_FEATURE)
		if (isTransportReady()) {
			TRANSPORT_DEBUG(PSTR("TSF:MSG:REL MSG\n"));	// relay msg
			if (command == C_INTERNAL) {
				if (type == I_PING || type == I_PONG) {
					uint8_t hopsCnt = MyMessage_getByte(&_msg);
					if (hopsCnt != MAX_HOPS) {
						TRANSPORT_DEBUG(PSTR("TSF:MSG:REL PxNG,HP=%d\n"), hopsCnt);
						hopsCnt++;
                        MyMessage_set(&_msg, &hopsCnt, P_BYTE, 0, 0);
					}
				}
			}
			// Relay this message to another node
			(void)transportRouteMessage(&_msg);
		}
#else
		TRANSPORT_DEBUG(PSTR("!TSF:MSG:REL MSG,NREP\n"));	// message relaying request, but not a repeater
#endif
	}
}

void transportInvokeSanityCheck(void)
{
	if (!transportSanityCheck()) {
		TRANSPORT_DEBUG(PSTR("!TSF:SAN:FAIL\n"));	// sanity check fail
		transportSwitchSM(&stFailure);
	} else {
		TRANSPORT_DEBUG(PSTR("TSF:SAN:OK\n"));		// sanity check ok
	}
}

void transportProcessFIFO(void)
{
	if (!_transportSM.transportActive) {
		// transport not active, no further processing required
		return;
	}

#if defined(MY_TRANSPORT_SANITY_CHECK)
	if (MyHwPsoc_hwMillis() - _lastSanityCheck > MY_TRANSPORT_SANITY_CHECK_INTERVAL_MS) {
		_lastSanityCheck = MyHwPsoc_hwMillis();
		transportInvokeSanityCheck();
	}
#endif

	uint8_t _processedMessages = MAX_SUBSEQ_MSGS;
	// process all msgs in FIFO or counter exit
	while (transportAvailable() && _processedMessages--) {
		transportProcessMessage();
	}
#if defined(MY_OTA_FIRMWARE_FEATURE)
	if (isTransportReady()) {
		// only process if transport ok
		firmwareOTAUpdateRequest();
	}
#endif
}

bool transportSendWrite(const uint8_t to, MyMessage *message)
{
	message->last = _transportConfig.nodeId; // Update last
	// sign message if required
	if (!signerSignMsg(message)) {
		TRANSPORT_DEBUG(PSTR("!TSF:MSG:SIGN FAIL\n"));
		setIndication(INDICATION_ERR_SIGN);
		return false;
	}

	// msg length changes if signed
	const uint8_t totalMsgLength = HEADER_SIZE + ( mGetSigned(message) ? MAX_PAYLOAD : mGetLength(
	                                   message) );

	// send
	setIndication(INDICATION_TX);
	bool result = transportSend(to, &message, min((uint8_t)MAX_MESSAGE_LENGTH, totalMsgLength));
	// broadcasting (workaround counterfeits)
	result |= (to == BROADCAST_ADDRESS);

	TRANSPORT_DEBUG(    PSTR("%sTSF:MSG:SEND,%d-%d-%d-%d,s=%d,c=%d,t=%d,pt=%d,l=%d,sg=%d,ft=%d,st=%s:%s\n"),
                        (result ? "" : "!"), 
                        message->sender, 
                        message->last, 
                        to, 
                        message->destination, 
                        message->sensor,
	                    mGetCommand(message), 
                        message->type,
	                    mGetPayloadType(message), 
                        mGetLength(message), 
                        mGetSigned(message),
	                    _transportSM.failedUplinkTransmissions, 
                        (result ? "OK" : "NACK"), 
                        MyMessage_getString(message, _convBuf));

	return result;
}

void transportRegisterReadyCallback(transportCallback_t cb)
{
	transportReady_cb = cb;
}

uint8_t transportGetNodeId(void)
{
	return _transportConfig.nodeId;
}
uint8_t transportGetParentNodeId(void)
{
	return _transportConfig.parentNodeId;
}
uint8_t transportGetDistanceGW(void)
{
	return _transportConfig.distanceGW;
}


void transportClearRoutingTable(void)
{
    uint16_t i=0;
	for (i = 0; i < SIZE_ROUTES; i++) {
		transportSetRoute((uint8_t)i, BROADCAST_ADDRESS);
	}
	transportSaveRoutingTable();	// save cleared routing table to EEPROM (if feature enabled)
	TRANSPORT_DEBUG(PSTR("TSF:CRT:OK\n"));	// clear routing table
}

void transportLoadRoutingTable(void)
{
#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	hwReadConfigBlock((void*)&_transportRoutingTable.route, (void*)EEPROM_ROUTES_ADDRESS, SIZE_ROUTES);
	TRANSPORT_DEBUG(PSTR("TSF:LRT:OK\n"));	//  load routing table
#endif
}

void transportSaveRoutingTable(void)
{
#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	hwWriteConfigBlock((void*)&_transportRoutingTable.route, (void*)EEPROM_ROUTES_ADDRESS, SIZE_ROUTES);
	TRANSPORT_DEBUG(PSTR("TSF:SRT:OK\n"));	//  save routing table
#endif
}

void transportSetRoute(const uint8_t node, const uint8_t route)
{
#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	_transportRoutingTable.route[node] = route;
#else
	MyHwPsoc_WriteConfig(EEPROM_ROUTES_ADDRESS + node, route);
#endif
}

uint8_t transportGetRoute(const uint8_t node)
{
	uint8_t result;
#if defined(MY_RAM_ROUTING_TABLE_ENABLED)
	result = _transportRoutingTable.route[node];
#else
	result = MyHwPsoc_ReadConfig(EEPROM_ROUTES_ADDRESS + node);
#endif
	return result;
}

#if defined(MY_RF24_ENABLE_ENCRYPTION)
#include "drivers/AES/AES.h"
#endif

#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
typedef struct _transportQueuedMessage {
	uint8_t m_len;                        // Length of the data
	uint8_t m_data[MAX_MESSAGE_LENGTH];   // The raw data
} transportQueuedMessage;

/** Buffer to store queued messages in. */
static transportQueuedMessage transportRxQueueStorage[MY_RX_MESSAGE_BUFFER_SIZE];
/** Circular buffer, which uses the transportRxQueueStorage and administers stored messages. */
static CircularBuffer<transportQueuedMessage> transportRxQueue(transportRxQueueStorage,
        MY_RX_MESSAGE_BUFFER_SIZE);

static volatile uint8_t transportLostMessageCount = 0;

static void transportRxCallback(void)
{
	// Called for each message received by radio, from interrupt context.
	// This function _must_ call RF24_readMessage() to de-assert interrupt line!
	if (!transportRxQueue.full()) {
		transportQueuedMessage* msg = transportRxQueue.getFront();
		msg->m_len = RF24_readMessage(msg->m_data);		// Read payload & clear RX_DR
		(void)transportRxQueue.pushFront(msg);
	} else {
		// Queue is full. Discard message.
		(void)RF24_readMessage(NULL);		// Read payload & clear RX_DR
		// Keep track of messages lost. Max 255, prevent wrapping.
		if (transportLostMessageCount < 255) {
			++transportLostMessageCount;
		}
	}
}
#endif

#if defined(MY_RF24_ENABLE_ENCRYPTION)
AES _aes;
uint8_t _dataenc[32] = {0};
uint8_t _psk[16];
#endif
