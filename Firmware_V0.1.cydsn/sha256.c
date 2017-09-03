#include <string.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#define PRIPSTR "%S"
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include "sha256.h"
#include "project.h"


static const uint32_t sha256K[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define BUFFER_SIZE 64
#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

static const uint8_t sha256InitState[32] = {
	0x67,0xe6,0x09,0x6a, // H0
	0x85,0xae,0x67,0xbb, // H1
	0x72,0xf3,0x6e,0x3c, // H2
	0x3a,0xf5,0x4f,0xa5, // H3
	0x7f,0x52,0x0e,0x51, // H4
	0x8c,0x68,0x05,0x9b, // H5
	0xab,0xd9,0x83,0x1f, // H6
	0x19,0xcd,0xe0,0x5b  // H7
};

union _state    state;
union _buffer   buffer;
uint32_t        byteCount;
uint8_t         bufferOffset;
uint8_t         keyBuffer[BLOCK_LENGTH]; // K0 in FIPS-198a
uint8_t         innerHash[HASH_LENGTH];


static uint8_t device_pin;
#ifdef ARDUINO_ARCH_AVR
static volatile uint8_t *device_port_DDR, *device_port_OUT, *device_port_IN;
#endif
static void sha204c_calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc);
static uint8_t sha204c_check_crc(uint8_t *response);
static void swi_set_signal_pin(uint8_t is_high);
static uint8_t swi_receive_bytes(uint8_t count, uint8_t *buffer);
static uint8_t swi_send_bytes(uint8_t count, uint8_t *buffer);
static uint8_t swi_send_byte(uint8_t value);
static uint8_t sha204p_receive_response(uint8_t size, uint8_t *response);
static uint8_t sha204m_read(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t zone, uint16_t address);
static uint8_t sha204c_resync(uint8_t size, uint8_t *response);
static uint8_t sha204c_send_and_receive(uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer,
                                        uint8_t execution_delay, uint8_t execution_timeout);

#ifndef SOFTWARE_SHA256
/* SWI bit bang functions */
static void swi_set_signal_pin(uint8_t is_high)
{
	SHA204_SET_OUTPUT();

	if (is_high) {
		SHA204_POUT_HIGH();
	} else {
		SHA204_POUT_LOW();
	}
}

