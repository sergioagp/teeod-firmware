/* Based on GP TEE Internal API Specification Version 1.1 */
#ifndef TEE_API_H
#define TEE_API_H

#include <stddef.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <stdio.h>
#define IMSG printf
#define DMSG printf
#define EMSG printf
//tee_internal_api_extensions.h:
/* Hint implementation defines */
#define TEE_USER_MEM_HINT_NO_FILL_ZERO       0x80000000

/* Data and Key Storage API  - Persistent Object Functions */

TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void *objectID,
				    uint32_t objectIDLen, uint32_t flags,
				    TEE_ObjectHandle *object);

TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void *objectID,
				      uint32_t objectIDLen, uint32_t flags,
				      TEE_ObjectHandle attributes,
				      const void *initialData,
				      uint32_t initialDataLen,
				      TEE_ObjectHandle *object);

void TEE_CloseObject(TEE_ObjectHandle object);

TEE_Result TEE_CloseAndDeletePersistentObject1(TEE_ObjectHandle object);


/* Data and Key Storage API  - Data Stream Access Functions */

TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void *buffer,
			      uint32_t size, uint32_t *count);

TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void *buffer,
			       uint32_t size);

TEE_Result TEE_SeekObjectData(TEE_ObjectHandle object, int32_t offset,
			      TEE_Whence whence);

/* System API - Memory Management */
void *TEE_Malloc(uint32_t size, uint32_t hint);

void *TEE_Realloc(void *buffer, uint32_t newSize);

void TEE_Free(void *buffer);

void *TEE_MemMove(void *dest, const void *src, uint32_t size);

int32_t TEE_MemCompare(const void *buffer1, const void *buffer2, uint32_t size);

void *TEE_MemFill(void *buff, uint32_t x, uint32_t size);


#endif /* TEE_API_H */
