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

int SystemInit(void)
{
  uart_init();
  uart_receive_enable;
  enableInterrupts();	
    return 0;
}

void main(void)
{
	SystemInit();
        uint8_t u8TXb = 0xAA;
  for(;;){
  uart_send(u8TXb);
while(test_status(transmit_in_progress) != 0);// wait for its transmition complete
}
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
