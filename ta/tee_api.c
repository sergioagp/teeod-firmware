// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */
#include <stdlib.h>
#include <string.h>
#include <tee_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <tee_api_types.h>

#include <aes.h>

TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation,
uint32_t algorithm,
uint32_t mode,
uint32_t maxKeySize)
{
  if (algorithm != TEE_ALG_AES_ECB_NOPAD &&
      algorithm != TEE_ALG_AES_CBC_NOPAD &&
      algorithm != TEE_ALG_AES_CTR &&
      algorithm != TEE_ALG_AES_XTS &&
      algorithm != TEE_ALG_AES_GCM) {
      return TEE_ERROR_NOT_SUPPORTED;
  }
  AES_ctx *ctx = (AES_ctx *) TEE_Malloc(sizeof(AES_ctx), TEE_MALLOC_FILL_ZERO);
  if (ctx == NULL) {
    return TEE_ERROR_OUT_OF_MEMORY;
  }

  // Store the algorithm and mode in the AES_ctx struct
  ctx->mode = mode;
  ctx->algorithm = algorithm;
  *operation = (TEE_OperationHandle) ctx;
 
  return TEE_SUCCESS;
}

#define AES_KEY_EXP_SIZE AES_keyExpSize

TEE_Result TEE_AllocateTransientObject(uint32_t objectType, uint32_t maxObjectSize, TEE_ObjectHandle *object) {
    if (objectType != TEE_TYPE_AES) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    (void) maxObjectSize;
    // if (maxObjectSize < AES_KEY_EXP_SIZE) {
    //     return TEE_ERROR_BAD_PARAMETERS;
    // }

    uint8_t* hkey = (uint8_t*)malloc(AES_KEY_EXP_SIZE);
    if (hkey == NULL) {
        return TEE_ERROR_OUT_OF_MEMORY;
    }

    memset(hkey, 0, AES_KEY_EXP_SIZE);
    *object = (TEE_ObjectHandle)hkey;

    return TEE_SUCCESS;
}


TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object, const void *data, size_t dataLen)
{
    TEE_Result result = TEE_SUCCESS;
    
    if (!object || !data)
    {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    
    // if (dataLen > object->maxObjectSize)
    // {
    //     return TEE_ERROR_BAD_PARAMETERS;
    // }
    
    memcpy(object, data, dataLen);
    //object->objectSize = dataLen;
    
    return result;
}

TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation, TEE_ObjectHandle key) {
  if (!operation || !key) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  // implementation specific logic to set the key for the operation
  operation->key = key;
  return TEE_SUCCESS;
}

void TEE_FreeTransientObject(TEE_ObjectHandle object)
{
    if (object != TEE_HANDLE_NULL)
    {
        free(object);
        object = TEE_HANDLE_NULL;
    }
}

void TEE_FreeOperation(TEE_OperationHandle operation) {
  // Check if the operation handle is valid
  if (!operation) {
    return;
  }

  // Deallocate memory for the operation
  free(operation);
}

void *TEE_Malloc(uint32_t len, uint32_t hint)
{
	if (hint == TEE_MALLOC_FILL_ZERO)
		return calloc(1, len);
	else if (hint == TEE_USER_MEM_HINT_NO_FILL_ZERO)
		return malloc(len);

	//EMSG("Invalid hint %#" PRIx32, hint);

	return NULL;
}

void *TEE_Realloc(void *buffer, uint32_t newSize)
{
	return realloc(buffer, newSize);
}

void TEE_Free(void *buffer)
{
	free(buffer);
}

void *TEE_MemMove(void *dest, const void *src, uint32_t size)
{
	return memmove(dest, src, size);
}
