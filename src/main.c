#include "stm8s_conf.h"
#include "lin.h"
#include "uart.h"
void main(void)
{
  UART_Init();  
  asm("rim");
  for(;;){
    for(uint16_t i = 0x00; i < 0xFFFF; ++i){asm("nop");}
    UART_Transmit_IT((uint8_t*) "Hello\n\r", 7);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