static uint8_t swi_send_bytes(uint8_t count, uint8_t *buffer)
{
	uint8_t i, bit_mask;

	// Disable interrupts while sending.
	noInterrupts();  //swi_disable_interrupts();

	// Set signal pin as output.
	SHA204_POUT_HIGH();
	SHA204_SET_OUTPUT();


	// Wait turn around time.
	delayMicroseconds(RX_TX_DELAY);  //RX_TX_DELAY;

	for (i = 0; i < count; i++) {
		for (bit_mask = 1; bit_mask > 0; bit_mask <<= 1) {
			if (bit_mask & buffer[i]) {
				SHA204_POUT_LOW(); //*device_port_OUT &= ~device_pin;
				delayMicroseconds(BIT_DELAY);  //BIT_DELAY_1;
				SHA204_POUT_HIGH(); //*device_port_OUT |= device_pin;
				delayMicroseconds(7*BIT_DELAY);  //BIT_DELAY_7;
			} else {
				// Send a zero bit.
				SHA204_POUT_LOW(); //*device_port_OUT &= ~device_pin;
				delayMicroseconds(BIT_DELAY);  //BIT_DELAY_1;
				SHA204_POUT_HIGH(); //*device_port_OUT |= device_pin;
				delayMicroseconds(BIT_DELAY);  //BIT_DELAY_1;
				SHA204_POUT_LOW(); //*device_port_OUT &= ~device_pin;
				delayMicroseconds(BIT_DELAY);  //BIT_DELAY_1;
				SHA204_POUT_HIGH(); //*device_port_OUT |= device_pin;
				delayMicroseconds(5*BIT_DELAY);  //BIT_DELAY_5;
			}
		}
	}
	interrupts();  //swi_enable_interrupts();
	return SWI_FUNCTION_RETCODE_SUCCESS;
}
#endif
static uint8_t swi_send_byte(uint8_t value)
{
	return swi_send_bytes(1, &value);
}
#ifndef SOFTWARE_SHA256
static uint8_t swi_receive_bytes(uint8_t count, uint8_t *buffer)
{
	uint8_t status = SWI_FUNCTION_RETCODE_SUCCESS;
	uint8_t i;
	uint8_t bit_mask;
	uint8_t pulse_count;
	uint8_t timeout_count;

	// Disable interrupts while receiving.
	noInterrupts(); //swi_disable_interrupts();

	// Configure signal pin as input.
	SHA204_SET_INPUT();

	// Receive bits and store in buffer.
	for (i = 0; i < count; i++) {
		for (bit_mask = 1; bit_mask > 0; bit_mask <<= 1) {
			pulse_count = 0;

			// Make sure that the variable below is big enough.
			// Change it to uint16_t if 255 is too small, but be aware that
			// the loop resolution decreases on an 8-bit controller in that case.
			timeout_count = START_PULSE_TIME_OUT;

			// Detect start bit.
			while (--timeout_count > 0) {
				// Wait for falling edge.
				if (SHA204_PIN_READ() == 0) {
					break;
				}
			}

			if (timeout_count == 0) {
				status = SWI_FUNCTION_RETCODE_TIMEOUT;
				break;
			}

			do {
				// Wait for rising edge.
				if (SHA204_PIN_READ() != 0) {
					// For an Atmel microcontroller this might be faster than "pulse_count++".
					pulse_count = 1;
					break;
				}
			} while (--timeout_count > 0);

			if (pulse_count == 0) {
				status = SWI_FUNCTION_RETCODE_TIMEOUT;
				break;
			}

			// Trying to measure the time of start bit and calculating the timeout
			// for zero bit detection is not accurate enough for an 8 MHz 8-bit CPU.
			// So let's just wait the maximum time for the falling edge of a zero bit
			// to arrive after we have detected the rising edge of the start bit.
			timeout_count = ZERO_PULSE_TIME_OUT;

			// Detect possible edge indicating zero bit.
			do {
				if (SHA204_PIN_READ() == 0) {
					// For an Atmel microcontroller this might be faster than "pulse_count++".
					pulse_count = 2;
					break;
				}
			} while (--timeout_count > 0);

			// Wait for rising edge of zero pulse before returning. Otherwise we might interpret
			// its rising edge as the next start pulse.
			if (pulse_count == 2) {
				do {
					if (SHA204_PIN_READ() != 0) {
						break;
					}
				} while (timeout_count-- > 0);
			}

			// Update byte at current buffer index.
			else {
				buffer[i] |= bit_mask;    // received "one" bit
			}
		}

		if (status != SWI_FUNCTION_RETCODE_SUCCESS) {
			break;
		}
	}
	interrupts(); //swi_enable_interrupts();

	if (status == SWI_FUNCTION_RETCODE_TIMEOUT) {
		if (i > 0) {
			// Indicate that we timed out after having received at least one byte.
			status = SWI_FUNCTION_RETCODE_RX_FAIL;
		}
	}
	return status;
}
#endif
/* Physical functions */

static uint8_t sha204p_receive_response(uint8_t size, uint8_t *response)
{
	uint8_t i;
	uint8_t ret_code;

	for (i = 0; i < size; i++) {
		response[i] = 0;
	}

	(void) swi_send_byte(SHA204_SWI_FLAG_TX);

	ret_code = swi_receive_bytes(size, response);
	if (ret_code == SWI_FUNCTION_RETCODE_SUCCESS || ret_code == SWI_FUNCTION_RETCODE_RX_FAIL) {
		uint8_t count_byte;
		count_byte = response[SHA204_BUFFER_POS_COUNT];
		if ((count_byte < SHA204_RSP_SIZE_MIN) || (count_byte > size)) {
			return SHA204_INVALID_SIZE;
		}

		return SHA204_SUCCESS;
	}

	// Translate error so that the Communication layer
	// can distinguish between a real error or the
	// device being busy executing a command.
	if (ret_code == SWI_FUNCTION_RETCODE_TIMEOUT) {
		return SHA204_RX_NO_RESPONSE;
	} else {
		return SHA204_RX_FAIL;
	}
}

