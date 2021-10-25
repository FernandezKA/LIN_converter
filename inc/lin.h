#ifndef _lin_h_
#define _lin_h_
//User includes
#include <stm8s.h>
//User defines
#define BAUDRATE 9600
//User variable definitions for LIN protocol
extern uint8_t u8CountBreak;
extern uint8_t u8SyncValue;
extern uint8_t u8PID;
extern uint8_t* pu8DataRecieve;
extern uint8_t u8CRC;
//User functions
bool bBreakDetect(void);//This function detect first break for LIN bus
enum {
    wait_break,
    wait_sync,
    wait_id,
    wait_parity, 
    wait_data, 
    completed
} eStateRecieve;
#endif