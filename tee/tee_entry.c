#include "tee_entry.h"
#include "tee_ipc.h"
#include "tee_ta_api.h"
#include "tee_session.h"

#define UNUSED(x) (void)(x)

void tee_entry(TEE_Operation *operation) {
  TEE_Result ret = TEE_SUCCESS;
  TEE_Session *session;

  // Parse the session structure to determine the operation
  switch (operation->type) {
    case TEE_OPERATION_OPEN_SESSION:
      // Create a new session
      session = create_session();
      operation->ctx.session_id = session->session_id;
      // Call the TA_OpenSessionEntryPoint function
      ret = (TEE_Result) TA_OpenSessionEntryPoint((uint32_t) operation->par.param_types,
                                      (TEE_Param*) operation->par.params,
                                      (void**) &session);
      break;
    case TEE_OPERATION_INVOKE_COMMAND:
      session = get_session(operation->ctx.session_id);
      // Call the TA_InvokeCommandEntryPoint function
      ret = (TEE_Result) TA_InvokeCommandEntryPoint((void *) session,
                                       (uint32_t) operation->par.cmd_id,
                                       (uint32_t) operation->par.param_types,
                                       (TEE_Param*) operation->par.params);
      break;
    case TEE_OPERATION_CLOSE_SESSION:
      session = get_session(operation->ctx.session_id);
      // Call the TA_CloseSessionEntryPoint function
      TA_CloseSessionEntryPoint((void *) session);
      // Delete the session id
      delete_session(operation->ctx.session_id);
      break;
    default:
      ret = TEE_ERROR_BAD_PARAMETERS;
  }
  // Save the return code of the operation
  operation->ret = ret;
}

void __tee_entry() {
  // Get the address of the shared memory
  TEE_Operation *operation = (TEE_Operation *)TEE_IPC_ADRR;
  // Call the tee_entry function
  tee_entry(operation);
  // Return to sleep
}
