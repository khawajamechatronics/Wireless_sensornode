#ifndef MyHwAVR_h
#define MyHwAVR_h

#include <project.h>
    
typedef enum {
    RX_LED = 0,
    TX_LED = 1,
    ERR_LED = 2,
    NRF24_CSN = 3,
    NRF24_CE = 4
} outputs;

uint32_t    MyHwPsoc_Random(uint32_t min, uint32_t max);
void        MyHwPsoc_init(void);
void        MyHwPsoc_RandomNumberInit(void);
void        MyHwPsoc_ReadConfigBlock(void* buf, uint16_t addr, size_t length);
uint8_t     MyHwPsoc_ReadConfig(uint16_t addr);
void        MyHwPsoc_WriteConfigBlock(void* buf, uint16_t addr, size_t length);
void        MyHwPsoc_WriteConfig(uint16_t addr, uint8_t buf);
void        MyHwPsoc_Reboot(void);
int8_t      MyHwPsoc_hwSleep(unsigned long ms);
int8_t      MyHwPsoc_hwSleep1(uint8_t interrupt, uint8_t mode, unsigned long ms);
int8_t      MyHwPsoc_hwSleep2(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms);
void        MyHwPsoc_LedSet(outputs pin, uint8_t state);
uint8_t     MyHwPsoc_SpiTransfer(uint8_t data);
void        MyHwPsoc_SpiEndTransaction(void);

#endif
    