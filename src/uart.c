#include "uart.h"
//Variables
uint8_t  u8RxCnt, u8TxCnt;
uint8_t  u8RxSize, u8TxSize;

uint8_t u8RxData[RxBufSize];
uint8_t u8TxData[TxBufSize];
bool bTransmitted = false;
bool bReceived = false;
//UART initializations
void UART_Init(void){
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; //ENABLE CLOCKING
  UART1->BRR1 = 0x68;
  UART1->BRR2 = 0x02;
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
}
//This function configure recieve for IRQ RXNE
bool UART_Receive_IT(uint8_t* u8Buf, uint8_t u8Size){
  bReceived = false;
  u8RxCnt = 0x00;
  u8RxSize = u8Size;
  UART1->CR2|=UART1_CR2_REN;
  UART1->CR2|=UART1_CR2_RIEN;
  return true;
}
//This function configure transmit for IRQ TXCIE
bool UART_Transmit_IT(uint8_t* u8Buf, uint8_t u8Size){
  bTransmitted = false;
  u8TxCnt = 0x00;
  u8TxSize = u8Size;
  UART1->CR2|=UART1_CR2_TIEN;//Enable IRQ for complete send
  return true;
}
//This function clear RXE buff, and disable it
void UART_AbortReceive(void){
  asm("nop");
}
//TX IRQ handler
void UART_TX_IRQ(void){
  if(u8TxCnt < u8TxSize){
    UART1->DR = u8TxData[u8TxCnt++];
  }
  else{
    bTransmitted = true;
    u8TxCnt = 0x00;
    u8TxSize = 0x00;
    UART1->CR2&=~UART1_CR2_TIEN;
  }
}
//RX IRQ handler
void UART_RX_IRQ(void){
  UART1->SR&=~UART1_SR_RXNE;
      uint8_t u8Temp = UART1->DR;
  switch(currentHeader){
    case wait_break:
      UART1->CR2&=~UART1_CR2_REN;//It's mistake IRQ, disable UART
    break;

    case wait_synch:
      if(u8Temp == 0x55U){
        currentHeader = wait_pid;
      }
      else{
        UART1->CR2|=UART1_CR2_TEN;
        UART1->DR = 0x55U;
        currentHeader = wait_break;
        UART1->CR2&=~UART1_CR2_REN;
      }
    break;

    case wait_pid:
      asm("nop");//Successfull recognize of packet, __NOP instruction for debug
    break;

    default:

    break;
  }
}
//UART1 TX Interrupt routine.
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
	UART_TX_IRQ();
}

//UART1 RX Interrupt routine.
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
	UART_RX_IRQ();
}