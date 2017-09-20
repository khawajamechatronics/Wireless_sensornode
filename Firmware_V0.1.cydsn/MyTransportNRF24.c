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
#include "MyTransportNRF24.h"

uint8_t MY_RF24_BASE_ADDR[MY_RF24_ADDR_WIDTH] = { MY_RF24_BASE_RADIO_ID };
uint8_t MY_RF24_NODE_ADDRESS = AUTO;

#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
  RF24_receiveCallbackType RF24_receiveCallback = NULL;
#endif

#ifdef LINUX_SPI_BCM
uint8_t spi_rxbuff[32+1] ; //SPI receive buffer (payload max 32 bytes)
uint8_t spi_txbuff[32+1] ; //SPI transmit buffer (payload max 32 bytes + 1 byte for the command)
#endif


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

bool transportInit(void)
{
#if defined(MY_RF24_ENABLE_ENCRYPTION)
	hwReadConfigBlock((void*)_psk, (void*)EEPROM_RF_ENCRYPTION_AES_KEY_ADDRESS, 16);
	//set up AES-key
	_aes.set_key(_psk, 16);
	// Make sure it is purged from memory when set
	memset(_psk, 0, 16);
#endif

#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
	RF24_registerReceiveCallback( transportRxCallback );
#endif
	return RF24_initialize();
}

void transportSetAddress(const uint8_t address)
{
	RF24_setNodeAddress(address);
	RF24_startListening();
}

uint8_t transportGetAddress(void)
{
	return RF24_getNodeID();
}

bool transportSend(const uint8_t to, const void* data, const uint8_t len)
{
#if defined(MY_RF24_ENABLE_ENCRYPTION)
	// copy input data because it is read-only
	(void)memcpy(_dataenc,data,len);
	// has to be adjusted, WIP!
	_aes.set_IV(0);
	const uint8_t finalLength = len > 16 ? 32 : 16;
	//encrypt data
	_aes.cbc_encrypt(_dataenc, _dataenc, finalLength /16);
	return RF24_sendMessage(to, _dataenc, finalLength);
#else
	return RF24_sendMessage(to, data, len);
#endif
}

bool transportAvailable(void)
{
#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
	(void)RF24_isDataAvailable;				// Prevent 'defined but not used' warning
	return !transportRxQueue.empty();
#else
	return RF24_isDataAvailable();
#endif
}

bool transportSanityCheck(void)
{
	return RF24_sanityCheck();
}

uint8_t transportReceive(void* data)
{
	uint8_t len = 0;
#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
	transportQueuedMessage* msg = transportRxQueue.getBack();
	if (msg) {
		len = msg->m_len;
		(void)memcpy(data, msg->m_data, len);
		(void)transportRxQueue.popBack();
	}
#else
	len = RF24_readMessage(data);
#endif
#if defined(MY_RF24_ENABLE_ENCRYPTION)
	// has to be adjusted, WIP!
	_aes.set_IV(0);
	// decrypt data
	if (_aes.cbc_decrypt((uint8_t*)(data), (uint8_t*)(data), len > 16 ? 2 : 1) != AES_SUCCESS) {
		len = 0;
	}
#endif
	return len;
}