/* Communication functions */

static uint8_t sha204c_resync(uint8_t size, uint8_t *response)
{
	// Try to re-synchronize without sending a Wake token
	// (step 1 of the re-synchronization process).
    CyDelay(SHA204_SYNC_TIMEOUT);
	uint8_t ret_code = sha204p_receive_response(size, response);
	if (ret_code == SHA204_SUCCESS) {
		return ret_code;
	}

	// We lost communication. Send a Wake pulse and try
	// to receive a response (steps 2 and 3 of the
	// re-synchronization process).
	atsha204_sleep();
	ret_code = atsha204_wakeup(response);

	// Translate a return value of success into one
	// that indicates that the device had to be woken up
	// and might have lost its TempKey.
	return (ret_code == SHA204_SUCCESS ? SHA204_RESYNC_WITH_WAKEUP : ret_code);
}

static uint8_t sha204c_send_and_receive(uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer,
                                        uint8_t execution_delay, uint8_t execution_timeout)
{
	uint8_t ret_code = SHA204_FUNC_FAIL;
	uint8_t ret_code_resync;
	uint8_t n_retries_send;
	uint8_t n_retries_receive;
	uint8_t i;
	uint8_t status_byte;
	uint8_t count = tx_buffer[SHA204_BUFFER_POS_COUNT];
	uint8_t count_minus_crc = count - SHA204_CRC_SIZE;
	uint16_t execution_timeout_us = (uint16_t) (execution_timeout * 1000) + SHA204_RESPONSE_TIMEOUT;
	volatile uint16_t timeout_countdown;

	// Append CRC.
	sha204c_calculate_crc(count_minus_crc, tx_buffer, tx_buffer + count_minus_crc);

	// Retry loop for sending a command and receiving a response.
	n_retries_send = SHA204_RETRY_COUNT + 1;

	while ((n_retries_send-- > 0) && (ret_code != SHA204_SUCCESS)) {
		// Send command.
		ret_code = swi_send_byte(SHA204_SWI_FLAG_CMD);
		if (ret_code != SWI_FUNCTION_RETCODE_SUCCESS) {
			ret_code = SHA204_COMM_FAIL;
		} else {
			ret_code = swi_send_bytes(count, tx_buffer);
		}

		if (ret_code != SHA204_SUCCESS) {
			if (sha204c_resync(rx_size, rx_buffer) == SHA204_RX_NO_RESPONSE) {
				return ret_code; // The device seems to be dead in the water.
			} else {
				continue;
			}
		}

		// Wait minimum command execution time and then start polling for a response.
		CyDelay(execution_delay);

		// Retry loop for receiving a response.
		n_retries_receive = SHA204_RETRY_COUNT + 1;
		while (n_retries_receive-- > 0) {
			// Reset response buffer.
			for (i = 0; i < rx_size; i++) {
				rx_buffer[i] = 0;
			}

			// Poll for response.
			timeout_countdown = execution_timeout_us;
			do {
				ret_code = sha204p_receive_response(rx_size, rx_buffer);
				timeout_countdown -= SHA204_RESPONSE_TIMEOUT;
			} while ((timeout_countdown > SHA204_RESPONSE_TIMEOUT) && (ret_code == SHA204_RX_NO_RESPONSE));

			if (ret_code == SHA204_RX_NO_RESPONSE) {
				// We did not receive a response. Re-synchronize and send command again.
				if (sha204c_resync(rx_size, rx_buffer) == SHA204_RX_NO_RESPONSE) {
					// The device seems to be dead in the water.
					return ret_code;
				} else {
					break;
				}
			}

			// Check whether we received a valid response.
			if (ret_code == SHA204_INVALID_SIZE) {
				// We see 0xFF for the count when communication got out of sync.
				ret_code_resync = sha204c_resync(rx_size, rx_buffer);
				if (ret_code_resync == SHA204_SUCCESS) {
					// We did not have to wake up the device. Try receiving response again.
					continue;
				}
				if (ret_code_resync == SHA204_RESYNC_WITH_WAKEUP) {
					// We could re-synchronize, but only after waking up the device.
					// Re-send command.
					break;
				} else {
					// We failed to re-synchronize.
					return ret_code;
				}
			}

			// We received a response of valid size.
			// Check the consistency of the response.
			ret_code = sha204c_check_crc(rx_buffer);
			if (ret_code == SHA204_SUCCESS) {
				// Received valid response.
				if (rx_buffer[SHA204_BUFFER_POS_COUNT] > SHA204_RSP_SIZE_MIN) {
					// Received non-status response. We are done.
					return ret_code;
				}

				// Received status response.
				status_byte = rx_buffer[SHA204_BUFFER_POS_STATUS];

				// Translate the three possible device status error codes
				// into library return codes.
				if (status_byte == SHA204_STATUS_BYTE_PARSE) {
					return SHA204_PARSE_ERROR;
				}
				if (status_byte == SHA204_STATUS_BYTE_EXEC) {
					return SHA204_CMD_FAIL;
				}
				if (status_byte == SHA204_STATUS_BYTE_COMM) {
					// In case of the device status byte indicating a communication
					// error this function exits the retry loop for receiving a response
					// and enters the overall retry loop
					// (send command / receive response).
					ret_code = SHA204_STATUS_CRC;
					break;
				}

				// Received status response from CheckMAC, DeriveKey, GenDig,
				// Lock, Nonce, Pause, UpdateExtra, or Write command.
				return ret_code;
			}

			else {
				// Received response with incorrect CRC.
				ret_code_resync = sha204c_resync(rx_size, rx_buffer);
				if (ret_code_resync == SHA204_SUCCESS) {
					// We did not have to wake up the device. Try receiving response again.
					continue;
				}
				if (ret_code_resync == SHA204_RESYNC_WITH_WAKEUP) {
					// We could re-synchronize, but only after waking up the device.
					// Re-send command.
					break;
				} else {
					// We failed to re-synchronize.
					return ret_code;
				}
			} // block end of check response consistency

		} // block end of receive retry loop

	} // block end of send and receive retry loop

	return ret_code;
}


