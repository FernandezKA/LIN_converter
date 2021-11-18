#ifndef _lin_h_
#define _lin_h_
//User includes
#include "stm8s_conf.h"
#include "fifo.h"
//User defines

//FSM for frame header
enum LIN_HEADER
{
  wait_break,
  wait_synch,
  wait_pid,
  wait_data
};
enum LIN_Size
{
  bytes_2 = 2,
  bytes_4 = 4,
  bytes_8 = 8
};
struct LIN_Header
{
  uint8_t synch;
  uint8_t pid;
  enum LIN_Size size;
};
struct LIN_Response
{
  uint8_t data[8U];
  uint8_t CRC;
};
extern struct LIN_Header header;
extern struct LIN_Response response;
extern enum LIN_Size Lin_size;
extern uint8_t countReceived;
extern FIFO sw_transmit;
//User function definition
bool BreakDetection(void);
bool GetSynch(uint8_t data);
uint8_t GetPID(uint8_t u8PIDReceive);
void GetNextState(LIN_HEADER *current);
//User variables
extern LIN_HEADER currentHeader;
extern uint16_t u16BreakLength;
#endif