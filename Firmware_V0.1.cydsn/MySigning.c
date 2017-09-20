/*
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

#include "MySigning.h"
#include "sha256.h"


void signerInit(void)
{
#if defined(MY_SIGNING_FEATURE)
	// Read out the signing requirements from EEPROM
	MyHwPsoc_ReadConfigBlock((void*)_doSign, (void*)EEPROM_SIGNING_REQUIREMENT_TABLE_ADDRESS, sizeof(_doSign));

	// Read out the whitelist requirements from EEPROM
	MyHwPsoc_ReadConfigBlock((void*)_doWhitelist, (void*)EEPROM_WHITELIST_REQUIREMENT_TABLE_ADDRESS, sizeof(_doWhitelist));

	signerBackendInit();
#endif
}

void signerPresentation(MyMessage *msg, uint8_t destination)
{
	prepareSigningPresentation(msg, destination);

#if defined(MY_SIGNING_REQUEST_SIGNATURES)
	msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_SIGNATURES;
	SIGN_DEBUG(PSTR("Signing required\n"));
#endif
#if defined(MY_SIGNING_NODE_WHITELISTING)
	msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_WHITELISTING;
	SIGN_DEBUG(PSTR("Whitelisting required\n"));
#endif

	if (!_sendRoute(msg)) {
		SIGN_DEBUG(PSTR("Failed to transmit signing presentation!\n"));
	}

	if (destination == GATEWAY_ADDRESS) {
		SIGN_DEBUG(PSTR("Waiting for GW to send signing preferences...\n"));
		wait_cmd(2000, C_INTERNAL, I_SIGNING_PRESENTATION);
	}
}

bool signerProcessInternal(MyMessage *msg)
{
	bool ret;
	switch (msg->type) {
	case I_SIGNING_PRESENTATION:
		ret = signerInternalProcessPresentation(msg);
		break;
	case I_NONCE_REQUEST:
		ret = signerInternalProcessNonceRequest(msg);
		break;
	case I_NONCE_RESPONSE:
		ret = signerInternalProcessNonceResponse(msg);
		break;
	default:
		ret = false; // Let the transport process this message further as it is not related to signing
		break;
	}
	return ret;
}

bool signerCheckTimer(void)
{
	return signerBackendCheckTimer();
}

bool signerSignMsg(MyMessage *msg)
{
	bool ret;
#if defined(MY_SIGNING_FEATURE)
	// If destination is known to require signed messages and we are the sender,
	// sign this message unless it is identified as an exception
	if (DO_SIGN(msg->destination) && msg->sender == getNodeId()) {
		if (skipSign(msg)) {
			ret = true;
		} else {
			// Send nonce-request
			_signingNonceStatus=SIGN_WAITING_FOR_NONCE;
            build(&_msgSign, msg->destination, msg->sensor, C_INTERNAL,I_NONCE_REQUEST,false);
            MyMessage_set(&_msgSign, "", P_STRING, 0, 0); 
			if (!_sendRoute(&_msgSign)) {
				SIGN_DEBUG(PSTR("Failed to transmit nonce request!\n"));
				ret = false;
			} else {
				SIGN_DEBUG(PSTR("Nonce requested from %d. Waiting...\n"), msg->destination);
				// We have to wait for the nonce to arrive before we can sign our original message
				// Other messages could come in-between. We trust _process() takes care of them
                
				unsigned long enter = MyHwPsoc_hwMillis();
                /* Copy first struct values into the second one */
                memcpy(msg, &_msgSign, sizeof *msg);
               
				while (MyHwPsoc_hwMillis() - enter < MY_VERIFICATION_TIMEOUT_MS &&
				        _signingNonceStatus==SIGN_WAITING_FOR_NONCE) {
					_process();
				}
				if (MyHwPsoc_hwMillis() - enter > MY_VERIFICATION_TIMEOUT_MS) {
					SIGN_DEBUG(PSTR("Timeout waiting for nonce!\n"));
					ret = false;
				} else {
					if (_signingNonceStatus == SIGN_OK) {
						// process() received a nonce and signerProcessInternal successfully signed the message
						 /* Copy first struct values into the second one */
                        memcpy(&_msgSign, msg, sizeof(_msgSign));
						SIGN_DEBUG(PSTR("Message to send has been signed\n"));
						ret = true;
						// After this point, only the 'last' member of the message structure is allowed to be altered if the
						// message has been signed, or signature will become invalid and the message rejected by the receiver
					} else {
						SIGN_DEBUG(PSTR("Message to send could not be signed!\n"));
						ret = false;
					}
				}
			}
		}
	} else if (getNodeId() == msg->sender) {
		mSetSigned(msg, 0); // Message is not supposed to be signed, make sure it is marked unsigned
		SIGN_DEBUG(PSTR("Will not sign message for destination %d as it does not require it\n"),
		           msg->destination);
		ret = true;
	} else {
		SIGN_DEBUG(PSTR("Will not sign message since it was from %d and we are %d\n"), msg->sender,
		           getNodeId());
		ret = true;
	}