/* Marshaling functions */

static uint8_t sha204m_read(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t zone, uint16_t address)
{
	uint8_t rx_size;

	address >>= 2;

	tx_buffer[SHA204_COUNT_IDX] = READ_COUNT;
	tx_buffer[SHA204_OPCODE_IDX] = SHA204_READ;
	tx_buffer[READ_ZONE_IDX] = zone;
	tx_buffer[READ_ADDR_IDX] = (uint8_t) (address & SHA204_ADDRESS_MASK);
	tx_buffer[READ_ADDR_IDX + 1] = 0;

	rx_size = (zone & SHA204_ZONE_COUNT_FLAG) ? READ_32_RSP_SIZE : READ_4_RSP_SIZE;

	return sha204c_send_and_receive(&tx_buffer[0], rx_size, &rx_buffer[0], READ_DELAY,
	                                READ_EXEC_MAX - READ_DELAY);
}

/* CRC Calculator and Checker */

static void sha204c_calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc)
{
	uint8_t counter;
	uint16_t crc_register = 0;
	uint16_t polynom = 0x8005;
	uint8_t shift_register;
	uint8_t data_bit, crc_bit;

	for (counter = 0; counter < length; counter++) {
		for (shift_register = 0x01; shift_register > 0x00; shift_register <<= 1) {
			data_bit = (data[counter] & shift_register) ? 1 : 0;
			crc_bit = crc_register >> 15;

			// Shift CRC to the left by 1.
			crc_register <<= 1;

			if ((data_bit ^ crc_bit) != 0) {
				crc_register ^= polynom;
			}
		}
	}
	crc[0] = (uint8_t) (crc_register & 0x00FF);
	crc[1] = (uint8_t) (crc_register >> 8);
}