void transportPowerDown(void)
{
	RF24_powerDown();
}

  void RF24_csn(const bool level)
{
    MyHwPsoc_SetOutput(NRF24_CSN, level);
}

  void RF24_ce(const bool level)
{
	MyHwPsoc_SetOutput(NRF24_CE, level);
}

  uint8_t RF24_spiMultiByteTransfer(const uint8_t cmd, uint8_t* buf, uint8_t len,
                                        const bool aReadMode)
{
	uint8_t status;
	uint8_t* current = buf;
    
    RF24_csn(false);
	// timing
    CyDelayUs(10);
	status = MyHwPsoc_SpiTransfer(cmd);
	while ( len-- ) {
		if (aReadMode) {
			status = MyHwPsoc_SpiTransfer(RF24_NOP);
			if (buf != NULL) {
				*current++ = status;
			}
		} else {
			status = MyHwPsoc_SpiTransfer(*current++);
		}
	}
	RF24_csn(HIGH);
    
#if !defined(MY_SOFTSPI)
	MyHwPsoc_SpiEndTransaction();
#endif
	// timing
	CyDelayUs(10);
	return status;
}

  uint8_t RF24_spiByteTransfer(const uint8_t cmd)
{
	return RF24_spiMultiByteTransfer(cmd, NULL, 0, false);
}

  uint8_t RF24_RAW_readByteRegister(const uint8_t cmd)
{
	const uint8_t value = RF24_spiMultiByteTransfer(cmd, NULL, 1, true);
	RF24_DEBUG(PSTR("RF24:read register, reg=%d, value=%d\n"), cmd & RF24_REGISTER_MASK, value);
	return value;
}

  uint8_t RF24_RAW_writeByteRegister(const uint8_t cmd, uint8_t value)
{
	RF24_DEBUG(PSTR("RF24:write register, reg=%d, value=%d\n"), cmd & RF24_REGISTER_MASK, value);
	return RF24_spiMultiByteTransfer( cmd , &value, 1, false);
}

  void RF24_flushRX(void)
{
	RF24_DEBUG(PSTR("RF24:flushRX\n"));
	RF24_spiByteTransfer(RF24_FLUSH_RX);
}

  void RF24_flushTX(void)
{
	RF24_DEBUG(PSTR("RF24:flushTX\n"));
	RF24_spiByteTransfer(RF24_FLUSH_TX);
}

  uint8_t RF24_getStatus(void)
{
	return RF24_spiByteTransfer(RF24_NOP);
}

  uint8_t RF24_getFIFOStatus(void)
{
	return RF24_readByteRegister(RF24_FIFO_STATUS);
}

  void RF24_setChannel(const uint8_t channel)
{
	RF24_writeByteRegister(RF24_RF_CH,channel);
}

  void RF24_setRetries(const uint8_t retransmitDelay, const uint8_t retransmitCount)
{
	RF24_writeByteRegister(RF24_SETUP_RETR, retransmitDelay << RF24_ARD | retransmitCount << RF24_ARC);
}

  void RF24_setAddressWidth(const uint8_t width)
{
	RF24_writeByteRegister(RF24_SETUP_AW, width - 2);
}

  void RF24_setRFSetup(const uint8_t RFsetup)
{
	RF24_writeByteRegister(RF24_RF_SETUP, RFsetup);
}

  void RF24_setFeature(const uint8_t feature)
{
	RF24_writeByteRegister(RF24_FEATURE, feature);
}

  void RF24_setPipe(const uint8_t pipe)
{
	RF24_writeByteRegister(RF24_EN_RXADDR, pipe);
}

  void RF24_setAutoACK(const uint8_t pipe)
{
	RF24_writeByteRegister(RF24_EN_AA, pipe);
}

  void RF24_setDynamicPayload(const uint8_t pipe)
{
	RF24_writeByteRegister(RF24_DYNPD, pipe);
}

  void RF24_setRFConfiguration(const uint8_t configuration)
{
	RF24_writeByteRegister(RF24_NRF_CONFIG, configuration);
}

  void RF24_setPipeAddress(const uint8_t pipe, uint8_t* address, const uint8_t width)
{
	RF24_writeMultiByteRegister(pipe, address, width);
}

  void RF24_setPipeLSB(const uint8_t pipe, const uint8_t LSB)
{
	RF24_writeByteRegister(pipe, LSB);
}

  uint8_t RF24_getObserveTX(void)
{
	return RF24_readByteRegister(RF24_OBSERVE_TX);
}

  void RF24_setStatus(const uint8_t status)
{
	RF24_writeByteRegister(RF24_STATUS, status);
}
  void RF24_enableFeatures(void)
{
	RF24_RAW_writeByteRegister(RF24_ACTIVATE, 0x73);
}

  void RF24_openWritingPipe(const uint8_t recipient)
{
	RF24_DEBUG(PSTR("RF24:OPEN WPIPE,RCPT=%d\n"), recipient); // open writing pipe
	// only write LSB of RX0 and TX pipe
	RF24_setPipeLSB(RF24_RX_ADDR_P0, recipient);
	RF24_setPipeLSB(RF24_TX_ADDR, recipient);
}

  void RF24_startListening(void)
{
	RF24_DEBUG(PSTR("RF24:STRT LIS\n"));	// start listening
	// toggle PRX
	RF24_setRFConfiguration(MY_RF24_CONFIGURATION | _BV(RF24_PWR_UP) | _BV(RF24_PRIM_RX) );
	// all RX pipe addresses must be unique, therefore skip if node ID is AUTO
	if(MY_RF24_NODE_ADDRESS!=AUTO) {
		RF24_setPipeLSB(RF24_RX_ADDR_P0, MY_RF24_NODE_ADDRESS);
	}
	// start listening
	RF24_ce(HIGH);
}

  void RF24_stopListening(void)
{
	RF24_DEBUG(PSTR("RF24:STP LIS\n"));	// stop listening
	RF24_ce(LOW);
	// timing
	CyDelayUs(130);
	RF24_setRFConfiguration(MY_RF24_CONFIGURATION | _BV(RF24_PWR_UP) );
	// timing
	CyDelayUs(100);
}

  void RF24_powerDown(void)
{
	RF24_ce(LOW);
	RF24_setRFConfiguration(MY_RF24_CONFIGURATION);
	RF24_DEBUG(PSTR("RF24:PD\n")); // power down
}

  bool RF24_sendMessage(const uint8_t recipient, const void* buf, const uint8_t len)
{
	uint8_t RF24_status;
	RF24_stopListening();
	RF24_openWritingPipe( recipient );
	RF24_DEBUG(PSTR("RF24:SND:TO=%d,LEN=%d\n"),recipient,len); // send message
	// flush TX FIFO
	RF24_flushTX();
	// this command is affected in clones (e.g. Si24R1):  flipped NoACK bit when using W_TX_PAYLOAD_NO_ACK / W_TX_PAYLOAD
	// AutoACK is disabled on the broadcasting pipe - NO_ACK prevents resending
	RF24_spiMultiByteTransfer(recipient == BROADCAST_ADDRESS ? RF24_WRITE_TX_PAYLOAD_NO_ACK :
	                          RF24_WRITE_TX_PAYLOAD, (uint8_t*)buf, len, false );
	// go, TX starts after ~10us
	RF24_ce(HIGH);
	// timeout counter to detect HW issues
	uint16_t timeout = 0xFFFF;
	do {
		RF24_status = RF24_getStatus();
	} while  (!(RF24_status & ( _BV(RF24_MAX_RT) | _BV(RF24_TX_DS) )) && timeout--);
	// timeout value after successful TX on 16Mhz AVR ~ 65500, i.e. msg is transmitted after ~36 loop cycles
	RF24_ce(LOW);
	// reset interrupts
	RF24_setStatus(_BV(RF24_TX_DS) | _BV(RF24_MAX_RT) );
	// Max retries exceeded
	if(RF24_status & _BV(RF24_MAX_RT)) {
		// flush packet
		RF24_DEBUG(PSTR("!RF24:SND:MAX_RT\n"));	// max retries, no ACK
		RF24_flushTX();
	}
	RF24_startListening();
	// true if message sent
	return (RF24_status & _BV(RF24_TX_DS));
}

  uint8_t RF24_getDynamicPayloadSize(void)
{
	uint8_t result = RF24_spiMultiByteTransfer(RF24_READ_RX_PL_WID, NULL, 1, true);
	// check if payload size invalid
	if(result > 32) {
		RF24_DEBUG(PSTR("!RF24:GDP:PAYL LEN INVALID=%d\n"),result); // payload len invalid
		RF24_flushRX();
		result = 0;
	}
	return result;
}

  bool RF24_isDataAvailable(void)
{
	return (!(RF24_getFIFOStatus() & _BV(0)) );
}


  uint8_t RF24_readMessage(void* buf)
{
	const uint8_t len = RF24_getDynamicPayloadSize();
	RF24_DEBUG(PSTR("RF24:RDM:MSG LEN=%d\n"), len);	// read message
	RF24_spiMultiByteTransfer(RF24_READ_RX_PAYLOAD,(uint8_t*)buf,len,true);
	// clear RX interrupt
	RF24_setStatus(_BV(RF24_RX_DR));
	return len;
}

  void RF24_setNodeAddress(const uint8_t address)
{
	if(address!=AUTO) {
		MY_RF24_NODE_ADDRESS = address;
		// enable node pipe
		RF24_setPipe(_BV(RF24_ERX_P0 + RF24_BROADCAST_PIPE) | _BV(RF24_ERX_P0));
		// enable autoACK on pipe 0
		RF24_setAutoACK(_BV(RF24_ENAA_P0));
	}
}

  uint8_t RF24_getNodeID(void)
{
	return MY_RF24_NODE_ADDRESS;
}

  bool RF24_sanityCheck(void)
{
	// detect HW defect, configuration errors or interrupted SPI line, CE disconnect cannot be detected
	return (RF24_readByteRegister(RF24_RF_SETUP) == MY_RF24_RF_SETUP) & (RF24_readByteRegister(
	            RF24_RF_CH) == MY_RF24_CHANNEL);
}

