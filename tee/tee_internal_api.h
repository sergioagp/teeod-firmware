/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/* Based on GP TEE Internal API Specification Version 0.27 */
#ifndef TEE_INTERNAL_API_H
#define TEE_INTERNAL_API_H

#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <tee_ta_api.h>

#ifdef DEBUG_MODE
#include <stdio.h>
#endif

// Define the DMSG and IMSG macros
#ifdef DEBUG_MODE
#define DMSG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define IMSG(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define EMSG(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)
#else
#define DMSG(fmt, ...)
#define IMSG(fmt, ...)
#define EMSG(fmt, ...)
#endif

#endif
