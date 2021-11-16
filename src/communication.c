#include "communication.h"
#include "softuart.h"
void Reflect_LIN(LIN_Header header, LIN_Response response){
  uart_send(header.pid);
  uart_send(header.size);
  for(uint8_t i = 0; i < header.size; ++i){
    uart_send(response.data[i]);
  }
  uart_send(response.CRC);
}