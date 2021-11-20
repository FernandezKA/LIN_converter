#include "init.h"
//User function declarations
//This function configurate clocking
void Clk_Config(void)
{
  CLK->CKDIVR = 0x00;
}
//This function configurate software UART
void UART_SW_Config(void)
{
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  //enableInterrupts();
}
//This function configurate hardware UART
void UART_HW_Config(void)
{
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; //ENABLE CLOCKING
  UART1->BRR1 = 0x68;
  UART1->BRR2 = 0x02;
  UART1->CR2 |= UART1_CR2_TEN;
}
//This function configured Tim1 for stop detection
void Tim1_Config(void)
{
  TIM1->PSCRH = 0x00;
  TIM1->PSCRL = 0x10;
  TIM1->ARRH = 0x0F;
  TIM1->ARRL = 0xFF;
  TIM1->CR1 |= TIM1_CR1_CEN;
}
//This function configured GPIO
void GPIO_Config(void)
{
  GPIOD->DDR &= ~(1 << 6);
  GPIOD->CR1 |= (1 << 6); //Input with pull-up
  GPIOD->CR2 |= (1 << 6); //Enable external interrupt
  GPIOB->DDR |= (1 << 5);
}
//This function disable UART, and begin wait a break
void SetExtIRQ(void)
{
  UART1->CR2 &= ~UART1_CR2_REN;
  UART1->CR2 &= ~UART1_CR2_RIEN;
  UART_PORT->CR1 |= UART_RX;
  UART_PORT->CR2 |= UART_RX;
  EXTI->CR1 |= EXTI_CR1_PDIS; //Enable IRQ for all of change edge
}
//This function enable wait IRQ mode for synch packet
void SetSynchMode(void)
{
  UART_PORT->CR2 &= ~UART_RX; //Dis IRQ
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
  UART1->CR2 |= UART1_CR2_RIEN; //Enable IRQ for receive synch packet
  asm("rim");
}
