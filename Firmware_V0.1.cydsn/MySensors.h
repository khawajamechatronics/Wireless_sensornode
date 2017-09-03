/**
 * @file MySensors.h
 *
 * MySensors main interface (includes all necessary code for the library)
 */
#ifndef MySensors_h
#define MySensors_h

#include "MySensorsCore.h"

    // Detect node type
/**
 * @def MY_GATEWAY_FEATURE
 * @brief Is set for gateway sketches.
 */
/**
 * @def MY_IS_GATEWAY
 * @brief Is true when @ref MY_GATEWAY_FEATURE is set.
 */
/**
 * @def MY_NODE_TYPE
 * @brief Contain a string describing the class of sketch/node (gateway/repeater/sensor).
 */
#if defined(MY_GATEWAY_SERIAL) || defined(MY_GATEWAY_W5100) || defined(MY_GATEWAY_ENC28J60) || defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_LINUX) || defined(MY_GATEWAY_MQTT_CLIENT)
    #define MY_GATEWAY_FEATURE
    #define MY_IS_GATEWAY (true)
    #define MY_NODE_TYPE "GW"
    #elif defined(MY_REPEATER_FEATURE)
    #define MY_IS_GATEWAY (false)
    #define MY_NODE_TYPE "REPEATER"
#else
    #define MY_IS_GATEWAY (false)
    #define MY_NODE_TYPE "NODE"
#endif

// Enable radio "feature" if one of the radio types was enabled
#if defined(MY_RADIO_NRF24) || defined(MY_RADIO_RFM69) || defined(MY_RADIO_RFM95) || defined(MY_RS485)
    #define MY_SENSOR_NETWORK
#endif

// HARDWARE
#if defined(ARDUINO_ARCH_ESP8266)
    #include "core/MyHwESP8266.cpp"
    #elif defined(ARDUINO_ARCH_AVR)
    #include "drivers/AVR/DigitalWriteFast/digitalWriteFast.h"
    #include "core/MyHwAVR.cpp"
    #elif defined(ARDUINO_ARCH_SAMD)
    #include "core/MyHwSAMD.cpp"
    #elif defined(__linux__)
    #include "core/MyHwLinuxGeneric.cpp"
#endif

// LEDS
#if !defined(MY_DEFAULT_ERR_LED_PIN) && defined(MY_HW_ERR_LED_PIN)
    #define MY_DEFAULT_ERR_LED_PIN MY_HW_ERR_LED_PIN
#endif

#if !defined(MY_DEFAULT_TX_LED_PIN) && defined(MY_HW_TX_LED_PIN)
    #define MY_DEFAULT_TX_LED_PIN MY_HW_TX_LED_PIN
#endif

#if !defined(MY_DEFAULT_RX_LED_PIN) && defined(MY_HW_TX_LED_PIN)
    #define MY_DEFAULT_RX_LED_PIN MY_HW_TX_LED_PIN
#endif

#if defined(MY_LEDS_BLINKING_FEATURE)
    #error MY_LEDS_BLINKING_FEATURE is now removed from MySensors core,\
    define MY_DEFAULT_ERR_LED_PIN, MY_DEFAULT_TX_LED_PIN or\
    MY_DEFAULT_RX_LED_PIN in your sketch instead to enable LEDs
#endif

/**
 * @def MY_DEFAULT_LED_BLINK_PERIOD
 * @brief Default LEDs blinking period in milliseconds.
 */
#ifndef MY_DEFAULT_LED_BLINK_PERIOD
    #define MY_DEFAULT_LED_BLINK_PERIOD 300
#endif

#if defined(MY_DEFAULT_RX_LED_PIN) || defined(MY_DEFAULT_TX_LED_PIN) || defined(MY_DEFAULT_ERR_LED_PIN)
    #include "MyLeds.h"
#else
    #include "MyLeds.h"
#endif

#include "MyIndication.h"


// INCLUSION MODE
#if defined(MY_INCLUSION_MODE_FEATURE)
    //#include "core/MyInclusionMode.cpp"
#endif


// SIGNING
#if defined(MY_SIGNING_ATSHA204) || defined(MY_SIGNING_SOFT)
    #define MY_SIGNING_FEATURE
#endif
//#include "MySigning.h"
//#include "drivers/ATSHA204/sha256.cpp"
#if defined(MY_SIGNING_FEATURE)
// SIGNING COMMON FUNCTIONS
    #if defined(MY_SIGNING_ATSHA204) && defined(MY_SIGNING_SOFT)
        #error Only one signing engine can be activated
    #endif
    #if defined(MY_SIGNING_ATSHA204) && defined(__linux__)
        #error No support for ATSHA204 on this platform
    #endif
    #if defined(MY_SIGNING_ATSHA204)
        #include "core/MySigningAtsha204.cpp"
        #include "drivers/ATSHA204/ATSHA204.cpp"
    #elif defined(MY_SIGNING_SOFT)
        #include "MySigning.h"
    #endif
