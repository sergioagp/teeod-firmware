#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include <tee_ipc.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
void parse_TEE_Operation(TEE_Operation *op) {
  printf("type addr:                          %p, value: %d\r\n", &(op->type), op->type);
  printf("ctx.session_id addr:	              %p, value: %u\r\n", &(op->ctx.session_id), op->ctx.session_id);
  printf("par.cmd_id addr:                    %p, value: %u\r\n", &(op->par.cmd_id), op->par.cmd_id);
  printf("par.param_types addr:               %p, value: %u\r\n", &(op->par.param_types), op->par.param_types);
  for (int i = 0; i < 4; i++) {
  printf("par.params[%d].memref.buffer addr: %p, value: %p\r\n", i, &(op->par.params[i].memref.buffer), op->par.params[i].memref.buffer);
  printf("par.params[%d].memref.size addr:   %p, value: %u\r\n", i, &(op->par.params[i].memref.size), op->par.params[i].memref.size);
  printf("par.params[%d].value.a addr:       %p, value: %u\r\n", i, &(op->par.params[i].value.a), op->par.params[i].value.a);
  printf("par.params[%d].value.b addr:       %p, value: %u\r\n", i, &(op->par.params[i].value.b), op->par.params[i].value.b);
  }
  printf("ret addr:                          %p, value: %u\r\n", &(op->ret), op->ret);

}

int main(void)
{
  TEE_Operation *op = (TEE_Operation *)0x44A00000;
  printf("waiting for msg...\r\n");
  while(op->type == 0);
  printf("Message received!\r\n");
  parse_TEE_Operation(op);
  //fflush(stdout);
  /* Loop forever */
	for(;;);
}
