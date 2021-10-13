#include "stm8s_conf.h"
#include "softuart.h"
#include "LIN.h"
uint8_t u8RXB;
int SystemInit(void)
{
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);	// set the highest HSI speed
  //Soft UART
  GPIOD->DDR|=(1<<5);
  GPIOD->CR1|=(1<<5);
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();	
  //LIN (HW UART)
  vLIN_Init();
  return 0;
}
void main(void)
{
  SystemInit();
  for(;;){
    for(int j = 0; j < 0xFF; ++j){
    uart_send(j);
    for(uint16_t i = 0; i < 0xFFFF; ++i) {asm("nop");}
    }
    if(test_status(receive_buffer_full) == receive_buffer_full){
      uart_read(&u8RXB);
      uart_send(u8RXB);
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