#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
  void RF24_irqHandler(void)
{
	if (RF24_receiveCallback) {
		// Will stay for a while (several 100us) in this interrupt handler. Any interrupts from serial
		// rx coming in during our stay will not be handled and will cause characters to be lost.
		// As a workaround we re-enable interrupts to allow nested processing of other interrupts.
		// Our own handler is disconnected to prevent recursive calling of this handler.
#if defined(MY_GATEWAY_SERIAL) && !defined(__linux__)
		detachInterrupt(digitalPinToInterrupt(MY_RF24_IRQ_PIN));
		interrupts();
#endif
		// Read FIFO until empty.
		// Procedure acc. to datasheet (pg. 63):
		// 1.Read payload, 2.Clear RX_DR IRQ, 3.Read FIFO_status, 4.Repeat when more data available.
		// Datasheet (ch. 8.5) states, that the nRF de-asserts IRQ after reading STATUS.

		// Start checking if RX-FIFO is not empty, as we might end up here from an interrupt
		// for a message we've already read.
		while (RF24_isDataAvailable()) {
			RF24_receiveCallback();		// Must call RF24_readMessage(), which will clear RX_DR IRQ !
		}
		// Restore our interrupt handler.
#if defined(MY_GATEWAY_SERIAL) && !defined(__linux__)
		noInterrupts();
		attachInterrupt(digitalPinToInterrupt(MY_RF24_IRQ_PIN), RF24_irqHandler, FALLING);
#endif
	} else {
		// clear RX interrupt
		RF24_setStatus(_BV(RF24_RX_DR));
	}
}

