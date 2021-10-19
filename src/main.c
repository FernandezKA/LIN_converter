#include "stm8s_conf.h"
#include "softuart.h"
uint8_t u8RXB;
event mode;
LRtype lrTypeSel;
uint8_t u8TempVar = 0;
uint16_t u16TempCrc16Value = 0;


uint8_t *CRC16_u8Value=NULL;
uint8_t *Tps_u8LedPosDeviceId=NULL;
uint8_t Tps_u8LedMatrixI = 0;

uint16_t u16AdcValue=0;
uint16_t u16AdcValueStore=0;
bool     bRelayTrigger = false;
bool     bButtonControlTrigger = false;
uint8_t u8UartMessage[5]={0xff};
uint8_t u8UartIcounter = 0;
uint8_t u8ParityData = 0;
uartPackage upacMsg;

int SystemInit(void)
{
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);	// set the highest HSI speed
  //Soft UART
  GPIOD->DDR|=(1<<5);
  GPIOD->CR1|=(1<<5);
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();	

  return 0;
}
void main(void)
{
  //SystemInit();
  GPIOD->ODR|=(1<<5);
   GPIOD->CR1|=(1<<5);
  for(;;){
    for(uint16_t i = 0x00; i < 0xFFFF; ++i){
      asm("nop");
    }
    GPIOD->ODR^=(1<<5);
  }

  }

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
