#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_uart1.h"
#include "stddef.h"
#include "stdbool.h"

//#define PC3                           //PC3 as Pull-down COM PIN
#define PD2                             //PD2 as Pull-down COM PIN

#define PWM_FREQ                ((u16)(1024-1))
#define CCR3_Val                ((u16)(PWM_FREQ+1)>>3)        // 12.5% duty cycle
#define TIMER1_CNT              ((u16)0x3e80)
#define TIMER1_PRD	        ((u16)0)
#define DELAY_TIMER1_CNT        ((u16)0x3e80)
#define DELAY_TIMER1_PRD	((u16)0)
#define TIMER2_ARR              1600                          //((u16)0x640)
#define TIMER2_CRR3             ((u16)0x320)
#define TIME_TO_STEADY          1

#define TIMER4_CNT_1	        ((u8)198)
#define WDG_RLOAD               IWDG->KR = (uint8_t)0xAA 

#define ADC_MAX_VOL	        1000		
#define ADC_MIN_VOL	        30

#define CRC16_PRESET 0X0000
#define CRC16_POLYNOM 0XA001
#define CRC16_BITS 8


#define ADC_MIN_VALUE                   400





void delay_nms(uint16_t n);
void CLK_Config(void);
void GPIO_Config(void);
void Mcu_vidPortIntEnable(void);
void Mcu_vidPortIntDisable(void);
void UART1_Config(void);
void Timer1_Counter_Config(void);
void Timer1_PWM_Config(void);
void Timer2_Config(void);
void Flash_Config(void);
void Time4_Config(void);
void ADC_Config(void);
void IWDG_Config(void);
uint16_t get_adc_result(uint8_t adc_channel_no);
uint16_t TMR1_ReadTimer(void);
uint8_t Read_Flash_Byte(uint32_t add);
void Write_Flash_Byte(uint32_t add, uint8_t value);
void Mcu_init(void);
void Uart_vidResetUart(void);
uint16_t Crc16_u16CalCrc16(uint8_t *u8Packet,uint8_t u8Number);
uint8_t *Lin_u8CalCrc16(uint8_t *u8Header,uint8_t *u8Packet,uint8_t u8NumberHeader,uint8_t u8NumberPacket);
void Mcu_AdcRun();


//typedef struct event event;
typedef struct event{
  uint8_t modePd3:1;    //Stream mode 0-31
  uint8_t modePd2:1;    //ADB 1(refer to Pixel22 IHM) mode
  uint8_t modePd4:1;
  uint8_t modePc7:1;
  uint8_t modePc6:1;
  uint8_t modePc5:1;
  uint8_t modePajPsMode:1;
  uint8_t modeAdcDimming:1;
}event;

typedef struct uartPackage{
  bool msgUpdate;
  uint8_t msgCmd;
  uint16_t msgData; 
}uartPackage;

typedef enum LRtype{
 LEFT = 0,
 RIGHT = 1
}LRtype;

void Lin_RunMode();
uint8_t Lin_parity(uint8_t data);
LRtype Left_Right_selection(void);

extern LRtype lrTypeSel;
extern event mode;
extern uint8_t *CRC16_u8Value;

extern uint16_t u16AdcValue;
extern uint16_t u16AdcValueStore;
extern bool     bRelayTrigger;
extern bool     bButtonControlTrigger;
extern uint8_t u8UartMessage[5];
extern uint8_t u8UartIcounter;
extern uint8_t u8ParityData;
extern uartPackage upacMsg;

//extern uint16_t TPS_u8WidthData[32];

