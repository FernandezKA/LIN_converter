#include "stm8s_conf.h"
#include "lin.h"
void main(void)
{
  for(;;){
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
