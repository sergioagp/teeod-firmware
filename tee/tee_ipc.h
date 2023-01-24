#ifndef TEE_CLIENT_IPC_H
#define TEE_CLIENT_IPC_H

#include "tee_api_types.h"
#include <stdint.h>

#define TEE_IPC_ADRR  0x40000000

#define TEE_OPERATION_OPEN_SESSION    0
#define TEE_OPERATION_INVOKE_COMMAND  1
#define TEE_OPERATION_CLOSE_SESSION   2

typedef uint32_t TEE_Result;

typedef struct {
  int type;
  union {
    uint32_t session_id;
    TEE_UUID tee_uuid;
  } ctx;
} TEE_Operation_ctx;

typedef struct {
  uint32_t cmd_id;
  uint32_t param_types;
  TEE_Param params[4];
} TEE_Operation_params;

typedef struct {
  int type;
  union {
    uint32_t session_id;
    TEE_UUID tee_uuid;
  } ctx;
  TEE_Operation_params par;
  TEE_Result ret;
} TEE_Operation;

#endif /* TEE_CLIENT_IPC_H */