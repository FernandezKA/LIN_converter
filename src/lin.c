/* Includes ------------------------------------------------------------------*/

#include "lin.h"
#include "uart.h"
#include "init.h"
//User function declaration

//User variables
uint16_t u16BreakLength;
LIN_HEADER currentHeader;
//This function detect break from measured pulse structure
bool BreakDetection(void){
  return false;
}
//This function recognize sync field into the frame
bool GetSynch(uint8_t data){
  return false;
}
//This function receive PID frame
uint8_t GetPID(void){
  return 0xFF;
}
//This function switch FSM states
void GetNextState(LIN_HEADER* current){
  switch(*current){
  case wait_break:
    //At falling edge IRQ -> start cnt -> at rising edge write result, check for valid
    //If valid - switch FSM state, else set zero value into the counter
    //PD6 - UART1_RX

    break;
  case wait_synch:
    //Receive data, if it is equal 0x55 -> switch FSM state, else set state at wait_break
    break;
  case wait_pid:
    //set other frame type, wait IRQ
    break;
  default:
    //Set first FSM state
    break;
  }
}

#ifdef EXTI_PORTD_IRQ 
//External Interrupt PORTD Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  GPIOC->ODR^=(1<<7);
  if((GPIOD->IDR & (1<<6)) == (1<<6)){//Rising edge
    uint16_t u16TempTime = 0x0000;
    u16TempTime = (TIM1->CNTRH)<<8;
    u16TempTime |= TIM1->CNTRL;
    if(u16TempTime > 1200 && u16TempTime < 1450){
      currentHeader = wait_synch;
      SetSynchMode();
    }
  }
  else{//Falling edge
    TIM1->CNTRH = 0x00;
    TIM1->CNTRL = 0x00;
  }
}
#endif
