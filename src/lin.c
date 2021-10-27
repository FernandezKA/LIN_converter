/**
  ******************************************************************************
  * @file           : lin.c
  * @brief          : LIN driver
  * @author         : MicroTechnics (microtechnics.ru)
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/

#include "lin.h"
#include "uart.h"

#define BAUDRATE 9600UL
/* Declarations and definitions ----------------------------------------------*/

static LIN_State curState = LIN_IDLE;
static LIN_Mode curMode = LIN_MASTER;

static uint32_t curBaudrate = 0;

static uint16_t breakCnt = 0;
static uint16_t breakCntLimit = 0;
static uint16_t breakCntUpperLimit = 0;
static uint16_t breakCntLowerLimit = 0;

static uint16_t rxTimeoutCntLimit = LIN_RX_TIMEOUT_US / LIN_TIMER_PERIOD_US;
static uint16_t rxTimeoutCnt = 0;

static uint8_t txDataNum = 0;
static uint8_t rxDataNum = 0;

static uint8_t txId = 0;
static uint8_t txCheckSum = 0;
static uint8_t rxByte = 0;
static uint8_t syncByte = LIN_SYNC_BYTE;

static uint8_t isInit = 0;
static uint8_t *dataPtr;

//Pointer to the IRQ handlers
void (*uartHandle) (void);
void (*TimHandle) (void);
//static UART_HandleTypeDef *uartHandle;
//static TIM_HandleTypeDef *timerHandle;

static GPIO_TypeDef *rxPort;
static GPIO_TypeDef *txPort;


static uint8_t rxPin;
static uint8_t txPin;



/* Functions -----------------------------------------------------------------*/
__weak void LIN_SlaveIdRxCallback(uint8_t id)
{
}



/*----------------------------------------------------------------------------*/
__weak void LIN_RxCpltCallback()
{
}



/*----------------------------------------------------------------------------*/
uint8_t LIN_ReadRxPortState()
{
  uint8_t res = (rxPort -> ODR)&rxPin;//GPIO_ReadInputPin(rxPort, rxPin);
  return res;
}



/*----------------------------------------------------------------------------*/
void LIN_WriteTxPortState(uint8_t state)
{  
  //GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  if (state == 0)
  {
    //Enable GPIO, reset state, disable UART
    UART1->CR2&=~UART1_CR2_TEN | UART1_CR2_REN;
    //Pp, fast mode
    txPort->DDR|=(1<<txPin);
    txPort->CR1|=(1<txPin);
    txPort->CR2|=(1<<txPin);
    /*HAL_GPIO_DeInit(txPort, txPin);
    GPIO_InitStruct.Pin = txPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(txPort, &GPIO_InitStruct);
    HAL_GPIO_WritePin(txPort, txPin, GPIO_PIN_RESET);*/
    
  }
  else
  {
    //Enable UART
    /*HAL_GPIO_WritePin(txPort, txPin, GPIO_PIN_SET);
    HAL_GPIO_DeInit(txPort, txPin);
    GPIO_InitStruct.Pin = txPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(txPort, &GPIO_InitStruct);*/
  }
}



/*----------------------------------------------------------------------------*/
void LIN_Init(void (*uHandle) (void), void (*tHandle) (void), LIN_Mode mode,
              GPIO_TypeDef *rPort, uint32_t rPin, GPIO_TypeDef *tPort, uint32_t tPin)
{  
  uartHandle = uHandle;//Pointer to the function
  TimHandle = tHandle;
  rxPort = rPort;
  rxPin = rPin;
  txPort = tPort;
  txPin = tPin;
  
  curMode = mode;
  curState = LIN_IDLE;
  curBaudrate = BAUDRATE;//uartHandle->Init.BaudRate;
  
  //HAL_TIM_Base_Start_IT(timerHandle);
  //TODO: Enable Tim4 IRQ
  
  breakCntLimit = (1000000 * LIN_BREAK_SIZE_BITS / curBaudrate) / LIN_TIMER_PERIOD_US;
  breakCntUpperLimit = breakCntLimit + breakCntLimit * LIN_BREAK_DEVIATION_PERCENT / 100;
  breakCntLowerLimit = breakCntLimit - breakCntLimit * LIN_BREAK_DEVIATION_PERCENT / 100;
    
  isInit = 1;
}



