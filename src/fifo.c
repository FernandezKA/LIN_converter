//Includes
#include "fifo.h"
//User variable declarations
//User function definitions
//This function get data from fifo
uint8_t Pull(FIFO *fifo)
{
  fifo->isFull = false;
  uint8_t u8Data = fifo->u8Data[fifo->u8Tail++];
  if (fifo->u8Tail == fifo->u8Head)
  {
    fifo->isEmpty = true;
    fifo->u8Head = fifo->u8Tail = 0x00U;
  }
  return u8Data;
}
//This function put new data into fifo
void Push(FIFO *fifo, uint8_t data)
{
  if (fifo->u8Head - fifo->u8Tail == FIFO_LENGTH)
  {
    fifo->isFull = true;
  }
  else
  {
    fifo->u8Data[fifo->u8Head++] = data;
    fifo->isEmpty = false;
  }
}