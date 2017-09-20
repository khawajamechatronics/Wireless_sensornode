#ifndef MyHwAVR_h
#define MyHwAVR_h

#include <project.h>
#include "MyHw.h"
#include "MySensorsCore.h"
#include "string.h"
    
#define USB
#define USBFS_DEVICE    (0u)
#define USBUART_BUFFER_SIZE (64u)
#define LINE_STR_LENGTH     (20u)

//char8* parity[] = {"None", "Odd", "Even", "Mark", "Space"};
//char8* stop[]   = {"1", "1.5", "2"};
    
// mapping
#define snprintf_P snprintf
#define vsnprintf_P vsnprintf
    
// emulated EEPROM (experimental)
#define EEPROM_PAGE_SIZE		(uint16_t)1024
#define EEPROM_SIZE				(uint16_t)2048
#define EEPROM_START_ADDRESS	((uint32_t)(0x8000000 + 128 * EEPROM_PAGE_SIZE - 2 * EEPROM_PAGE_SIZE))

#define RFM69_NOP			    0x00
#define RFM69_MAX_PACKET_LEN    (0x40u)
    
typedef enum {
    RX_LED = 0,
    TX_LED = 1,
    ERR_LED = 2,
    NRF24_CSN = 3,
    NRF24_CE = 4
} outputs;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
//#define snprintf_P(s, f, ...) snprintf((s), (f), __VA_ARGS__)
#define PSTR(x) (x)

uint32_t    MyHwPsoc_Random(uint32_t min, uint32_t max);
void        MyHwPsoc_init(void);
void        MyHwPsoc_RandomNumberInit(void);
void        MyHwPsoc_ReadConfigBlock(void* buf, void* addr, size_t length);
uint8_t     MyHwPsoc_ReadConfig(const int addr);
void        MyHwPsoc_WriteConfigBlock(void* buf, void* addr, size_t length);
void        MyHwPsoc_WriteConfig(const int addr, uint8_t buf);
void        MyHwPsoc_Reboot(void);
void        MyHwPsoc_SetOutput(outputs pin, uint8_t state);
uint8_t     MyHwPsoc_SpiTransfer(uint8_t data);
void        MyHwPsoc_SpiEndTransaction(void);
uint16_t    MyHwPsoc_hwMillis(void);
void        MyHwPsoc_SerialBegin(void);
void        MyHwPsoc_SerialPrint(char *buf);
uint8_t     MyHwPsoc_SerialAvailable(void);
uint8_t     MyHwPsoc_SerialRead(void);

uint8_t MyHwPsoc_RFM69_spiMultiByteTransfer(const uint8_t cmd, uint8_t* buf, uint8_t len, const bool aReadMode);
// low level register access
uint8_t MyHwPsoc_RFM69_RAW_readByteRegister(const uint8_t address);
// low level register access
uint8_t MyHwPsoc_RFM69_RAW_writeByteRegister(const uint8_t address, uint8_t value);


void        pinMode(uint8_t x,uint8_t y);
void        digitalWrite(uint8_t x,uint8_t y);
uint8_t     digitalRead(uint8_t x);

#endif
    