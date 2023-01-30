#include "system.h"
#include "stdio.h"

void SystemInit(void)
{
  /* NOTE :SystemInit(): 
                         User can setups the default system clock (System clock source, PLL Multiplier
                         and Divider factors, AHB/APBx prescalers and Flash settings).
   */
 //  setvbuf(stdout, NULL, _IONBF, 0); // disable buffering stdout, needed so that enters to _write
  setbuf(stdout, NULL);
}
