#ifndef _communication_h_
#define _communication_h_
//User includes
#include "lin.h"
enum PartDigit{
  value_incompleted, 
  value_completed
};

//User defines
extern enum PartDigit ValueReceived;
extern uint8_t resValue;
//User variables
enum PartDigit GetDigit(char digit);
//User functions
void Reflect_LIN(LIN_Header header, LIN_Response response);
#endif