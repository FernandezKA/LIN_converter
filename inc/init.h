#ifndef _init_h_
#define _init_h_
//User includes
#include "stm8s_conf.h"
//User defines
#define F_CPU 16000000UL
#define BAUD_SW 19200
#define BAUD_HW 9600
#define LED_PORT GPIOB
#define LED_PIN  (1<<5)
//User function definitions
void Clk_Config(void);
void UART_SW_Config(void);
void UART_HW_Config(void);
void Tim1_Config(void);
void Tim4_Config(void);
//User variables
#endif