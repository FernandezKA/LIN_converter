#include "stm8s_conf.h"
#include "lin.h"
#include "uart.h"
void main(void)
{
  CLK->CKDIVR = 0x00;
  UART_Init();  
  asm("rim");
  for(;;){
    UART_Receive_IT(u8RxData, 5);
    while(!bReceived){
      asm("nop");
    }
    asm("nop");
    //UART_Transmit_IT(u8TxData, 5);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
