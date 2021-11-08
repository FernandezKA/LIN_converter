#include "init.h"
//User function declarations
//This function configurate clocking
void Clk_Config(void){
  CLK->CKDIVR = 0x00;
}
//This function configurate software UART
void UART_SW_Config(void){
  
}
//This function configurate hardware UART
void UART_HW_Config(void){
  
}