/*----------------------------------------------------------------------------*/
LIN_Error LIN_Transmit(uint32_t id, uint8_t *ptr, uint8_t len)
{
  LIN_Error res = LIN_OK;
    
  if (isInit == 1)
  {
    if (((curMode == LIN_MASTER) && (curState == LIN_IDLE)) ||
        ((curMode == LIN_SLAVE) && (curState == LIN_RECEIVING_ID)))
    {
      rxDataNum = 0;
      txDataNum = len;
      txId = id;
      dataPtr = ptr;
      
      if (curMode == LIN_MASTER)
      {
        curState = LIN_SENDING_BREAK;
      }
      else
      {
        curState = LIN_SENDING_DATA;
        UART_Transmit_IT(dataPtr, txDataNum);
      }
    }      
    else
    {
      res = LIN_BUSY;
    }
  }
  else
  {
    res = LIN_NOT_INIT;
  }
  
  return res;
}



/*----------------------------------------------------------------------------*/
LIN_Error LIN_Receive(uint32_t id, uint8_t *ptr, uint8_t len)
{
  LIN_Error res = LIN_OK;
    
  if (isInit == 1)
  {
    if (((curMode == LIN_MASTER) && (curState == LIN_IDLE)) ||
        ((curMode == LIN_SLAVE) && (curState == LIN_RECEIVING_ID)))
    {
      rxDataNum = len;
      txDataNum = 0;
      txId = id;
      dataPtr = ptr;
      
      if (curMode == LIN_MASTER)
      {
        curState = LIN_SENDING_BREAK;
      }
      else
      {
        curState = LIN_RECEIVING_DATA;
        UART_Receive_IT(dataPtr, rxDataNum);
      }
    }
    else
    {
      res = LIN_BUSY;
    }
  }
  else
  {
    res = LIN_NOT_INIT;
  }
  
  return res;
}



/*----------------------------------------------------------------------------*/
void LIN_Reset()
{
  curState = LIN_IDLE;
}



/*----------------------------------------------------------------------------*/
LIN_State LIN_GetState()
{
  return curState;
}



/*----------------------------------------------------------------------------*/
uint8_t LIN_CalcCheckSum(uint8_t *data, uint8_t len)
{
  uint16_t sum = 0;
  
  for (uint8_t i = 0; i < len; i++)
  {
    sum += data[i];
  }
  
  while(sum > 0xFF)
  {
    sum -= 0xFF;
  }
  
  sum = 0xFF - sum;
  
  return (uint8_t)sum;
}



/*----------------------------------------------------------------------------*/
uint8_t LIN_GetDataLength(uint8_t id)
{
  uint8_t len = 0;
  
  if (id <= 0x1F)
  {
    len = 2;
  }
  else
  {
    if (id <= 0x2F)
    {
      len = 4;
    }
    else
    {
      if (id <= 0x3F)
      {
        len = 8;
      }
    }
  }
  
  return len;
}