static uint8_t sha204c_check_crc(uint8_t *response)
{
	uint8_t crc[SHA204_CRC_SIZE];
	uint8_t count = response[SHA204_BUFFER_POS_COUNT];

	count -= SHA204_CRC_SIZE;
	sha204c_calculate_crc(count, response, crc);

	return (crc[0] == response[count] && crc[1] == response[count + 1])
	       ? SHA204_SUCCESS : SHA204_BAD_CRC;
}

/* Public functions */

void atsha204_init(uint8_t pin)
{
#if defined(ARDUINO_ARCH_AVR)
	device_pin = digitalPinToBitMask(pin);  // Find the bit value of the pin
	uint8_t port = digitalPinToPort(pin); // temoporarily used to get the next three registers

	// Point to data direction register port of pin
	device_port_DDR = portModeRegister(port);
	// Point to output register of pin
	device_port_OUT = portOutputRegister(port);
	// Point to input register of pin
	device_port_IN = portInputRegister(port);
#else
	device_pin = pin;
#endif
}

void atsha204_idle(void)
{
	swi_send_byte(SHA204_SWI_FLAG_IDLE);
}

void atsha204_sleep(void)
{
	swi_send_byte(SHA204_SWI_FLAG_SLEEP);
}

uint8_t atsha204_wakeup(uint8_t *response)
{
	swi_set_signal_pin(0);
    CyDelayUs(10*SHA204_WAKEUP_PULSE_WIDTH);
	swi_set_signal_pin(1);
	CyDelay(SHA204_WAKEUP_DELAY);

	uint8_t ret_code = sha204p_receive_response(SHA204_RSP_SIZE_MIN, response);
	if (ret_code != SHA204_SUCCESS) {
		return ret_code;
	}

	// Verify status response.
	if (response[SHA204_BUFFER_POS_COUNT] != SHA204_RSP_SIZE_MIN) {
		ret_code = SHA204_INVALID_SIZE;
	} else if (response[SHA204_BUFFER_POS_STATUS] != SHA204_STATUS_BYTE_WAKEUP) {
		ret_code = SHA204_COMM_FAIL;
	} else {
		if ((response[SHA204_RSP_SIZE_MIN - SHA204_CRC_SIZE] != 0x33)
		        || (response[SHA204_RSP_SIZE_MIN + 1 - SHA204_CRC_SIZE] != 0x43)) {
			ret_code = SHA204_BAD_CRC;
		}
	}
	if (ret_code != SHA204_SUCCESS) {
		CyDelay(SHA204_COMMAND_EXEC_MAX);
	}

	return ret_code;
}

