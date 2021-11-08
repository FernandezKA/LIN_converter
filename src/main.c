#include "stm8s_conf.h"
#include "lin.h"
#include "uart.h"
void main(void)
{
  UART_Init();  
  asm("rim");
  for(;;){
    UART_Receive_IT(u8RxData, 5);
    while(!bReceived){
      asm("nop");
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif
