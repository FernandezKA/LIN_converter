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