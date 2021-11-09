#include "init.h"
//User function declarations
//This function configurate clocking
void Clk_Config(void){
  CLK->CKDIVR = 0x00;
}
//This function configurate software UART
void UART_SW_Config(void){
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();	
}
//This function configurate hardware UART
void UART_HW_Config(void){
  
}
//This function configured Tim1 for stop detection
void Tim1_Config(void){
  TIM1->PSCRH = 0x00;
  TIM1->PSCRL = 0x10;
  TIM1->ARRH = 0xFF;
  TIM1->ARRL = 0xFF;
  TIM1->CR1|=TIM1_CR1_CEN;
}
//This function configured GPIO
void GPIO_Config(void){
    GPIOD->DDR&=~(1<<6);
    GPIOD->CR1|=(1<<6);//Input with pull-up
    GPIOD->CR2|=(1<<6);//Enable external interrupt
}