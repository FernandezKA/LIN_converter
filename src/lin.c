/* Includes ------------------------------------------------------------------*/

#include "lin.h"
#include "uart.h"
#include "init.h"
//User function declaration

//User variables
uint16_t u16BreakLength;
LIN_HEADER currentHeader;
struct LIN_Header header;
struct LIN_Response response;
enum LIN_Size Lin_size = bytes_2;
uint8_t countReceived = 0x00;
//This function receive PID frame
uint8_t GetPID(uint8_t u8PIDReceive)
{
  uint8_t u8PID = u8PIDReceive & 0xC0;
  //TODO: Add parity check
  //bool P0 =
  //bool P1 =
  return u8PID;
}
//This is IRQ handler for detecting break on the LIN mode
#ifdef EXTI_PORTD_IRQ
//External Interrupt PORTD Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  GPIOB->ODR ^= (1 << 5);
  if ((GPIOD->IDR & (1 << 6)) == (1 << 6))
  { //Rising edge
    uint16_t u16TempTime = 0x0000;
    u16TempTime = (TIM1->CNTRH) << 8;
    u16TempTime |= TIM1->CNTRL;
    if (u16TempTime > 1200 && u16TempTime < 1450)
    {
      currentHeader = wait_synch;
      SetSynchMode();
      UART1->DR = 0x00U;
    }
  }
  else
  { //Falling edge
    TIM1->CNTRH = 0x00;
    TIM1->CNTRL = 0x00;
  }
}
#endif