uint8_t atsha204_execute(uint8_t op_code, uint8_t param1, uint16_t param2,
                         uint8_t datalen1, uint8_t *data1,	uint8_t tx_size, uint8_t *tx_buffer, uint8_t rx_size,
                         uint8_t *rx_buffer)
{
	uint8_t poll_delay, poll_timeout, response_size;
	uint8_t *p_buffer;
	uint8_t len;
	(void)tx_size;

	// Supply delays and response size.
	switch (op_code) {
	case SHA204_GENDIG:
		poll_delay = GENDIG_DELAY;
		poll_timeout = GENDIG_EXEC_MAX - GENDIG_DELAY;
		response_size = GENDIG_RSP_SIZE;
		break;

	case SHA204_HMAC:
		poll_delay = HMAC_DELAY;
		poll_timeout = HMAC_EXEC_MAX - HMAC_DELAY;
		response_size = HMAC_RSP_SIZE;
		break;

	case SHA204_NONCE:
		poll_delay = NONCE_DELAY;
		poll_timeout = NONCE_EXEC_MAX - NONCE_DELAY;
		response_size = param1 == NONCE_MODE_PASSTHROUGH
		                ? NONCE_RSP_SIZE_SHORT : NONCE_RSP_SIZE_LONG;
		break;

	case SHA204_RANDOM:
		poll_delay = RANDOM_DELAY;
		poll_timeout = RANDOM_EXEC_MAX - RANDOM_DELAY;
		response_size = RANDOM_RSP_SIZE;
		break;

	case SHA204_SHA:
		poll_delay = SHA_DELAY;
		poll_timeout = SHA_EXEC_MAX - SHA_DELAY;
		response_size = param1 == SHA_INIT
		                ? SHA_RSP_SIZE_SHORT : SHA_RSP_SIZE_LONG;
		break;

	case SHA204_WRITE:
		poll_delay = WRITE_DELAY;
		poll_timeout = WRITE_EXEC_MAX - WRITE_DELAY;
		response_size = WRITE_RSP_SIZE;
		break;

	default:
		poll_delay = 0;
		poll_timeout = SHA204_COMMAND_EXEC_MAX;
		response_size = rx_size;
	}

	// Assemble command.
	len = datalen1 + SHA204_CMD_SIZE_MIN;
	p_buffer = tx_buffer;
	*p_buffer++ = len;
	*p_buffer++ = op_code;
	*p_buffer++ = param1;
	*p_buffer++ = param2 & 0xFF;
	*p_buffer++ = param2 >> 8;

	if (datalen1 > 0) {
		memcpy(p_buffer, data1, datalen1);
		p_buffer += datalen1;
	}

	sha204c_calculate_crc(len - SHA204_CRC_SIZE, tx_buffer, p_buffer);

	// Send command and receive response.
	return sha204c_send_and_receive(&tx_buffer[0], response_size,
	                                &rx_buffer[0],	poll_delay, poll_timeout);
}

uint8_t atsha204_getSerialNumber(uint8_t * response)
{
	uint8_t readCommand[READ_COUNT];
	uint8_t readResponse[READ_4_RSP_SIZE];

	/* read from bytes 0->3 of config zone */
	uint8_t returnCode = sha204m_read(readCommand, readResponse, SHA204_ZONE_CONFIG, ADDRESS_SN03);
	if (!returnCode) {
        int i;
		for (i=0; i<4; i++) {// store bytes 0-3 into respones array
			response[i] = readResponse[SHA204_BUFFER_POS_DATA+i];
		}

		/* read from bytes 8->11 of config zone */
		returnCode = sha204m_read(readCommand, readResponse, SHA204_ZONE_CONFIG, ADDRESS_SN47);

		for (i=4; i<8; i++) {// store bytes 4-7 of SN into response array
			response[i] = readResponse[SHA204_BUFFER_POS_DATA+(i-4)];
		}

		if (!returnCode) {
			/* Finally if last two reads were successful, read byte 8 of the SN */
			returnCode = sha204m_read(readCommand, readResponse, SHA204_ZONE_CONFIG, ADDRESS_SN8);
			response[8] = readResponse[SHA204_BUFFER_POS_DATA]; // Byte 8 of SN should always be 0xEE
		}
	}

	return returnCode;
}

void sha256_init(void)
{
	memcpy(state.b,sha256InitState,32);
	byteCount = 0;
	bufferOffset = 0;
}

uint32_t sha256_ror32(uint32_t number, uint8_t bits)
{
	return ((number << (32-bits)) | (number >> bits));
}

