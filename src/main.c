#include "stm8s_conf.h"
#include "stm8s_itc.h"
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
  currentHeader = wait_break;
  asm("rim");
  for (;;)
  {
    asm("nop");
    //uart_send(response.CRC);
  }
}


#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif
//This function combined all of init function
static void SysInit(void)
{
  Clk_Config();
  UART_SW_Config();
  UART_HW_Config();
  Tim1_Config();
  GPIO_Config();
  SetExtIRQ();
  ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_1);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_2);
  asm("rim");
}
