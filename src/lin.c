/* Includes ------------------------------------------------------------------*/

#include "lin.h"
#include "uart.h"

//User function declaration

//User variables
uint16_t u16BreakLength;
LIN_HEADER currentHeader;
//This function detect break from measured pulse structure
bool BreakDetection(void){
  
}
//This function recognize sync field into the frame
bool GetSynch(uint8_t data){
  
}
//This function receive PID frame
uint8_t GetPID(void){
  
}
//This function switch FSM states
void GetNextState(LIN_HEADER* current){
  switch(*current){
  case wait_break:
    
    break;
  case wait_synch:
    
    break;
  case wait_pid:
    
    break;
  default:
    
    break;
  }
}
