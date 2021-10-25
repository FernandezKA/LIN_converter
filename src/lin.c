#include "lin.h"
//Function declarations
bool bBreakDetect(void){
  switch(BAUDRATE){
  case 9600:
    asm("nop");
    break;
  case 19200:
    asm("nop");
    break;
  default:
    asm("nop");
    break;
  }
}