LOCAL void RF24_registerReceiveCallback(RF24_receiveCallbackType cb)
{
	MY_CRITICAL_SECTION {
		RF24_receiveCallback = cb;
	}
}
#endif

bool RF24_initialize(void)
{
	// prevent warning
	(void)RF24_getObserveTX;

	// Initialize pins
	//hwPinMode(MY_RF24_CE_PIN,OUTPUT);
	//hwPinMode(MY_RF24_CS_PIN,OUTPUT);
#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
	hwPinMode(MY_RF24_IRQ_PIN,INPUT);
#endif
	// Initialize SPI
	RF24_ce(LOW);
	RF24_csn(HIGH);
#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
	// assure SPI can be used from interrupt context
	// Note: ESP8266 & SoftSPI currently do not support interrupt usage for SPI,
	// therefore it is unsafe to use MY_RF24_IRQ_PIN with ESP8266/SoftSPI!
	_SPI.usingInterrupt(digitalPinToInterrupt(MY_RF24_IRQ_PIN));
	// attach interrupt
	attachInterrupt(digitalPinToInterrupt(MY_RF24_IRQ_PIN), RF24_irqHandler, FALLING);
#endif
	// CRC and power up
	RF24_setRFConfiguration(MY_RF24_CONFIGURATION | _BV(RF24_PWR_UP)) ;
	// settle >2ms
	CyDelay(5);
	// set address width
	RF24_setAddressWidth(MY_RF24_ADDR_WIDTH);
	// auto retransmit delay 1500us, auto retransmit count 15
	RF24_setRetries(RF24_SET_ARD, RF24_SET_ARC);
	// set channel
	RF24_setChannel(MY_RF24_CHANNEL);
	// set data rate and pa level
	RF24_setRFSetup(MY_RF24_RF_SETUP);
	// toggle features (necessary on some clones and non-P versions)
	RF24_enableFeatures();
	// enable ACK payload and dynamic payload
	RF24_setFeature(MY_RF24_FEATURE);
	// sanity check (this function is P/non-P independent)
	if (!RF24_sanityCheck()) {
		RF24_DEBUG(PSTR("!RF24:INIT:SANCHK FAIL\n")); // sanity check failed, check wiring or replace module
		return false;
	}
	// enable broadcasting pipe
	RF24_setPipe(_BV(RF24_ERX_P0 + RF24_BROADCAST_PIPE));
	// disable AA on all pipes, activate when node pipe set
	RF24_setAutoACK(0x00);
	// enable dynamic payloads on used pipes
	RF24_setDynamicPayload(_BV(RF24_DPL_P0 + RF24_BROADCAST_PIPE) | _BV(RF24_DPL_P0));
	// listen to broadcast pipe
	MY_RF24_BASE_ADDR[0] = BROADCAST_ADDRESS;
	RF24_setPipeAddress(RF24_RX_ADDR_P0 + RF24_BROADCAST_PIPE, (uint8_t*)&MY_RF24_BASE_ADDR,
	                    RF24_BROADCAST_PIPE > 1 ? 1 : MY_RF24_ADDR_WIDTH);
	// pipe 0, set full address, later only LSB is updated
	RF24_setPipeAddress(RF24_RX_ADDR_P0, (uint8_t*)&MY_RF24_BASE_ADDR, MY_RF24_ADDR_WIDTH);
	RF24_setPipeAddress(RF24_TX_ADDR, (uint8_t*)&MY_RF24_BASE_ADDR, MY_RF24_ADDR_WIDTH);
	// reset FIFO
	RF24_flushRX();
	RF24_flushTX();
	// reset interrupts
	RF24_setStatus(_BV(RF24_TX_DS) | _BV(RF24_MAX_RT) | _BV(RF24_RX_DR));
	return true;
}

