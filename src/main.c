#include "stm8s_conf.h"
#include "softuart.h"
/**
  *inc->stm8s_conf.h - раскоментировать строчки с нужными хедерами 
  *периферии и необходимыми обработчиками прерываний. Все закоментированные
  *обработчики ведут на бесконечные циклы.
  * 
  *Project->Options->General Options - выбрать мк
  *
  *Project->Options->Debugger - выбрать отладчик
  *
  *Project->Options->C/C++ Compiler->Preprocessor->Defined symbols  - задать
  *семейство процессора(перечислены в lib->SPL->inc->stm8s.h), а также задать
  *частоты внутренних и внешних генераторов(если не задать, то будут ипользованы
  *значения по умолчанию из stm8s.h).
  */
uint8_t u8RXB;
int SystemInit(void)
{
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);	// set the highest HSI speed
  GPIOD->DDR|=(1<<5);
  GPIOD->CR1|=(1<<5);
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();	
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
