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
    //At falling edge IRQ -> start cnt -> at rising edge write result, check for valid
    //If valid - switch FSM state, else set zero value into the counter
    //PD6 - UART1_RX
    GPIOD->DDR&=~(1<<5);
    GPIOD->CR1|=(1<<5);//Input with pull-up
    GPIOD->CR2|=(1<<5);//Enable external interrupt
    
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
  if((GPIOD->IDR & (1<<4)) == (1<<4)){
    asm("nop");
  }
  else{
    asm("nop");
  }
}
#endif
