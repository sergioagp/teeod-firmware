#ifndef __TEE_ARGS_H__
#define __TEE_ARGS_H__
#include "tee_defines.h"

struct mb_args
{
    uint32_t cmd;
    uint32_t func;
    uint32_t sess;
    uint32_t cl_id;
    uint32_t pad;
    uint32_t ret;
    uint32_t ret_origin;
    uint32_t num_params;
    uint32_t param0[PARAM_SIZE];
    uint32_t param1[PARAM_SIZE];
    uint32_t param2[PARAM_SIZE];
    uint32_t param3[PARAM_SIZE];
    uint32_t param4[PARAM_SIZE];
    uint32_t param5[PARAM_SIZE];
    uint32_t rsrv0;
    uint32_t rsrv1;
    uint32_t rsrv2;
    uint32_t rsrv3;
    uint32_t rsrv4;
    uint32_t rsrv5;
    uint32_t intclr;
    uint32_t done_a;
};



#endif  /*__TEE_ARGS_H__*/