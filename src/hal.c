#include "hal.h"


/*************************************************
Sub Function: 


**************************************************/
void Mcu_init(void)
{
  disableInterrupts();
  Flash_Config();
  CLK_Config();
  GPIO_Config();
  //GPIO_WriteHigh(GPIOA,GPIO_PIN_3);                          //PWM PIN output high min current
  Timer1_Counter_Config();
  //Time4_Config();
  ADC_Config();
  UART1_Config();
  UART1_Cmd(ENABLE);
  //UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  enableInterrupts();
  //IWDG_Config(); 	
}


void Mcu_AdcRun()
{
  uint8_t Rsel_filter;

  for(Rsel_filter=0;Rsel_filter<16;Rsel_filter++)
  {
    u16AdcValue = u16AdcValue  + get_adc_result(ADC1_CHANNEL_2);
  }
  u16AdcValue  = u16AdcValue >>4;
  
  if(u16AdcValue<ADC_MIN_VALUE)
  {
    u16AdcValue = ADC_MIN_VALUE;
    u16AdcValueStore = u16AdcValue;
    mode.modeAdcDimming = 1;
    bRelayTrigger = true;
  }  
  else if((u16AdcValue>(u16AdcValueStore+50)) || (u16AdcValue<(u16AdcValueStore-50)))
  {
    mode.modeAdcDimming = 1;
    u16AdcValueStore = u16AdcValue;
  }
  else
  {
    mode.modeAdcDimming = 0;
  }
  
}



LRtype Left_Right_selection(void)
{
  if((GPIOA->IDR &0x08) == 0x00)
    return LEFT;
  else
    return RIGHT;
}

void Uart_vidResetUart(void)
{
  GPIO_WriteHigh(GPIOD,GPIO_PIN_5);                             //Pull down RX of TPS
}
