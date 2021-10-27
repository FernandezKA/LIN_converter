#include "stm8s_conf.h"
#include "lin.h"
#include "uart.h"
uint8_t u8Data[5] = {0x55, 0xFF, 0x64, 0xFF, 0x55};  
void main(void)
{
  UART_Init();
  asm("rim");
  for(;;){
    for(uint16_t i = 0x00; i < 0xFFFF; ++i){asm("nop");}
    UART_Transmit_IT(u8Data, 5);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
