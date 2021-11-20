#include "stm8s_conf.h"
#include "stm8s_itc.h"
//User includes
#include "init.h"
#include "lin.h"
#include "uart.h"
#include "fifo.h"
//Includes for SPL library
#include "stm8s_tim2.h"
#include "stm8s_gpio.h"
//Function declaration
static void SysInit(void);
FIFO sw_transmit;
static FIFO sw_receive;
bool SendLIN = false;
enum FSM_REC{
  w_mode, 
  w_pid, 
  w_data
};
static FSM_REC fsm_receive = w_mode;

struct LIN_SEND LIN_Send;

void main(void)
{
  SysInit();
  currentHeader = wait_break;
  sw_transmit.isEmpty = true;
  sw_receive.isEmpty = true;
  static uint8_t CountDataLIN = 0x00;
  asm("rim");
  for (;;)
  {
    if(test_status(receive_buffer_full) == receive_buffer_full){//FIFO buffer for RS232, load data from UART
      uint8_t u8Data;
      uart_read(&u8Data);
      Push(&sw_receive, u8Data);
    }
    if(!sw_receive.isEmpty){//Parse data from RS232
      switch(fsm_receive){
      case w_mode:
        static uint8_t data = Pull(&sw_receive);
        if(data == 0x00){//Slave mode
          LIN_Send.Mode = SLAVE;
          fsm_receive = w_pid;
        }
        else if(data == 0x55){//Master mode
          LIN_Send.Mode = MASTER;
          fsm_receive = w_pid;
        }
        else{//Mistake
          fsm_receive = w_mode;
        }
        break;
        
      case w_pid:
          LIN_Send.PID = Pull(&sw_receive);
          if(LIN_Send.PID < 0x1FU){
            LIN_Send.SIZE = bytes_2;
          }
          else if(LIN_Send.PID < 0x2FU){
            LIN_Send.SIZE = bytes_4;
          }
          else if(LIN_Send.PID < 0x3FU){
            LIN_Send.SIZE = bytes_8;
          }
          fsm_receive = w_data;
        break;
        
      case w_data:
        if(CountDataLIN < LIN_Send.SIZE){
          LIN_Send.Data[CountDataLIN++] = Pull(&sw_receive);
        }
        else{
          
          //Receive CRC and send packet
          CountDataLIN = 0x00U;
          LIN_Send.CRC = Pull(&sw_receive);
          //TODO: Add send packet as slave and master
          if(LIN_Send.Mode == SLAVE){
            SendLIN = true;
          }
          else if(LIN_Send.Mode == MASTER){
            send_response(&LIN_Send, true);
          }
          fsm_receive = w_mode;
        }
        break;
        
      default:
        
        break;
      }
    }
    if (!sw_transmit.isEmpty)//Lin packet recognized, reflect from RS232
    {
      if(test_status(transmit_data_reg_empty) == transmit_data_reg_empty)
      {
        uart_send(Pull(&sw_transmit));
      }
    }
    asm("nop");
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif
//This function combined all of init function
static void SysInit(void)
{
  Clk_Config();
  UART_SW_Config();
  UART_HW_Config();
  Tim1_Config();
  GPIO_Config();
  SetExtIRQ();
  asm("rim");
}
