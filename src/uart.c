#include "uart.h"

bool UART_Receive_IT(uint8_t* u8Buf, uint8_t u8Size){
  asm("nop");
}

bool UART_Transmit_IT(uint8_t* u8Buf, uint8_t u8Size){
  asm("nop");
}

void UART_AbortReceive(void){
  asm("nop");
}
