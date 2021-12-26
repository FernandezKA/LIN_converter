#include "communication.h"
#include "fifo.h"
#include "softuart.h"

bool isSecondDigit = false;
uint8_t resValue = 0x00;
enum PartDigit ValueReceived = value_completed;
//Send LIN packet into RS232
void Reflect_LIN(LIN_Header header, LIN_Response response)
{
  Push(&sw_transmit, header.pid);
  Push(&sw_transmit, header.size);
  for (int i = 0; i < header.size; ++i)
  {
    Push(&sw_transmit, response.data[i]);
  }
  Push(&sw_transmit, response.CRC);
}
//Convers two ASCII symbols to hex value (unsigned)
enum PartDigit GetDigit(char digit)
{
  if (ValueReceived == value_incompleted){
    ValueReceived = value_completed;
    if (digit >= '0' && digit <= '9')
    {
      digit -= 48;
    }
    else if (digit >= 'a' && digit <= 'f')
    {
      digit -= 87;
    }
    else if (digit >= 'A' && digit <= 'F')
    {
      digit -= 55;

    }
    resValue|=digit;
    return ValueReceived;
  }
  else if (ValueReceived == value_completed)
  {
    ValueReceived = value_incompleted;
    if (digit >= '0' && digit <= '9')
    {
      digit -= 48;
      digit=digit<<4;
    }
    else if (digit >= 'a' && digit <= 'f')
    {
      digit -= 87;
      digit=digit<<4;
    }
    else if (digit >= 'A' && digit <= 'F')
    {
      digit = digit - 55;
      digit=digit<<4;
    }
    resValue=digit;
    return ValueReceived;
  }
  else{
    return value_incompleted;
  }
}