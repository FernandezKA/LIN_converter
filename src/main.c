#include "stm8s_conf.h"
#include "lin.h"
#include "uart.h"
void main(void)
{
  for(uint8_t i = 0; i < TxBufSize; ++i){
    u8TxData[i] = i;
  }
  UART_Init();  
  asm("rim");
  for(;;){
    for(uint16_t i = 0x00; i < 0xFFFF; ++i){asm("nop");}
    UART_Receive_IT(u8RxData, 5);
    while(!bReceived){
      asm("nop");
    }
    UART_Transmit_IT(u8TxData, 5);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
