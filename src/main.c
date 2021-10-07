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
  CLK->CKDIVR = 0;
  CLK->PCKENR1|= 1<<5;
  GPIOD->DDR|=(1<<4);
  GPIOD->CR1|=(1<<4);
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();	
    return 0;
}
uint8_t buff, sts;
uint8_t tx_byte;
void main(void)
{
  SystemInit();
  for(;;){
    sts= uart_send(0x55U);
    while(test_status(transmit_in_progress) == transmit_in_progress){//Wait while TXNE
      asm("nop");
    }
    for(uint8_t i = 0; i < 0xFF; ++i) {asm("nop");}
    sts =uart_read(&buff);
   if(test_status(receive_buffer_full) == receive_buffer_full){
      sts =uart_read(&buff);
      uart_send(buff);
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
