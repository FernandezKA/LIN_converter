#include "stm8s_conf.h"
#include "stm8s_itc.h"
//User includes
#include "init.h"
#include "lin.h"
#include "uart.h"
#include "fifo.h"
//Includes for SPL library
#include "stm8s_tim2.h"
#include "stm8s_gpio.h"
//Function declaration
static void SysInit(void);
FIFO sw_transmit;
void main(void)
{
  SysInit();
  currentHeader = wait_break;
  sw_transmit.isEmpty = true;
  asm("rim");
  for (;;)
  {
    if(!sw_transmit.isEmpty){
      while(!sw_transmit.isEmpty){
        uart_send(Pull(&sw_transmit));
      }
    }
    asm("nop");
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
  asm("rim");
}
