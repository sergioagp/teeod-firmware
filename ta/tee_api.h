/* Based on GP TEE Internal API Specification Version 1.1 */
#ifndef TEE_API_H
#define TEE_API_H

#include <stddef.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <stdio.h>
#define EMSG printf
#define DMSG printf
//tee_internal_api_extensions.h:
/* Hint implementation defines */
#define TEE_USER_MEM_HINT_NO_FILL_ZERO       0x80000000

/* System API - Memory Management */
void *TEE_Malloc(uint32_t size, uint32_t hint);

void *TEE_Realloc(void *buffer, uint32_t newSize);

void TEE_Free(void *buffer);

void *TEE_MemMove(void *dest, const void *src, uint32_t size);

//void *TEE_MemFill(void *buff, uint32_t x, uint32_t size);

#endif /* TEE_API_H */
