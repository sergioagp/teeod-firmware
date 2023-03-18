#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include <tee_ipc.h>
#include <tee_entry.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{ 
  //printf("\033[2J");  // sends the ASCII escape sequence to clear the console screen
  /* Loop forever */
  //TODO: change from pulling program to interrupt service
  volatile TEE_Operation *op = (TEE_Operation *)TEE_IPC_ADRR;
  /* Loop forever */
	for(;;) {
    while(op->type == 0);
    __tee_entry();
    op->type = 0;
    //fflush(stdout);
  }
  return 0;
}
