#include "uart.h"
#include "init.h"
#include "lin.h"
//Function declaration
inline static void UART_RX_IRQ(uint8_t UART_DR);
inline static void UART_TX_IRQ(void);
static inline void UART_Send(uint8_t data){
  while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE) {asm("nop");}
  UART1->DR = data;
}
//inline static void SetSizeData(enum DataSize iDataSize, struct LIN_Packet packet);
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
inline static void UART_TX_IRQ(void){
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
inline static void UART_RX_IRQ(uint8_t UART_DR){
  volatile uint8_t u8SynchField;
  volatile uint8_t u8PIDField;
  // UART1->DR = UART_DR;
  //static volatile uint8_t UART_DR= UART1->DR; 
  //UART1->DR = UART_DR;
  //UART1->CR2&=~UART1_CR2_RIEN;
  switch(currentHeader){
    case wait_break:
      UART1->CR2&=~UART1_CR2_RIEN;
      return;
    break;

    case wait_synch:
      u8SynchField = UART_DR;
      if(u8SynchField == 0x55U){
        currentHeader = wait_pid;
        header.synch = UART_DR;
#ifdef DEBUG
        UART_Send(u8SynchField);
#endif
        return;
      }
      else{
        currentHeader = wait_break;
#ifdef DEBUG
        UART_Send(0x64);
#endif
        SetExtIRQ();
      }
    break;

    case wait_pid:
      u8PIDField = UART_DR;
      header.pid = UART_DR;
#ifdef DEBUG
      UART_Send(u8PIDField);
#endif
      //u8PIDField = GetPID(u8PIDField);
      if(u8PIDField < 0x20){//2 bytes of data
        Lin_size = bytes_2;
      }
      else if(u8PIDField < 0x30){//4 butes of data
        Lin_size = bytes_4;
      }
      else{//8 bytes of data
        Lin_size = bytes_8;
      }
      header.size = Lin_size;
      currentHeader = wait_break;
      SetExtIRQ();
      //Without parity check!!
        asm("nop");//For debug
    break;
    
    default:
    SetExtIRQ();
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
  uint8_t u8Data = UART1->DR;
  //UART1->DR = u8Data;
  UART_RX_IRQ(u8Data);
}