#endif


// FLASH
#if defined(MY_OTA_FIRMWARE_FEATURE)
    #include "drivers/SPIFlash/SPIFlash.cpp"
    #include "core/MyOTAFirmwareUpdate.cpp"
#endif

// GATEWAY - TRANSPORT
#if defined(MY_CONTROLLER_IP_ADDRESS) || defined(MY_CONTROLLER_URL_ADDRESS)
    #define MY_GATEWAY_CLIENT_MODE
#endif
#if defined(MY_USE_UDP) && !defined(MY_GATEWAY_CLIENT_MODE)
    #error You must specify MY_CONTROLLER_IP_ADDRESS or MY_CONTROLLER_URL_ADDRESS for UDP
#endif

#if defined(MY_GATEWAY_MQTT_CLIENT)
    #if defined(MY_SENSOR_NETWORK)
        // We assume that a gateway having a radio also should act as repeater
        #define MY_REPEATER_FEATURE
    #endif

    // GATEWAY - COMMON FUNCTIONS
    // We support MQTT Client using W5100, ESP8266 and Linux
    #if !defined(MY_GATEWAY_CLIENT_MODE)
        #error You must specify MY_CONTROLLER_IP_ADDRESS or MY_CONTROLLER_URL_ADDRESS
    #endif

    #if !defined(MY_MQTT_PUBLISH_TOPIC_PREFIX)
        #error You must specify a topic publish prefix MY_MQTT_PUBLISH_TOPIC_PREFIX for this MQTT client
    #endif
    #if !defined(MY_MQTT_SUBSCRIBE_TOPIC_PREFIX)
        #error You must specify a topic subscribe prefix MY_MQTT_SUBSCRIBE_TOPIC_PREFIX for this MQTT client
    #endif

    #if !defined(MY_MQTT_CLIENT_ID)
        #error You must define a unique MY_MQTT_CLIENT_ID for this MQTT client
    #endif

    #include "core/MyGatewayTransport.cpp"
    #include "core/MyProtocolMySensors.cpp"

    #if defined(MY_GATEWAY_LINUX)
        #include "drivers/Linux/EthernetClient.h"
        #include "drivers/Linux/EthernetServer.h"
        #include "drivers/Linux/IPAddress.h"
    #endif
    #include "drivers/PubSubClient/PubSubClient.cpp"
    #include "core/MyGatewayTransportMQTTClient.cpp"
    #elif defined(MY_GATEWAY_FEATURE)
    // GATEWAY - COMMON FUNCTIONS
    //#include "core/MyGatewayTransport.cpp"

    //#include "core/MyProtocolMySensors.cpp"

    // GATEWAY - CONFIGURATION
    #if defined(MY_SENSOR_NETWORK)
    // We assume that a gateway having a radio also should act as repeater
    #define MY_REPEATER_FEATURE
    #endif
    #if !defined(MY_PORT)
    #error You must define MY_PORT (controller or gatway port to open)
    #endif
    #if defined(MY_GATEWAY_ESP8266)
    // GATEWAY - ESP8266
    //#include "core/MyGatewayTransportEthernet.cpp"
    #elif defined(MY_GATEWAY_LINUX)
    // GATEWAY - Generic Linux
    //#include "drivers/Linux/EthernetClient.h"
    //#include "drivers/Linux/EthernetServer.h"
    //#include "drivers/Linux/IPAddress.h"
    //#include "core/MyGatewayTransportEthernet.cpp"
    #elif defined(MY_GATEWAY_W5100)
    // GATEWAY - W5100
    //#include "core/MyGatewayTransportEthernet.cpp"
    #elif defined(MY_GATEWAY_ENC28J60)
    // GATEWAY - ENC28J60
    #if defined(MY_USE_UDP)
    #error UDP mode is not available for ENC28J60
    #endif
    //#include "core/MyGatewayTransportEthernet.cpp"
    #elif defined(MY_GATEWAY_SERIAL)
    // GATEWAY - SERIAL
    //#include "core/MyGatewayTransportSerial.cpp"
    #endif
#endif

// RAM ROUTING TABLE
#if defined(MY_RAM_ROUTING_TABLE_FEATURE) && defined(MY_REPEATER_FEATURE)
// activate feature based on architecture
    #if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_SAMD) || defined(LINUX_ARCH_RASPBERRYPI)
        #define MY_RAM_ROUTING_TABLE_ENABLED
    #elif defined(ARDUINO_ARCH_AVR)
    // memory limited, enable with care
    // #define MY_RAM_ROUTING_TABLE_ENABLED
    #endif