void sha256_hashBlock()
{
	uint8_t i;
	uint32_t a,b,c,d,e,f,g,h,t1,t2;

	a=state.w[0];
	b=state.w[1];
	c=state.w[2];
	d=state.w[3];
	e=state.w[4];
	f=state.w[5];
	g=state.w[6];
	h=state.w[7];

	for (i=0; i<64; i++) {
		if (i>=16) {
			t1 = buffer.w[i&15] + buffer.w[(i-7)&15];
			t2 = buffer.w[(i-2)&15];
			t1 += sha256_ror32(t2,17) ^ sha256_ror32(t2,19) ^ (t2>>10);
			t2 = buffer.w[(i-15)&15];
			t1 += sha256_ror32(t2,7) ^ sha256_ror32(t2,18) ^ (t2>>3);
			buffer.w[i&15] = t1;
		}
		t1 = h;
		t1 += sha256_ror32(e,6) ^ sha256_ror32(e,11) ^ sha256_ror32(e,25); // ∑1(e)
		t1 += g ^ (e & (g ^ f)); // Ch(e,f,g)
		t1 += pgm_read_dword(sha256K+i); // Ki
		t1 += buffer.w[i&15]; // Wi
		t2 = sha256_ror32(a,2) ^ sha256_ror32(a,13) ^ sha256_ror32(a,22); // ∑0(a)
		t2 += ((b & c) | (a & (b | c))); // Maj(a,b,c)
		h=g;
		g=f;
		f=e;
		e=d+t1;
		d=c;
		c=b;
		b=a;
		a=t1+t2;
	}
	state.w[0] += a;
	state.w[1] += b;
	state.w[2] += c;
	state.w[3] += d;
	state.w[4] += e;
	state.w[5] += f;
	state.w[6] += g;
	state.w[7] += h;
}

void sha256_addUncounted(uint8_t data)
{
	buffer.b[bufferOffset ^ 3] = data;
	bufferOffset++;
	if (bufferOffset == BUFFER_SIZE) {
		sha256_hashBlock();
		bufferOffset = 0;
	}
}

void sha256_write(uint8_t data)
{
	++byteCount;
	sha256_addUncounted(data);
}

void sha256_pad()
{
	// Implement SHA-256 padding (fips180-2 §5.1.1)

	// Pad with 0x80 followed by 0x00 until the end of the block
	sha256_addUncounted(0x80);
	while (bufferOffset != 56) {
		sha256_addUncounted(0x00);
	}

	// Append length in the last 8 bytes
	sha256_addUncounted(0); // We're only using 32 bit lengths
	sha256_addUncounted(0); // But SHA-1 supports 64 bit lengths
	sha256_addUncounted(0); // So zero pad the top bits
	sha256_addUncounted(byteCount >> 29); // Shifting to multiply by 8
	sha256_addUncounted(byteCount >> 21); // as SHA-1 supports bitstreams as well as
	sha256_addUncounted(byteCount >> 13); // byte.
	sha256_addUncounted(byteCount >> 5);
	sha256_addUncounted(byteCount << 3);
}

uint8_t* sha256_result(void)
{
	// Pad to complete the last block
	sha256_pad();
    int i;
	// Swap byte order back
	for (i=0; i<8; i++) {
		uint32_t a,b;
		a=state.w[i];
		b=a<<24;
		b|=(a<<8) & 0x00ff0000;
		b|=(a>>8) & 0x0000ff00;
		b|=a>>24;
		state.w[i]=b;
	}

	// Return pointer to hash (20 characters)
	return &state.b[0];
}

void sha256_initHmac(const uint8_t* key, int keyLength)
{
	uint8_t i;
	memset(keyBuffer,0,BLOCK_LENGTH);
	if (keyLength > BLOCK_LENGTH) {
		// Hash long keys
		sha256_init();
		for (; keyLength--;) {
			sha256_write(*key++);
		}
		memcpy(keyBuffer,sha256_result(),HASH_LENGTH);
	} else {
		// Block length keys are used as is
		memcpy(keyBuffer,key,keyLength);
	}
	// Start inner hash
	sha256_init();
	for (i=0; i<BLOCK_LENGTH; i++) {
		sha256_write(keyBuffer[i] ^ HMAC_IPAD);
	}
}

uint8_t* sha256_resultHmac(void)
{
	uint8_t i;
	// Complete inner hash
	memcpy(innerHash,sha256_result(),HASH_LENGTH);
	// Calculate outer hash
	sha256_init();
	for (i=0; i<BLOCK_LENGTH; i++) {
		sha256_write(keyBuffer[i] ^ HMAC_OPAD);
	}
	for (i=0; i<HASH_LENGTH; i++) {
		sha256_write(innerHash[i]);
	}
	return sha256_result();
}
