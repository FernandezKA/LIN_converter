#include "stm8s_conf.h"
#include "lin.h"
void main(void)
{
  for(;;){
    switch(eStateRecieve){
    case wait_break:
      
      break;
    case wait_sync:
      
      break;
    case wait_id:
      
      break;
    case wait_parity:
      
      break;
    case wait_data:
      
      break;
    case completed:
      
      break;
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
