#include "stm8s_conf.h"
//User includes
#include "init.h"
#include "lin.h"
#include "uart.h"
//Includes for SPL library
#include "stm8s_tim2.h"
#include "stm8s_gpio.h"
//Function declaration
static void SysInit(void);
void main(void)
{
  SysInit();
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
static void SysInit(void){
  Clk_Config();
  Tim1_Config();
  GPIO_Config();
  //UART_HW_Config();  
  //UART_SW_Config();
  asm("rim");
}
