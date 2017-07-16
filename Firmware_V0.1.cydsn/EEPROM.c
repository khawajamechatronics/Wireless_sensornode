/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */

#include "EEPROM.h"

void EEPROM_WriteBuf(uint8_t addr, uint16_t subaddr, uint8_t *data, uint8_t len)
{
    
    //I2C_I2CMasterWriteBuf(addr, data, len, I2C_I2C_MODE_COMPLETE_XFER);
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
    uint8_t HighByte, LowByte;
    HighByte = ((subaddr >> 8)&0xFF);
    LowByte = (subaddr & 0xFF);
    I2C_I2CMasterWriteByte(HighByte);
    I2C_I2CMasterWriteByte(LowByte);
    while(len--)
    {
        I2C_I2CMasterWriteByte((uint32_t)*data);
        data++;
    }
    I2C_I2CMasterSendStop();
    
}

uint8_t EEPROM_ReadBuf(uint8_t addr, uint16_t subaddr, uint8_t *data, uint8_t len)
{
    //I2C_I2CMasterWriteBuf(addr, data, len, I2C_I2C_MODE_COMPLETE_XFER);
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
    uint8_t HighByte, LowByte;
    HighByte = ((subaddr >> 8)&0xFF);
    LowByte = (subaddr & 0xFF);
    I2C_I2CMasterWriteByte(HighByte);
    I2C_I2CMasterWriteByte(LowByte);
    I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE);
    len--;
    while(len--)
    {
        *data = I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA);
        data++;
    }
    *data = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA);
    I2C_I2CMasterSendStop();
    return 0;
}