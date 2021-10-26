#ifndef _uart_h_
#define _uart_h_
//Includes 
#include <stm8s.h>

//Defines
bool UART_Receive_IT(uint8_t* u8Buf, uint8_t u8Size);
bool UART_Transmit_IT(uint8_t* u8Buf, uint8_t u8Size);
void UART_AbortReceive(void);
//Variables
extern uint8_t* pRxBuf, pTxBuf;
extern uint8_t  u8RxCnt, u8TxCnt;
//Function definitions

#endif