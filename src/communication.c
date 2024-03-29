#include "communication.h"
#include "fifo.h"
#include "softuart.h"
#include "init.h"

void uartSend(uint8_t data){
  while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty){asm("nop");}
  uart_send(data);
}

bool isSecondDigit = false;
uint8_t resValue = 0x00;
enum PartDigit ValueReceived = value_completed;
//Send LIN packet into RS232
void Reflect_LIN(LIN_Header header, LIN_Response response, bool isCorrect)
{
  asm("rim");
  LinPrint(header.pid);
  //Push(&sw_transmit, header.size);
  for (int i = 0; i < header.size; ++i)
  {
    LinPrint(response.data[i]);
  }
  if(!isCorrect){
    uartSend('#');
    uartSend(' ');
  }
  LinPrint(response.CRC);
  uartSend('\n');
  uartSend('\r');
  asm("sim");
}
//Convers two ASCII symbols to hex value (unsigned)
enum PartDigit GetHex(char digit)
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
//Convers hex valie to ASCII numbers
void GetASCII(uint8_t value, uint8_t* s1, uint8_t* s2){
  uint8_t firstDigit = value >> 4;
  uint8_t secondDigit = value & 0x0F;
  if(firstDigit < 10){
    *s1 = firstDigit + 0x30;
  }
  else{
    *s1 = firstDigit + 55;    
  }
    if(secondDigit < 10){
    *s2 = secondDigit + 0x30;
  }
  else{
    *s2 = secondDigit + 55;    
  }
}
//Send LIN recognized packet
void LinPrint(uint8_t val){
  uint8_t firstDigit, secondDigit;
  GetASCII(val, &firstDigit, &secondDigit);
  while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty){asm("nop");}
  uart_send(firstDigit);
  while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty){asm("nop");}
  uart_send(secondDigit);
  while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty){asm("nop");}
  uart_send(' ');
}
