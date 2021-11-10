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
static inline void SetExtIRQ(void);
static inline void SetSynchMode(void);
void main(void)
{
  SysInit();
  GPIOC->DDR|=(1<<7);
  GPIOC->CR1|=(1<<7);
  GPIOC->CR2|=(1<<7);
  for (;;)
  {
    switch (currentHeader)
    {
    case wait_break:
      SetExtIRQ();
      break;

    case wait_synch:
      UART_HW_Config();
      SetSynchMode();
      break;

    case wait_pid:

      break;

    default:

      break;
    }
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
  Tim1_Config();
  GPIO_Config();
  EXTI->CR1|=EXTI_CR1_PDIS;
  //UART_HW_Config();
  //UART_SW_Config();
  asm("rim");
}
//This function disable UART, and begin wait a break
static inline void SetExtIRQ(void){
  UART1->CR2&=~UART1_CR2_REN;
  UART_PORT->CR1|=UART_RX;
  UART_PORT->CR2|=UART_RX;
  EXTI->CR1|=EXTI_CR1_PDIS;//Enable IRQ for all of change edge
}
//This function enable wait IRQ mode for synch packet
static inline void SetSynchMode(void){
  UART1->CR2|=UART1_CR2_REN;
  //Todo: enable IRQ and wait a new byte
  UART1->CR2|=UART1_CR2_RIEN;
  asm("rim");
}