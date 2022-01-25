#include "init.h"
// User function declarations
static uint16_t CounterIndicate = 0x00;
// This function configurate clocking
void Clk_Config(void)
{
  CLK->CKDIVR = 0x00;
}
// This function configurate software UART
void UART_SW_Config(void)
{
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  // enableInterrupts();
}
// This function configurate hardware UART
void UART_HW_Config(void)
{
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; // ENABLE CLOCKING
  if (BAUD_LIN == 9600)
  {
    UART1->BRR1 = 0x68;
    UART1->BRR2 = 0x02;
  }
  else
  {
    UART1->BRR1 = 0x34;
    UART1->BRR2 = 0x02;
  }
  UART1->CR4 |= UART1_CR4_LBDL;
  UART1->CR2 |= UART1_CR2_TEN;
}
// This function configured Tim1 for stop detection
void Tim1_Config(void)
{
  TIM1->PSCRH = 0x00;
  TIM1->PSCRL = 0x10;
  TIM1->ARRH = 0x0F;
  TIM1->ARRL = 0xFF;
  TIM1->CR1 |= TIM1_CR1_CEN;
}
//This function configurate Tim4 for indicate activity
void Tim4_Config(void){
  TIM4->PSCR = 7;//2^7 PRESCALER
  TIM4->ARR = 100; //OVF EVERY 810 uS (~ 1 mS)
  TIM4->IER|=TIM4_IER_UIE;
  TIM4->CR1|=TIM4_CR1_CEN;
}
// This function configured GPIO
void GPIO_Config(void)
{
  GPIOD->DDR &= ~(1 << 6);
  GPIOD->CR1 |= (1 << 6); // Input with pull-up
  GPIOD->CR2 |= (1 << 6); // Enable external interrupt
  GPIOB->DDR |= (1 << 5);
}
// This function disable UART, and begin wait a break
void SetExtIRQ(void)
{
  UART1->CR2 &= ~UART1_CR2_REN;
  UART1->CR2 &= ~UART1_CR2_RIEN;
  UART_PORT->CR1 |= UART_RX;
  UART_PORT->CR2 |= UART_RX;
  EXTI->CR1 |= EXTI_CR1_PDIS; // Enable IRQ for all of change edge
}
// This function enable wait IRQ mode for synch packet
void SetSynchMode(void)
{
  UART_PORT->CR2 &= ~UART_RX; // Dis IRQ
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
  UART1->CR2 |= UART1_CR2_RIEN; // Enable IRQ for receive synch packet
  asm("rim");
}
//This function for indicate of activity
void GetIndicate(void){
  ++CounterIndicate;
  if(BAUD_LIN == 9600){
    if(CounterIndicate%400 == 0){
      LED_PORT ->ODR^=(LED_PIN);
    }
    if(CounterIndicate == 60000){
      CounterIndicate = 0;
    }
  }
  else{
    if(CounterIndicate%200 == 0){
      LED_PORT ->ODR^=(LED_PIN);
    }
    if(CounterIndicate == 60000){
      CounterIndicate = 0;
    }
  }
}
//This function calcuate CRC
void CRC8(uint8_t *CRC, uint8_t data,bool isEnd)
{
 //uint16_t sum = *CRC + data;
 //if(sum >= 0xFF){
 //  sum += 1;
 //}
 //*CRC = sum & 0xFF; 
 if(!isEnd){
   *CRC ^= 0xFF; 
 }
 else{
   *CRC^=0xFF;
 }
}