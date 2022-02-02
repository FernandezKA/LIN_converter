#ifndef _communication_h_
#define _communication_h_
//User includes
#include "lin.h"
enum PartDigit{
  value_incompleted, 
  value_completed
};

//User defines
//For RS232 - LIN parser
extern enum PartDigit ValueReceived;
extern uint8_t resValue;
enum PartDigit GetHex(char digit);
void GetASCII(uint8_t value, uint8_t* s1, uint8_t* s2);
void LinPrint(uint8_t val);
//User functions
void Reflect_LIN(LIN_Header header, LIN_Response response, bool isValid);
#endif