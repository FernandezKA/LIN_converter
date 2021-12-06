#include "help.h"
#include "softuart.h"

void PrintHelp(void){
  char BAUD_MSG[18] = {'0','x','F','F','-','s','e','l','e','c','t','_','b','a','u','d','\n','\r'};
  char MODE_MSG[22] = {'0','x','1','0','-','s','e','l','e','c','t','_','l','i','n','_','m','o','d','e','\n','\r'};
  char MASTER_MODE[18] = {'0','x','5','5','-','m','a','s','t','e','r','_','m','o','d','e','\n','\r'};
  char SLAVE_MODE[17] = {'0','x','0','0','-','s','l','a','v','e','_','m','o','d','e','\n','\r'};
  print(BAUD_MSG, 18);
  print(MODE_MSG, 22);
  print(MASTER_MODE, 18);
  print(SLAVE_MODE, 17);
}
void print(char* pText, uint8_t size){
  for(uint8_t i = 0; i < size; ++i){
    while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty) {asm("nop");}
      if (test_status(transmit_data_reg_empty) == transmit_data_reg_empty)
      {
        uart_send(pText[i]);
      }
  }
}