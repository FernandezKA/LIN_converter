#include "stm8s_conf.h"
#include "LIN.h"
//User init function
//This function configure UART at LIN mode with default BAUDRATE 9600
void vLIN_Init(void){
  //Set default BAUDRATE 9600
  uint16_t u16BAUD = 9600;
  uint16_t u16Prescale = F_CPU/u16BAUD;
  UART1->BRR1 = (u16Prescale << 4) >> 8;
  UART1->BRR2 = ((u16Prescale & 0xF000 ) >> 12 ) | (u16Prescale & 0x000F);
  UART1->CR3 |= UART1_CR3_LINEN;//enable lin mode
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;//enable reciever and transmitter
}
//This function edit default BAUDRATE on current
void vLIN_BAUD(uint16_t u16BAUD){
  uint16_t u16Prescale = F_CPU/u16BAUD;
  UART1->BRR1 = (u16Prescale << 4) >> 8;
  UART1->BRR2 = ((u16Prescale & 0xF000 ) >> 12 ) | (u16Prescale & 0x000F);
}
//This function send data from LIN
void vLIN_Send(uint8_t u8Data){
  
}
//LIN header tranmission
void vLIN_header(void){
UART1->CR2|=UART1_CR2_SBK;
UART1->DR = 0x55U;
while((UART1->SR & UART1_SR_TC) != UART1_SR_TC) asm("nop");
}
