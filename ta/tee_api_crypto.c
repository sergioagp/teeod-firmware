#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_types.h>

#include "aes.h"
#include "sha1.h"
#include "sha256.h"

typedef struct {
  uint32_t algorithm;
  uint32_t mode;
  uint32_t maxKeySize;
  union {
    SHA1_CTX sha1_ctx;
    SHA256_CTX sha256_ctx;
    struct AES_ctx aes_ctx;
  };
} Crypto_Operation;

TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation, uint32_t algorithm, uint32_t mode, uint32_t maxKeySize) {

  Crypto_Operation *op = (Crypto_Operation *)malloc(sizeof(Crypto_Operation));
  
  switch (algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
  case TEE_ALG_AES_CBC_NOPAD:
  case TEE_ALG_AES_CTR:
  case TEE_ALG_AES_XTS:
    if (op == NULL) {
      return TEE_ERROR_OUT_OF_MEMORY;
    }
    memset(op, 0, sizeof(struct AES_ctx));
    op->algorithm = algorithm;
    op->mode = mode;
    op->maxKeySize = maxKeySize;
    *operation = (TEE_OperationHandle)op;
    return TEE_SUCCESS;
    break;

  case TEE_ALG_SHA1:
  case TEE_ALG_SHA256:
    if (mode != TEE_MODE_DIGEST) {
      return TEE_ERROR_NOT_SUPPORTED;
    }
    if (op == NULL) {
      return TEE_ERROR_OUT_OF_MEMORY;
    }

    memset(op, 0, sizeof(Crypto_Operation));
    op->algorithm = algorithm;
    op->mode = mode;
    op->maxKeySize = maxKeySize;

    if (algorithm == TEE_ALG_SHA1) {
      sha1_Init(&op->sha1_ctx);
    } else if (algorithm == TEE_ALG_SHA256) {
      sha256_init(&op->sha256_ctx);
    }

    *operation = (TEE_OperationHandle)op;
    return TEE_SUCCESS;
    break;  
  default:
    return TEE_ERROR_NOT_SUPPORTED;
  }
}

void TEE_FreeOperation(TEE_OperationHandle operation) {
  if (operation == NULL) {
    return;
  }

  free(operation);
}

TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, const void *chunk, uint32_t chunkLen, void *hash, uint32_t *hashLen) {
  Crypto_Operation *op = (Crypto_Operation *)operation;

  if (!op || !hash || !hashLen) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  switch (op->algorithm) {
    case TEE_ALG_SHA1:
      if (*hashLen < SHA1_DIGEST_SIZE) {
        *hashLen = SHA1_DIGEST_SIZE;
        return TEE_ERROR_SHORT_BUFFER;
      }

      sha1_Update(&op->sha1_ctx, chunk, chunkLen);
      sha1_Final(&op->sha1_ctx, hash);
      *hashLen = SHA1_DIGEST_SIZE;
      break;

    case TEE_ALG_SHA256:
      if (*hashLen < SHA256_BLOCK_SIZE) {
        *hashLen = SHA256_BLOCK_SIZE;
        return TEE_ERROR_SHORT_BUFFER;
      }

      sha256_update(&op->sha256_ctx, chunk, chunkLen);
      sha256_final(&op->sha256_ctx, hash);
      *hashLen = SHA256_BLOCK_SIZE;
      break;

    default:
      return TEE_ERROR_NOT_IMPLEMENTED;
  }

  return TEE_SUCCESS;
}

TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation,
			       TEE_ObjectHandle key) {

  if (!operation || !key) {
    return TEE_ERROR_BAD_PARAMETERS;
  }
  Crypto_Operation *op = (Crypto_Operation *)operation;
  TEE_DataHandle *obj = (TEE_DataHandle*) key;

  switch (op->algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
  case TEE_ALG_AES_CBC_NOPAD:
  case TEE_ALG_AES_CTR:
  case TEE_ALG_AES_XTS:
    AES_init_ctx(&op->aes_ctx, obj->address);
    return TEE_SUCCESS;
    break;
  default:
    return TEE_ERROR_NOT_SUPPORTED;
  }

}


TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation,
				TEE_ObjectHandle key1, TEE_ObjectHandle key2){

  if (!operation || !key1 || !key2) {
    return TEE_ERROR_BAD_PARAMETERS;
  }
  Crypto_Operation *op = (Crypto_Operation *)operation;
  TEE_DataHandle *obj1 = (TEE_DataHandle*) key1;
  TEE_DataHandle *obj2 = (TEE_DataHandle*) key2;
  (void) obj1;
  (void) obj2;

  switch (op->algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
  case TEE_ALG_AES_CBC_NOPAD:
  case TEE_ALG_AES_CTR:
  case TEE_ALG_AES_XTS:
    //AES_init_ctx_iv(op->aes_ctx, obj1->address, obj2->address);
    return TEE_SUCCESS;
    break;
  default:
    return TEE_ERROR_NOT_SUPPORTED;
  }

}

void TEE_CipherInit(TEE_OperationHandle operation, const void *IV,
		    uint32_t IVLen) {
  (void) IVLen;

  if (!operation || !IV) {
    return;
  }
  Crypto_Operation *op = (Crypto_Operation *)operation;

  switch (op->algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
  case TEE_ALG_AES_CBC_NOPAD:
  case TEE_ALG_AES_CTR:
  case TEE_ALG_AES_XTS:
    AES_ctx_set_iv(&op->aes_ctx, IV);
    return;
    break;
  default:
    return;
  }

}

TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation, const void *srcData,
			    uint32_t srcLen, void *destData, uint32_t *destLen) {
  (void) destLen;
  
  if (!operation || !srcData || !destData) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  uint8_t* buf = malloc(srcLen);
  memcpy(buf, srcData, srcLen);
  Crypto_Operation *op = (Crypto_Operation *)operation;

  switch (op->algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
    AES_ECB_encrypt(&op->aes_ctx, buf);
    break;
  case TEE_ALG_AES_CBC_NOPAD:
    AES_CBC_encrypt_buffer(&op->aes_ctx, buf, srcLen);
    break;
  case TEE_ALG_AES_CTR:
    AES_CTR_xcrypt_buffer(&op->aes_ctx, buf, srcLen);
  case TEE_ALG_AES_XTS:
    break;
  default:
    return TEE_ERROR_NOT_SUPPORTED;
  }

  memcpy(destData, buf, srcLen);
  free(buf);
  return TEE_SUCCESS;
}