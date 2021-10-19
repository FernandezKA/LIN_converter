/*****************
Lin simple protocol:
1 byte Header + 1 byte PID + 1 byte command + 2 bytes data
****************/

#include "Lin_Driver.h"


uint8_t u8StoreCrc[2] = {0,0};
uint8_t u8CountGestture = 0;
uint8_t u8TransmitData;




/**********************
u8Header TPS header(lmm + device id + regaddr)
u8Packet TPS data
************************/
uint8_t *Lin_u8CalCrc16(uint8_t *u8Header,uint8_t *u8Packet,uint8_t u8NumberHeader,uint8_t u8NumberPacket)
{
  /*uint8_t u8CountNumber;
  uint16_t u16CrcValue = 0;
  u16CrcValue = CRC16_PRESET;
  for(u8CountNumber = 0;u8CountNumber<(u8NumberHeader+u8NumberPacket);u8CountNumber++)
  {
    uint8_t u8CountBit = CRC16_BITS;
    if(u8CountNumber<u8NumberHeader)
    {
      u16CrcValue ^= *(u8Header+u8CountNumber);
    }
    else
    {
      u16CrcValue ^= *(u8Packet+u8CountNumber-u8NumberHeader);
    }
    while(u8CountBit)
    {
      u8CountBit--;
      if(u16CrcValue & 0x01)
      {
        u16CrcValue = (u16CrcValue>>1)^CRC16_POLYNOM;
      }
      else
      {
        u16CrcValue = (u16CrcValue>>1)^0;
      }
    }
  }
  u8StoreCrc[0] = (uint8_t)u16CrcValue;
  u8StoreCrc[1] = (uint8_t)(u16CrcValue>>8);*/
  return u8StoreCrc;
}

void Lin_vidSendBytes(uint8_t u8Bytes,uint8_t *pu8SendByes)
{
  uint8_t u8CountBytes;
  //uint8_t u8TransmitData;
  for(u8CountBytes=0;u8CountBytes<u8Bytes;u8CountBytes++)
  {
    //while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
    while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE){asm("nop");}
    u8TransmitData = *(pu8SendByes+u8CountBytes);
    UART1_SendData8(u8TransmitData);

  }
}

uint8_t Lin_parity(uint8_t data)
{
  uint8_t bitLoctaion;
  uint8_t bitData[8] = {0};
  uint8_t tempData;
  uint8_t temp;
  
  for(bitLoctaion=0;bitLoctaion<8;bitLoctaion++)
  {
    bitData[bitLoctaion] = (data>>bitLoctaion);
    bitData[bitLoctaion]&=0x01;
  }
  bitData[6] = bitData[0]^bitData[1]^bitData[2]^bitData[4];
  bitData[7] = bitData[1]^bitData[3]^bitData[4]^bitData[5]^0x01;
  tempData = 0;
  for(bitLoctaion=8;bitLoctaion>0;bitLoctaion--)
  {
    temp=(bitData[bitLoctaion-1]<<(bitLoctaion-1));
    tempData+=temp;
  }
  return tempData;
}

void Lin_vidDecodeMsg(uint8_t *pu8GetMsg)
{
  if(*pu8GetMsg == 0x55)        //Header correctly received
  {
    if(*(pu8GetMsg+1)==Lin_parity(*(pu8GetMsg+1))) // PID is correct
    {
      //upacMsg.msgCmd = *(pu8GetMsg+2);
      //upacMsg.msgData = (uint16_t)*(pu8GetMsg+3)<<8 + *(pu8GetMsg+4);
    }
  }
  
}



void Lin_RunMode()
{ 
  /*Lin_vidDecodeMsg(u8UartMessage);
  switch(upacMsg.msgCmd){
    case 0x00:          //LB mode
      break;
    case 0x01:          //HB mode
      break;
    default:
      break;
  
  }
  Lin_vidSendBytes(2,u8StoreCrc);
  */

}