#else
	(void)msg;
	ret = true;
#endif // MY_SIGNING_FEATURE
	return ret;
}

bool signerVerifyMsg(MyMessage *msg)
{
	bool verificationResult = true;
	// Before processing message, reject unsigned messages if signing is required and check signature
	// (if it is signed and addressed to us)
	// Note that we do not care at all about any signature found if we do not require signing
#if defined(MY_SIGNING_FEATURE) && defined(MY_SIGNING_REQUEST_SIGNATURES)
	// If we are a node, or we are a gateway and the sender require signatures (or just a strict gw)
	// and we are the destination...
#if defined(MY_SIGNING_GW_REQUEST_SIGNATURES_FROM_ALL)
	if (msg->destination == getNodeId()) {
#else
	if ((!MY_IS_GATEWAY || DO_SIGN(msg->sender)) && msg->destination == getNodeId()) {
#endif
		// Internal messages of certain types are not verified
		if (skipSign(msg)) {
			verificationResult = true;
		} else if (!mGetSigned(msg)) {
			// Got unsigned message that should have been signed
			SIGN_DEBUG(PSTR("Message is not signed, but it should have been!\n"));
			verificationResult = false;
		} else {
			if (!signerBackendVerifyMsg(msg)) {
				SIGN_DEBUG(PSTR("Signature verification failed!\n"));
				verificationResult = false;
			}
#if defined(MY_NODE_LOCK_FEATURE)
			if (verificationResult) {
				// On successful verification, clear lock counters
				nof_nonce_requests = 0;
				nof_failed_verifications = 0;
			} else {
				nof_failed_verifications++;
				SIGN_DEBUG(PSTR("Failed verification attempts left until lockdown: %d\n"),
				           MY_NODE_LOCK_COUNTER_MAX-nof_failed_verifications);
				if (nof_failed_verifications >= MY_NODE_LOCK_COUNTER_MAX) {
					_nodeLock("TMFV"); //Too many failed verifications
				}
			}
#endif
			mSetSigned(msg,0); // Clear the sign-flag now as verification is completed
		}
	}
#else
	(void)msg;
#endif // MY_SIGNING_REQUEST_SIGNATURES
	return verificationResult;
}

static uint8_t sha256_hash[32];
//Sha256Class _soft_sha256;

void signerSha256Init(void)
{
	memset(sha256_hash, 0, 32);
	sha256_init();
}

void signerSha256Update(const uint8_t* data, size_t sz)
{
	size_t i = 0;
    for (i = 0; i < sz; i++) {
		sha256_write(data[i]);
	}
}

uint8_t* signerSha256Final(void)
{
	memcpy(sha256_hash, sha256_result(), 32);
	return sha256_hash;
}

int signerMemcmp(const void* a, const void* b, size_t sz)
{
	int retVal;
	size_t i;
	int done = 0;
	const uint8_t* ptrA = (const uint8_t*)a;
	const uint8_t* ptrB = (const uint8_t*)b;
	for (i=0; i < sz; i++) {
		if (ptrA[i] == ptrB[i]) {
			if (done > 0) {
				done = 1;
			} else {
				done = 0;
			}
		}	else {
			if (done > 0) {
				done = 2;
			} else {
				done = 3;
			}
		}
	}
	if (done > 0) {
		retVal = -1;
	} else {
		retVal = 0;
	}
	return retVal;
}

#if defined(MY_SIGNING_FEATURE)
// Helper function to centralize signing/verification exceptions
bool skipSign(MyMessage *msg)
{
	bool ret;
	if (mGetAck(msg)) {
		SIGN_DEBUG(PSTR("Skipping security for ACK on command %d type %d\n"), mGetCommand(msg), msg->type);
		ret = true;
	}	else if (mGetCommand(msg) == C_INTERNAL &&
	             (msg->type == I_NONCE_REQUEST				|| msg->type == I_NONCE_RESPONSE        ||
	              msg->type == I_SIGNING_PRESENTATION	    ||
	              msg->type == I_ID_REQUEST					|| msg->type == I_ID_RESPONSE		    ||
	              msg->type == I_FIND_PARENT_REQUEST		|| msg->type == I_FIND_PARENT_RESPONSE	||
	              msg->type == I_HEARTBEAT_REQUEST			|| msg->type == I_HEARTBEAT_RESPONSE	||
	              msg->type == I_PING						|| msg->type == I_PONG		            ||
	              msg->type == I_REGISTRATION_REQUEST	    || msg->type == I_DISCOVER_REQUEST	    ||
	              msg->type == I_DISCOVER_RESPONSE )) {
		SIGN_DEBUG(PSTR("Skipping security for command %d type %d\n"), mGetCommand(msg), msg->type);
		ret = true;
	} else if (mGetCommand(msg) == C_STREAM &&
	           (msg->type == ST_FIRMWARE_REQUEST || msg->type == ST_FIRMWARE_RESPONSE ||
	            msg->type == ST_SOUND || msg->type == ST_IMAGE)) {
		SIGN_DEBUG(PSTR("Skipping security for command %d type %d\n"), mGetCommand(msg), msg->type);
		ret = true;
	} else {
		ret = false;
	}
	return ret;
}
#endif

// Helper to prepare a signing presentation message
void prepareSigningPresentation(MyMessage *msg, uint8_t destination)
{
	// Only supports version 1 for now
	build(msg, destination, NODE_SENSOR_ID, C_INTERNAL, I_SIGNING_PRESENTATION,false);
    MyMessage_set(msg, "", P_STRING, 0, 0);
	mSetLength(msg, 2);
	mSetPayloadType(msg, P_CUSTOM);		// displayed as hex
	msg->payload_data.data[0] = SIGNING_PRESENTATION_VERSION_1;
	msg->payload_data.data[1] = 0;
}

// Helper to process presentation mesages
bool signerInternalProcessPresentation(MyMessage *msg)
{
	const uint8_t sender = msg->sender;
#if defined(MY_SIGNING_FEATURE)
	if (msg->payload_data.data[0] != SIGNING_PRESENTATION_VERSION_1) {
		SIGN_DEBUG(PSTR("Unsupported signing presentation version (%d)!\n"), msg->payload_data.data[0]);
		return true; // Just drop this presentation message
	}
	// We only handle version 1 here...
	if (msg->payload_data.data[1] & SIGNING_PRESENTATION_REQUIRE_SIGNATURES) {
		// We received an indicator that the sender require us to sign all messages we send to it
		SIGN_DEBUG(PSTR("Mark node %d as one that require signed messages\n"), sender);
		SET_SIGN(sender);
	} else {
		// We received an indicator that the sender does not require us to sign all messages we send to it
		SIGN_DEBUG(PSTR("Mark node %d as one that do not require signed messages\n"), sender);
		CLEAR_SIGN(sender);
	}
	if (msg->payload_data.data[1] & SIGNING_PRESENTATION_REQUIRE_WHITELISTING) {
		// We received an indicator that the sender require us to salt signatures with serial
		SIGN_DEBUG(PSTR("Mark node %d as one that require whitelisting\n"), sender);
		SET_WHITELIST(sender);
	} else {
		// We received an indicator that the sender does not require us to sign all messages we send to it
		SIGN_DEBUG(PSTR("Mark node %d as one that do not require whitelisting\n"), sender);
		CLEAR_WHITELIST(sender);
	}

	// Save updated tables
	MyHwPsoc_WriteConfigBlock((void*)_doSign, (void*)EEPROM_SIGNING_REQUIREMENT_TABLE_ADDRESS,
	                   sizeof(_doSign));
	MyHwPsoc_WriteConfigBlock((void*)_doWhitelist, (void*)EEPROM_WHITELIST_REQUIREMENT_TABLE_ADDRESS,
	                   sizeof(_doWhitelist));

	// Inform sender about our preference if we are a gateway, but only require signing if the sender
	// required signing unless we explicitly configure it to
#if defined(MY_GATEWAY_FEATURE)
	prepareSigningPresentation(msg, sender);
#if defined(MY_SIGNING_REQUEST_SIGNATURES)
#if defined(MY_SIGNING_GW_REQUEST_SIGNATURES_FROM_ALL)
	msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_SIGNATURES;
#else
	if (DO_SIGN(sender)) {
		msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_SIGNATURES;
	}
#endif
#endif // MY_SIGNING_REQUEST_SIGNATURES
#if defined(MY_SIGNING_NODE_WHITELISTING)
#if defined(MY_SIGNING_GW_REQUEST_SIGNATURES_FROM_ALL)
	msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_WHITELISTING;
#else
	if (DO_WHITELIST(sender)) {
		msg->payload_data.data[1] |= SIGNING_PRESENTATION_REQUIRE_WHITELISTING;
	}
#endif
#endif // MY_SIGNING_NODE_WHITELISTING
	if (msg->payload_data.data[1] & SIGNING_PRESENTATION_REQUIRE_SIGNATURES) {
		SIGN_DEBUG(PSTR("Informing node %d that we require signatures\n"), sender);
	} else {
		SIGN_DEBUG(PSTR("Informing node %d that we do not require signatures\n"), sender);
	}
	if (msg->payload_data.data[1] & SIGNING_PRESENTATION_REQUIRE_WHITELISTING) {
		SIGN_DEBUG(PSTR("Informing node %d that we require whitelisting\n"), sender);
	} else {
		SIGN_DEBUG(PSTR("Informing node %d that we do not require whitelisting\n"), sender);
	}
	if (!_sendRoute(msg)) {
		SIGN_DEBUG(PSTR("Failed to transmit signing presentation!\n"));
	}
#endif // MY_GATEWAY_FEATURE
#else // not MY_SIGNING_FEATURE
#if defined(MY_GATEWAY_FEATURE)
	// If we act as gateway and do not have the signing feature and receive a signing request we still
	// need to do make sure the requester does not believe the gateway still require signatures
	prepareSigningPresentation(msg, sender);
	SIGN_DEBUG(
	    PSTR("Informing node %d that we do not require signatures because we do not support it\n"),
	    sender);
	if (!_sendRoute(msg)) {
		SIGN_DEBUG(PSTR("Failed to transmit signing presentation!\n"));
	}
#else // not MY_GATEWAY_FEATURE
	// If we act as a node and do not have the signing feature then we just silently drop any signing
	// presentation messages received
	(void)msg;
	(void)sender;
	SIGN_DEBUG(PSTR("Received signing presentation, but signing is not supported (message ignored)\n"));
#endif // not MY_GATEWAY_FEATURE
#endif // not MY_SIGNING_FEATURE 
	return true; // No need to further process I_SIGNING_PRESENTATION
}

// Helper to process nonce request mesages
bool signerInternalProcessNonceRequest(MyMessage *msg)
{
#if defined(MY_SIGNING_FEATURE)
#if defined(MY_NODE_LOCK_FEATURE)
	nof_nonce_requests++;
	SIGN_DEBUG(PSTR("Nonce requests left until lockdown: %d\n"),
	           MY_NODE_LOCK_COUNTER_MAX-nof_nonce_requests);
	if (nof_nonce_requests >= MY_NODE_LOCK_COUNTER_MAX) {
		_nodeLock("TMNR"); //Too many nonces requested
	}
#endif // MY_NODE_LOCK_FEATURE
	if (signerBackendGetNonce(msg)) {
        build(msg, msg->sender, NODE_SENSOR_ID, C_INTERNAL, I_NONCE_RESPONSE, false);
		if (!_sendRoute(msg)) {
			SIGN_DEBUG(PSTR("Failed to transmit nonce!\n"));
		} else {
			SIGN_DEBUG(PSTR("Transmitted nonce\n"));
		}
	} else {
		SIGN_DEBUG(PSTR("Failed to generate nonce!\n"));
	}
#else // not MY_SIGNING_FEATURE
	(void)msg;
	SIGN_DEBUG(PSTR("Received nonce request, but signing is not supported (message ignored)\n"));
#endif // MY_SIGNING_FEATURE
	return true; // No need to further process I_NONCE_REQUEST
}

// Helper to process nonce response mesages
bool signerInternalProcessNonceResponse(MyMessage *msg)
{
#if defined(MY_SIGNING_FEATURE)
	// Proceed with signing if nonce has been received
	SIGN_DEBUG(PSTR("Nonce received from %d.\n"), msg->sender);
	if (msg->sender != _msgSign.destination) {
		SIGN_DEBUG(PSTR("Nonce did not come from the destination (%d) of the message to be signed! "
		                "It came from %d.\n"), _msgSign.destination, msg->sender);
		SIGN_DEBUG(PSTR("Silently discarding this nonce\n"));
	} else {
		SIGN_DEBUG(PSTR("Proceeding with signing...\n"));
		signerBackendPutNonce(msg);
		if (!signerBackendSignMsg(&_msgSign)) {
			SIGN_DEBUG(PSTR("Failed to sign message!\n"));
		} else {
			SIGN_DEBUG(PSTR("Message signed\n"));
			_signingNonceStatus = SIGN_OK; // _msgSign now contains the signed message pending transmission
		}
	}
#else
	(void)msg;
	SIGN_DEBUG(PSTR("Received nonce response, but signing is not supported (message ignored)\n"));
#endif
	return true; // No need to further process I_NONCE_RESPONSE
}
