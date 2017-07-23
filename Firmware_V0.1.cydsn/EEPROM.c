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

#ifdef old
void EEPROM_WriteBuf(uint8_t addr, uint16_t subaddr, uint8_t *data, uint8_t len)
{
    EEPROM_WC_Write(0);
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
#endif


void EEPROM_WriteByte(uint16_t addr, uint16_t subaddr, uint8_t data)
{
    uint8_t EEPROM_Writebuffer[3];
    //EEPROM_WC_Write(0);
    EEPROM_Writebuffer[0] = ((subaddr >> 8) & 0xFF);
    EEPROM_Writebuffer[1] = ((subaddr & 0xFF));
    EEPROM_Writebuffer[2] = data;
    I2C_I2CMasterWriteBuf(addr, &EEPROM_Writebuffer[0], 3, I2C_I2C_MODE_COMPLETE_XFER);
    //EEPROM_WC_Write(1);
}

uint8_t EEPROM_ReadByte(uint16_t addr, uint16_t subaddr)
{
    uint8_t buf;
    //EEPROM_Writebuffer[2] = data;
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
    I2C_I2CMasterWriteByte(((subaddr >> 8) & 0xFF));
    I2C_I2CMasterWriteByte(((subaddr & 0xFF)));
    I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE);
    buf = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA);
    I2C_I2CMasterSendStop();
    return buf;
}

void EEPROM_WriteBuffer(uint16_t addr, uint16_t subaddr, uint8_t* data, uint8_t len)
{
    uint16_t pageaddr = ((subaddr & 0xFFE0) >> 5);
    uint8_t byteaddr  = (subaddr & 0x1F);
   
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
    CyDelayCycles(1);
    I2C_I2CMasterWriteByte(((subaddr >> 8) & 0xFF));
    CyDelayCycles(1);
    I2C_I2CMasterWriteByte(((subaddr & 0xFF)));
    CyDelayCycles(1);
    while(len--)
    {
        I2C_I2CMasterWriteByte(*data);
        CyDelayCycles(1);
        data++;
        byteaddr++;
        if(byteaddr == 32 && len>0)
        {
            byteaddr = 0;
            pageaddr++;
            I2C_I2CMasterSendStop();
            CyDelay(7);
            I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
            CyDelayCycles(1);
            subaddr = (pageaddr << 5);
            I2C_I2CMasterWriteByte(((subaddr >> 8) & 0xFF));
            CyDelayCycles(1);
            I2C_I2CMasterWriteByte(((subaddr & 0xFF)));
        }
    }
    I2C_I2CMasterSendStop();
    CyDelay(7);
}

void EEPROM_ReadBuffer(uint16_t addr, uint16_t subaddr, uint8_t* data, uint8_t len)
{
    uint16_t pageaddr = ((subaddr & 0xFFE0) >> 5);
    uint8_t byteaddr  = (subaddr & 0x1F);
    I2C_I2CMasterClearReadBuf();
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
    CyDelayCycles(1);
    I2C_I2CMasterWriteByte(((subaddr >> 8) & 0xFF));
    CyDelayCycles(1);
    I2C_I2CMasterWriteByte(((subaddr & 0xFF)));
    CyDelayCycles(1);
    I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE);
    CyDelayCycles(1);
    while(len--)
    {
        if(len)
            *data = I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA);
        else
            *data = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA);
        CyDelayCycles(1);
        data++;
        byteaddr++;
        if(byteaddr == 100 && len>0)
        {
            byteaddr = 0;
            pageaddr++;
            I2C_I2CMasterSendStop();
            CyDelayCycles(1);
            I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE);
            CyDelayCycles(1);
            subaddr = (pageaddr << 5);
            I2C_I2CMasterWriteByte(((subaddr >> 8) & 0xFF));
            CyDelayCycles(1);
            I2C_I2CMasterWriteByte(((subaddr & 0xFF)));
            CyDelayCycles(1);
            I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE);
            CyDelayCycles(1);
        }
    }
    I2C_I2CMasterSendStop();
}



