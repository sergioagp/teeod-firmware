#include "tee_entry.h"
#include "tee_types.h"
#include "tee_ta_api.h"

#define SHARED_MEMORY_ADDRESS 0x40000000

#define UNUSED(x) (void)(x)

void tee_entry(TA_Session *session) {
  TEE_Result ret = TEE_SUCCESS;
  // Parse the session structure to determine the operation
  switch (session->operation) {
    case TEE_OPERATION_OPEN_SESSION:
      // Call the TA_OpenSessionEntryPoint function
      ret = (TEE_Result) TA_OpenSessionEntryPoint((uint32_t) session->params.open_session.param_types,
                                      (TEE_Param*) session->params.open_session.params,
                                      (void**) &session->sess_ctx);
      break;
    case TEE_OPERATION_INVOKE_COMMAND:
      // Call the TA_InvokeCommandEntryPoint function
      ret = (TEE_Result) TA_InvokeCommandEntryPoint((void *) session->sess_ctx,
                                       (uint32_t) session->params.invoke_command.cmd_id,
                                       (uint32_t) session->params.invoke_command.param_types,
                                       (TEE_Param*) session->params.invoke_command.params);
      break;
    case TEE_OPERATION_CLOSE_SESSION:
      // Call the TA_CloseSessionEntryPoint function
      TA_CloseSessionEntryPoint((void *) session->sess_ctx);
      break;
    default:
      ret = TEE_ERROR_BAD_PARAMETERS;
  }
  // Save the return code of the operation
  session->return_code = ret;
}

void __tee_entry() {
  // Get the address of the shared memory
  TA_Session *session = (TA_Session *)SHARED_MEMORY_ADDRESS;
  // Call the tee_entry function
  tee_entry(session);
  // Return to sleep
}
