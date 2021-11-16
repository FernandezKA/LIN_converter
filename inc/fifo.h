#ifndef _fifo_h_
#define _fifo_h_
//Includes
#include "stm8s_conf.h"
//User definitions
#define FIFO_LENGTH 16U
//Variable declaration
struct FIFO_STR {
  uint8_t u8Tail;
  uint8_t u8Head;
  uint8_t u8Data[FIFO_LENGTH];
  bool isEmpty;
  bool isFull;
};
typedef struct FIFO_STR FIFO;
//Function declaration
uint8_t Pull(FIFO* fifo);
void    Push(FIFO* fifo, uint8_t data);
#endif