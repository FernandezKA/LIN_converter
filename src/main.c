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
static FIFO sw_receive;
void main(void)
{
  SysInit();
  currentHeader = wait_break;
  sw_transmit.isEmpty = true;
  sw_receive.isEmpty = true;
  asm("rim");
  for (;;)
  {
    if(test_status(receive_buffer_full) == receive_buffer_full){//FIFO buffer for RS232
      uint8_t u8Data;
      uart_read(&u8Data);
      Push(&sw_receive, u8Data);
      uart_send(Pull(&sw_receive));
    }
    if (!sw_transmit.isEmpty)//Lin packet recognized, reflect from RS232
    {
      if(test_status(transmit_data_reg_empty) == transmit_data_reg_empty)
      {
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
