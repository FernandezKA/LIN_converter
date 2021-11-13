#ifndef _lin_h_
#define _lin_h_
//User includes
#include "stm8s_conf.h"
//User defines
//FSM for frame header
enum LIN_HEADER{
  wait_break, 
  wait_synch, 
  wait_pid
};
//User function definition
bool BreakDetection(void);
bool GetSynch(uint8_t data);
uint8_t GetPID(uint8_t u8PIDReceive);
void GetNextState(LIN_HEADER* current);
//User variables
extern LIN_HEADER currentHeader;
extern uint16_t u16BreakLength;
#endif