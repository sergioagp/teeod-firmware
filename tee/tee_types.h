#ifndef TEE_TYPES_H
#define TEE_TYPES_H

#include "tee_api_types.h"
#include <stdint.h>

#define TEE_OPERATION_OPEN_SESSION    0
#define TEE_OPERATION_INVOKE_COMMAND  1
#define TEE_OPERATION_CLOSE_SESSION	2

typedef struct {
    uint32_t session_id;
    // Other fields to keep track of the context
} TA_Session_ctx;

typedef union {
    struct {
        uint32_t param_types;
        TEE_Param params[4];
    } open_session;
    struct {
        uint32_t cmd_id;
        uint32_t param_types;
        TEE_Param params[4];
    } invoke_command;
} TA_Session_params;

typedef struct {
    TA_Session_params params;
    void *sess_ctx;
    int operation;
    TEE_Result return_code;
} TA_Session;

#endif /* TEE_TYPES_H */