#endif

#if defined(MY_REPEATER_FEATURE)
    #define MY_TRANSPORT_SANITY_CHECK
#endif


#if defined(MY_TRANSPORT_DONT_CARE_MODE)
    #error This directive is deprecated, set MY_TRANSPORT_WAIT_READY_MS instead!
#endif


// RADIO
#if defined(MY_RADIO_NRF24) || defined(MY_RADIO_RFM69) || defined(MY_RADIO_RFM95) ||defined(MY_RS485)
    // SOFTSPI
    #ifdef MY_SOFTSPI
        #if defined(ARDUINO_ARCH_ESP8266)
            #error Soft SPI is not available on ESP8266
        #endif
        #include "drivers/AVR/DigitalIO/DigitalIO.h"
    #endif

    #if defined(MY_RADIO_NRF24) && defined(__linux__) && !(defined(LINUX_SPI_BCM) || defined(LINUX_SPI_SPIDEV))
        #error No support for nRF24 radio on this platform
    #endif

    //#include "core/MyTransport.cpp"

    // count enabled transports
    #if defined(MY_RADIO_NRF24)
        #define __RF24CNT 1
    #else
    #define __RF24CNT 0
    #endif
    #if defined(MY_RADIO_RFM69)
    #define __RFM69CNT 1
    #else
    #define __RFM69CNT 0
    #endif
    #if defined(MY_RADIO_RFM95)
    #define __RFM95CNT 1
    #else
    #define __RFM95CNT 0
    #endif
    #if defined(MY_RS485)
    #define __RS485CNT 1
    #else
    #define __RS485CNT 0
    #endif


    #if (__RF24CNT + __RFM69CNT + __RFM95CNT + __RS485CNT > 1)
    #error Only one forward link driver can be activated
    #endif

    #if defined(MY_RADIO_NRF24)
    #if defined(MY_RF24_ENABLE_ENCRYPTION)
    //#include "drivers/AES/AES.cpp"
    #endif
    //#include "drivers/RF24/RF24.cpp"
    //#include "core/MyTransportNRF24.cpp"
    #elif defined(MY_RS485)
    #if !defined(MY_RS485_HWSERIAL)
    #if defined(__linux__)
    #error You must specify MY_RS485_HWSERIAL for RS485 transport
    #endif
    //#include "drivers/AltSoftSerial/AltSoftSerial.cpp"
    #endif
    //#include "core/MyTransportRS485.cpp"
    #elif defined(MY_RADIO_RFM69)
    //#include "drivers/RFM69/RFM69.cpp"
    //#include "core/MyTransportRFM69.cpp"
    #elif defined(MY_RADIO_RFM95)
    //#include "drivers/RFM95/RFM95.cpp"
    //#include "core/MyTransportRFM95.cpp"
    #endif
#endif

#if defined(MY_PARENT_NODE_IS_STATIC) && (MY_PARENT_NODE_ID == AUTO)
#error Parent is static but no parent ID defined.
#endif

// Make sure to disable child features when parent feature is disabled
#if !defined(MY_SENSOR_NETWORK)
#undef MY_OTA_FIRMWARE_FEATURE
#undef MY_REPEATER_FEATURE
#undef MY_SIGNING_NODE_WHITELISTING
#undef MY_SIGNING_FEATURE
#endif

#if !defined(MY_GATEWAY_FEATURE)
#undef MY_INCLUSION_MODE_FEATURE
#undef MY_INCLUSION_BUTTON_FEATURE
#endif

#if !defined(MY_CORE_ONLY)
#if !defined(MY_GATEWAY_FEATURE) && !defined(MY_SENSOR_NETWORK)
    #error No forward link or gateway feature activated. This means nowhere to send messages! Pretty pointless.
#endif
#endif

#include "MyCapabilities.h"
#include "MyMessage.h"
#include "MySensorsCore.h"

#if !defined(MY_CORE_ONLY)
#if defined(ARDUINO_ARCH_ESP8266)
#include "core/MyMainESP8266.cpp"
#elif defined(__linux__)
#include "core/MyMainLinux.cpp"
#else
#endif
#endif

#endif
// Doxygen specific constructs, not included when built normally
// This is used to enable disabled macros/definitions to be included in the documentation as well.
#if DOXYGEN
#define MY_GATEWAY_FEATURE
#define MY_LEDS_BLINKING_FEATURE //!< \deprecated use MY_DEFAULT_RX_LED_PIN, MY_DEFAULT_TX_LED_PIN and/or MY_DEFAULT_ERR_LED_PIN instead **** DEPRECATED, DO NOT USE ****
#endif
    
/* [] END OF FILE */
