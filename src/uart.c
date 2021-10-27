#include "uart.h"
//Variables
uint8_t *pRxBuf, *pTxBuf;
uint8_t  u8RxCnt, u8TxCnt;
uint8_t  u8RxSize, u8TxSize;
void (*UART_IRQ) (void);
//Function definitions
//UART initializations
void UART_Init(void){
  UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(ENABLE);
}
//This function configure recieve for IRQ RXNE
bool UART_Receive_IT(uint8_t* u8Buf, uint8_t u8Size){
  
  UART_IRQ = UART_RX_IRQ;//Set IRQ handler -> RX
  return true;
}
//This function configure transmit for IRQ TXCIE
bool UART_Transmit_IT(uint8_t* u8Buf, uint8_t u8Size){
  u8TxCnt = 0x00;
  u8TxSize = u8Size;
  pTxBuf = (uint8_t*) malloc(u8Size);
  for(uint8_t j = 0; j < u8TxSize; j++){
    pTxBuf[j] = u8Buf[j];
  }
  UART1->CR2|=UART1_CR2_TCIEN;//Enable IRQ for complete send
  //UART1->DR = u8Buf[u8TxCnt++];//Send first byte, next -> from IRQ
  UART_IRQ = UART_TX_IRQ;//Set IRQ handler -> TX
  return true;
}
//This function clear RXE buff, and disable it
void UART_AbortReceive(void){
  asm("nop");
}
//TX IRQ handler
void UART_TX_IRQ(void){
  if(u8TxCnt < u8TxSize){
    UART1->DR = u8Data[u8TxCnt++];
  }
  else{
    u8TxCnt = 0x00;
    u8TxSize = 0x00;
    UART1->CR2&=~UART1_CR2_TCIEN;
  }
}
//RX IRQ handler
void UART_RX_IRQ(void){
  asm("nop");
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