/*----------------------------------------------------------------------------*/
void LIN_TimerProcess()
{      
  uint8_t rxPortState = 0;
  
  switch (curState)
  {
    case LIN_IDLE:
      if (curMode == LIN_SLAVE)
      {
        curState = LIN_RECEIVING_BREAK;
      }
      break;

    case LIN_SENDING_BREAK:
      if (breakCnt == 0)
      {
        LIN_WriteTxPortState(0);
        breakCnt++;
      }
      else
      {          
        if (breakCnt == breakCntLimit)
        {
          breakCnt = 0;
          LIN_WriteTxPortState(1);
          curState = LIN_SENDING_SYNC;
          UART_Transmit_IT(&syncByte, 1);
        }
        else
        {          
          breakCnt++;
        }
      }
      break;
      
    case LIN_RECEIVING_BREAK:
      rxPortState = LIN_ReadRxPortState();
    
      if (rxPortState == 0)
      {
        breakCnt++;
      }
      else
      {
        if (breakCnt != 0)
        {
          if ((breakCnt <= breakCntUpperLimit) && (breakCnt >= breakCntLowerLimit))
          {
            curState = LIN_RECEIVING_SYNC;
            //uint16_t temp = uartHandle->Instance->DR;
            uint16_t temp = UART1->DR;
            UART_Receive_IT(&rxByte, 1);
          }
          
          breakCnt = 0;
        }
      }
      break;
      
    case LIN_RECEIVING_ID:
    case LIN_RECEIVING_DATA:
    case LIN_RECEIVING_SYNC:
      rxTimeoutCnt++;
      
      if (rxTimeoutCnt >= rxTimeoutCntLimit)
      {
        UART_AbortReceive();
        curState = LIN_IDLE;
      }
      
      break;
      
    case LIN_SENDING_DATA:
    default:
      break;
  }
}



/*----------------------------------------------------------------------------*/
void LIN_UartProcess()
{
  rxTimeoutCnt = 0;
  uint8_t checkSum = 0;
  
  switch (curState)
  {
    case LIN_SENDING_SYNC:
      curState = LIN_SENDING_ID;
      UART_Transmit_IT(&txId, 1);
      break;
      
    case LIN_SENDING_ID:
      if (txDataNum > 0)
      {
        curState = LIN_SENDING_DATA;
        txCheckSum = LIN_CalcCheckSum(dataPtr, txDataNum);
        UART_Transmit_IT(dataPtr, txDataNum);
      }
      else
      {
        if (rxDataNum > 0)
        {
          curState = LIN_RECEIVING_DATA;
          UART_Receive_IT(dataPtr, rxDataNum);
        }
        else
        {
          curState = LIN_IDLE;
        }
      }
      break;
    
    case LIN_RECEIVING_SYNC:      
      if (rxByte == LIN_SYNC_BYTE)
      {
        curState = LIN_RECEIVING_ID;
        UART_Receive_IT(&rxByte, 1);
      }
      else
      {
        curState = LIN_IDLE;
      }
      break;
      
    case LIN_RECEIVING_ID:
      LIN_SlaveIdRxCallback(rxByte);
      break;
      
    case LIN_RECEIVING_DATA:
      curState = LIN_RECEIVING_CHECKSUM;
      UART_Receive_IT(&rxByte, 1);
      break;
      
    case LIN_RECEIVING_CHECKSUM:
      checkSum = LIN_CalcCheckSum(dataPtr, rxDataNum);      
      curState = LIN_IDLE;
      
      if (rxByte == checkSum)
      {
        LIN_RxCpltCallback();
      }
      break;
      
    case LIN_SENDING_DATA:
      curState = LIN_SENDING_CHECKSUM;
      txCheckSum = LIN_CalcCheckSum(dataPtr, txDataNum);
      UART_Transmit_IT(&txCheckSum, 1);
      break;
      
    case LIN_SENDING_CHECKSUM:
      curState = LIN_IDLE;
      break;
      
    case LIN_IDLE:
    case LIN_RECEIVING_BREAK:
    default:
      break;
  }
}



/*----------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(void)
{
 /* if (htim->Instance == timerHandle->Instance)
  {
    LIN_TimerProcess();
  }
*/
  LIN_TimerProcess();
}



/*----------------------------------------------------------------------------*/
void HAL_UART_TxCpltCallback(void)
{
  /*if (huart->Instance == uartHandle->Instance)
  {
    LIN_UartProcess();
  }*/
  LIN_UartProcess();
}



/*----------------------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(void)
{
  //if (huart->Instance == uartHandle->Instance)
  //{
    LIN_UartProcess();
  //}
}
