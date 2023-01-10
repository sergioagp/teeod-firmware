
#include <stdint.h>
#include <stdio.h>
#include "tee_entry.h"


#include "tee_api_defines.h"
#include "tee_api_types.h"
#include "tee_ta_api.h"


#define UNUSED(x) (void)(x)

/*
 * If tee_entry_std() is overridden, it's still supposed to call this
 * function.
 */
uint32_t __tee_entry()
{
	uint32_t rv = 0xBEEF;

	printf("_tee_entry\n\r");

	struct mb_args *arg = (struct mb_args*) TEE_MSG_ADDR;

	/* Enable foreign interrupts for STD calls */

	switch (arg->cmd) {
	case TEE_MSG_CMD_OPEN_SESSION:
		entry_open_session(arg, arg->num_params);
		break;
	case TEE_MSG_CMD_CLOSE_SESSION:
		entry_close_session(arg, arg->num_params);
		break;
	case TEE_MSG_CMD_INVOKE_COMMAND:
		entry_invoke_command(arg, arg->num_params);
		break;
	// case TEE_MSG_CMD_CANCEL:
	// 	entry_cancel(arg, num_params);
	// 	break;
	default:
		break;
		//EMSG("Unknown cmd 0x%x", arg->cmd);
	}

	return rv;
}

void entry_open_session(struct mb_args * arg, size_t num_params)
{
	UNUSED(arg);
	UNUSED(num_params);
	TEE_Param params[TEE_NUM_PARAMS];
	params[0].value.a=(uint32_t) arg->param2[1];
	params[0].value.b=(uint32_t) arg->param2[2];
	params[1].value.a=(uint32_t) arg->param3[1];
	params[1].value.b=(uint32_t) arg->param3[2];
	params[2].value.a=(uint32_t) arg->param4[1];
	params[2].value.b=(uint32_t) arg->param4[2];
	params[3].value.a=(uint32_t) arg->param5[1];
	params[3].value.b=(uint32_t) arg->param5[2];

	printf("entry_open_session\n\r");
	TA_OpenSessionEntryPoint((uint32_t) TEE_PARAM_TYPES(arg->param2[0],arg->param3[0],arg->param4[0],arg->param5[0]),
				(TEE_Param *) params,
				(void**) &arg->sess);
	
}

void entry_close_session(struct mb_args * arg, size_t num_params)
{
	UNUSED(arg);
	UNUSED(num_params);
	printf("entry_close_session\n\r");
	TA_CloseSessionEntryPoint((void *)&arg->sess);
}

void entry_invoke_command(struct mb_args * arg, size_t num_params)
{
	UNUSED(arg);
	UNUSED(num_params);
	TEE_Param params[TEE_NUM_PARAMS];
	params[0].value.a=(uint32_t) arg->param0[1];
	params[0].value.b=(uint32_t) arg->param0[2];
	params[1].value.a=(uint32_t) arg->param1[1];
	params[1].value.b=(uint32_t) arg->param1[2];
	params[2].value.a=(uint32_t) arg->param2[1];
	params[2].value.b=(uint32_t) arg->param2[2];
	params[3].value.a=(uint32_t) arg->param3[1];
	params[3].value.b=(uint32_t) arg->param3[2];
	printf("entry_invoke_command\n\r");
	TA_InvokeCommandEntryPoint((void *)&arg->sess,
			(uint32_t)arg->cl_id,
			(uint32_t) TEE_PARAM_TYPES(arg->param0[0],arg->param1[0],arg->param2[0],arg->param3[0]),
			(TEE_Param*) params);
}