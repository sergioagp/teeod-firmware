/* Based on GP TEE Internal API Specification Version 1.1 */
#ifndef TEE_API_H
#define TEE_API_H

#include <stddef.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <stdio.h>
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

/* Date & Time API */

void TEE_GetSystemTime(TEE_Time *time);

/* Cryptographic Operations API - Generic Operation Functions */

TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation,
				 uint32_t algorithm, uint32_t mode,
				 uint32_t maxKeySize);

TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation,
			       TEE_ObjectHandle key);

TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation,
				TEE_ObjectHandle key1, TEE_ObjectHandle key2);

void TEE_FreeOperation(TEE_OperationHandle operation);

/* Data and Key Storage API  - Transient Object Functions */
TEE_Result TEE_AllocateTransientObject(TEE_ObjectType objectType,
				       uint32_t maxKeySize,
				       TEE_ObjectHandle *object);

void TEE_FreeTransientObject(TEE_ObjectHandle object);

TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object,
				       const TEE_Attribute *attrs,
				       uint32_t attrCount);

/* Cryptographic Operations API - Message Digest Functions */

TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, const void *chunk,
			     uint32_t chunkLen, void *hash, uint32_t *hashLen);



/* Cryptographic Operations API - Symmetric Cipher Functions */

void TEE_CipherInit(TEE_OperationHandle operation, const void *IV,
		    uint32_t IVLen);

TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation, const void *srcData,
			    uint32_t srcLen, void *destData, uint32_t *destLen);

TEE_Result TEE_CipherDoFinal(TEE_OperationHandle operation,
			     const void *srcData, uint32_t srcLen,
			     void *destData, uint32_t *destLen);/* System API - Memory Management */
TEE_Result TEE_CheckMemoryAccessRights(uint32_t accessFlags, void *buffer,
				       uint32_t size);

void *TEE_Malloc(uint32_t size, uint32_t hint);

void *TEE_Realloc(void *buffer, uint32_t newSize);

void TEE_Free(void *buffer);

void *TEE_MemMove(void *dest, const void *src, uint32_t size);

int32_t TEE_MemCompare(const void *buffer1, const void *buffer2, uint32_t size);

void *TEE_MemFill(void *buff, uint32_t x, uint32_t size);

//TODO: Implement the following functions
// void TEE_GenerateRandom(void *randomBuffer, uint32_t randomBufferLen);

// void TEE_InitRefAttribute(TEE_Attribute *attr, uint32_t attributeID,
//         void *buffer, uint32_t length);
// TEE_InitValueAttribute
// TEE_GetObjectBufferAttribute
// TEE_BigIntInit
// TEE_BigIntConvertFromOctetString
// TEE_BigIntDiv
// TEE_BigIntDiv
// TEE_BigIntConvertToS32
// TEE_BigIntCmpS32

#endif /* TEE_API_H */
