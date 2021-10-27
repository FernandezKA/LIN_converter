#ifndef _uart_h_
#define _uart_h_
//Includes 
#include <stm8s.h>
#include <stdlib.h>
//Defines
//Users functions
void UART_Init(void);
bool UART_Receive_IT(uint8_t* u8Buf, uint8_t u8Size);
bool UART_Transmit_IT(uint8_t* u8Buf, uint8_t u8Size);
void UART_AbortReceive(void);
void UART_RX_IRQ(void);
void UART_TX_IRQ(void);
//Pointer to functions
extern void (*UART_IRQ) (void);//UART IRQ Handlers
//Variables
extern uint8_t *pRxBuf, *pTxBuf;
extern uint8_t  u8RxCnt, u8TxCnt;
extern uint8_t  u8RxSize, u8TxSize;
//Function definitions

#endif