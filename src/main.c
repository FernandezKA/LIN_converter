#include "init.h"

//Function declaration
static void SysInit(void);
static void BAUD_Restore(uint16_t* BAUD_VAR, uint32_t address);
static uint16_t BAUD_TEST;
//User variables
uint16_t BAUD_LIN;
FIFO sw_transmit;
static FIFO sw_receive;
bool SendLIN = false;
static uint8_t P1;
static uint8_t P0;
static uint8_t parity;
uint32_t BAUD_ADDR = 0x00004000;//This add for BAUD_LIN value
enum FSM_REC
{
  w_mode,
  w_pid,
  w_data
};
static FSM_REC fsm_receive = w_mode;
struct LIN_SEND LIN_Send;

//Main function
void main(void)
{
  SysInit();
  BAUD_Restore(&BAUD_LIN, BAUD_ADDR);
  currentHeader = wait_break;
  sw_transmit.isEmpty = true;
  sw_receive.isEmpty = true;
  static uint8_t CountDataLIN = 0x00;
  asm("rim");
  for (;;)
  {
    if (test_status(receive_buffer_full) == receive_buffer_full)
    { //FIFO buffer for RS232, load data from UART
      uint8_t u8Data;
      uart_read(&u8Data);
      Push(&sw_receive, u8Data);
    }
    /***********************************************************/
    if (!sw_receive.isEmpty)
    { //Parse data from RS232 to LIN
      switch (fsm_receive)
      {
      case w_mode:
        static uint8_t data = 0xFF;
        data = Pull(&sw_receive);
        if (data == 0x00)
        { //Slave mode
          LIN_Send.Mode = SLAVE;
          fsm_receive = w_pid;
        }
        else if (data == 0x55)
        { //Master mode
          LIN_Send.Mode = MASTER;
          fsm_receive = w_pid;
          
        }
        
        else if (data == 0xFF){
          if(BAUD_LIN == 9600){
            BAUD_LIN = 19200;
          }
          else{
            BAUD_LIN = 9600;
          }
          UpdateBAUD_EEPROM(BAUD_LIN, BAUD_ADDR);
          SysInit();
        }
        else if(data == 0x10){
          if(LIN_ver == LIN_2_1){
            LIN_ver = LIN_1_3;
          }
          else{
            LIN_ver = LIN_2_1;
          }
        }
        else
        { //Mistake
          fsm_receive = w_mode;
          ResetState();
        }
        break;

      case w_pid:
        LIN_Send.PID = GetPID(Pull(&sw_receive));
        //LIN_Send.PID = Pull(&sw_receive)& 0x3F;
        P0 = ((LIN_Send.PID & (1<<0)) ^ (LIN_Send.PID & (1<<1) >> 1) ^ (LIN_Send.PID & (1<<2) >> 2) ^ (LIN_Send.PID & (1<<4) >> 4)) << 7;
        P1 = (!(((LIN_Send.PID & 0x02) >> 1) ^ (LIN_Send.PID & (1<<3) >> 3) ^ (LIN_Send.PID & (1<<4) >> 4) ^ (LIN_Send.PID & (1<<5) >> 5))<<6);
        parity = P0 | P1;
        //Calculate CRC
        LIN_Send.CRC = 0xFF;
        if(LIN_ver == LIN_1_3){
          asm("nop");
        }
        else if(LIN_ver == LIN_2_1){
          CRC8(&LIN_Send.CRC, LIN_Send.PID, false);  
        }
        //Define size of packet
        if (LIN_Send.PID < 0x1FU)
        {
          LIN_Send.SIZE = bytes_2;
        }
        else if (LIN_Send.PID < 0x2FU)
        {
          LIN_Send.SIZE = bytes_4;
        }
        else if (LIN_Send.PID < 0x3FU)
        {
          LIN_Send.SIZE = bytes_8;
        }
        else
        {
          fsm_receive = w_mode;
          ResetState();
          break;
        }
        LIN_Send.PID |= parity;
        fsm_receive = w_data;
        break;

      case w_data:
        if (CountDataLIN < LIN_Send.SIZE - 1)
        {
          uint8_t u8DataReaded = Pull(&sw_receive);
          CRC8(&LIN_Send.CRC, u8DataReaded, false); 
          //LIN_Send.CRC^= u8DataReaded;
          LIN_Send.Data[CountDataLIN++] = u8DataReaded;
         
        }
        else if (CountDataLIN == LIN_Send.SIZE - 1) //It's CRC field
        {
          //Receive CRC and send packet
          LIN_Send.Data[CountDataLIN] = Pull(&sw_receive);
          CRC8(&LIN_Send.CRC, LIN_Send.Data[CountDataLIN], true); 
          //LIN_Send.CRC^= LIN_Send.Data[CountDataLIN];
          CountDataLIN = 0x00U;
          //uint8_t u8CRCReceived = Pull(&sw_receive);
          fsm_receive = w_mode;
          if (LIN_Send.Mode == SLAVE)
          {
            SendLIN = true;
            while (SendLIN)
            {
              asm("nop");
            } //Wait while not handled request
            ResetState();
          }
          else if (LIN_Send.Mode == MASTER)
          {
            send_response(&LIN_Send, true);
            ResetState();
          }
          while (!sw_receive.isEmpty)
          { //Clear all of data, because it's mistake
            //Pull(&sw_receive);
            ResetState();
          }
        }
        break;
      }
    }
    /***********************************************************/
    if (!sw_transmit.isEmpty) //Lin packet recognized, reflect from RS232
    {
      if (test_status(transmit_data_reg_empty) == transmit_data_reg_empty)
      {
        uart_send(Pull(&sw_transmit));
      }
    }
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
  Tim4_Config();
  GPIO_Config();
  SetExtIRQ();
  asm("rim");
}
//This function reset configuration
void ResetState(void){
  //Clear ring buffer
  while(!sw_receive.isEmpty){
    Pull(&sw_receive);
  }
  fsm_receive = w_mode;
  LIN_Send.CRC = 0xFF;
  LIN_Send.PID = 0x00;
  LIN_Send.SIZE = bytes_2;
  LIN_Send.Mode = UNDEF;
}
//This function read BAUD value
static void BAUD_Restore(uint16_t* BAUD_VAR, uint32_t address){
  *BAUD_VAR = (FLASH_ReadByte(address)<<8);//Read MSB
  *BAUD_VAR |= FLASH_ReadByte(address + 1); //Read LSB
  BAUD_LIN = *BAUD_VAR;
  BAUD_TEST = *BAUD_VAR;
  if(*BAUD_VAR == 9600){
    asm("nop");
  }
  else if(*BAUD_VAR == 19200){
    asm("nop");
  }
  else{
    *BAUD_VAR = 19200;
    UpdateBAUD_EEPROM(BAUD_LIN, address);
  }
}
