#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_types.h>

#include "aes.h"
#include "sha.h"
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
      EMSG("TEE_AllocateOperation: out of memory");
      return TEE_ERROR_OUT_OF_MEMORY;
    }
    memset(op, 0, sizeof(Crypto_Operation));
    op->algorithm = algorithm;
    op->mode = mode;
    op->maxKeySize = maxKeySize;
    *operation = (TEE_OperationHandle)op;
    return TEE_SUCCESS;
    break;

  case TEE_ALG_SHA1:
  case TEE_ALG_SHA256:
    if (mode != TEE_MODE_DIGEST) {
      EMSG("TEE_AllocateOperation: bad parameters");
      return TEE_ERROR_NOT_SUPPORTED;
    }
    if (op == NULL) {
      EMSG("TEE_AllocateOperation: out of memory");
      return TEE_ERROR_OUT_OF_MEMORY;
    }

    memset(op, 0, sizeof(Crypto_Operation));
    op->algorithm = algorithm;
    op->mode = mode;
    op->maxKeySize = maxKeySize;

    if (algorithm == TEE_ALG_SHA1) {
      sha1_Init(&op->sha1_ctx);
    } else if (algorithm == TEE_ALG_SHA256) {
      sha256_Init(&op->sha256_ctx);
    }

    *operation = (TEE_OperationHandle)op;
    return TEE_SUCCESS;
    break;  
  default:
    EMSG("TEE_AllocateOperation: bad parameters");
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
  EVAL_INIT();
  if (!op || !hash || !hashLen) {
    EMSG("TEE_DigestDoFinal: bad parameters");
    return TEE_ERROR_BAD_PARAMETERS;
  }

  switch (op->algorithm) {
    case TEE_ALG_SHA1:
      if (*hashLen < SHA1_DIGEST_LENGTH) {
        EMSG("TEE_DigestDoFinal: short buffer");
        return TEE_ERROR_SHORT_BUFFER;
      }
      // IMSG("TEE_DigestDoFinal: SHA1");
       EVAL(sha1_Update(&op->sha1_ctx, chunk, chunkLen);
            sha1_Final(&op->sha1_ctx, hash));
      *hashLen = SHA1_DIGEST_LENGTH;
      break;

    case TEE_ALG_SHA256:
      if (*hashLen < SHA256_DIGEST_LENGTH) {
        EMSG("TEE_DigestDoFinal: short buffer");
        return TEE_ERROR_SHORT_BUFFER;
      }
      // IMSG("TEE_DigestDoFinal: SHA256");
       EVAL(sha256_Update(&op->sha256_ctx, chunk, chunkLen);
            sha256_Final(&op->sha256_ctx, hash));
      *hashLen = SHA256_DIGEST_LENGTH;
      break;

    default:
      EMSG("TEE_DigestDoFinal: bad parameters");
      return TEE_ERROR_NOT_IMPLEMENTED;
  }

  return TEE_SUCCESS;
}

TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation,
			       TEE_ObjectHandle key) {

  if (!operation || !key) {
    EMSG("TEE_SetOperationKey: bad parameters");
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
    EMSG("TEE_SetOperationKey: bad parameters");
    return TEE_ERROR_NOT_SUPPORTED;
  }

}


TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation,
				TEE_ObjectHandle key1, TEE_ObjectHandle key2){

  if (!operation || !key1 || !key2) {
    EMSG("TEE_SetOperationKey2: bad parameters");
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
    EMSG("TEE_SetOperationKey2: bad parameters");
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
  EVAL_INIT();
  if (!operation || !srcData || !destData) {
    EMSG("TEE_CipherUpdate: bad parameters");
    return TEE_ERROR_BAD_PARAMETERS;
  }

  uint8_t* buf = malloc(srcLen);
  memcpy(buf, srcData, srcLen);
  Crypto_Operation *op = (Crypto_Operation *)operation;

  switch (op->algorithm)
  {
  case TEE_ALG_AES_ECB_NOPAD:
    EVAL(AES_ECB_encrypt(&op->aes_ctx, buf));
    break;
  case TEE_ALG_AES_CBC_NOPAD:
     EVAL(AES_CBC_encrypt_buffer(&op->aes_ctx, buf, srcLen));
    break;
  case TEE_ALG_AES_CTR:
     EVAL(AES_CTR_xcrypt_buffer(&op->aes_ctx, buf, srcLen));
  case TEE_ALG_AES_XTS:
    break;
  default:
    EMSG("TEE_CipherUpdate: bad parameters");
    return TEE_ERROR_NOT_SUPPORTED;
  }

  memcpy(destData, buf, srcLen);
  free(buf);
  return TEE_SUCCESS;
}

void TEE_MACInit(TEE_OperationHandle operation, const void *IV,
		 uint32_t IVLen) {

}

void TEE_MACUpdate(TEE_OperationHandle operation, const void *chunk,
		   uint32_t chunkSize) {

}

TEE_Result TEE_MACComputeFinal(TEE_OperationHandle operation,
			       const void *message, uint32_t messageLen,
			       void *mac, uint32_t *macLen) {
  return TEE_SUCCESS;
}

TEE_Result TEE_AsymmetricSignDigest(TEE_ObjectHandle key,
                                    const TEE_Attribute *params,
                                    uint32_t paramCount,
                                    const void *digest,
                                    uint32_t digestLen,
                                    void *signature,
                                    uint32_t *signatureLen)
{
    // Check input arguments
    if (key == TEE_HANDLE_NULL || digest == NULL ||
        signature == NULL || signatureLen == NULL) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    return TEE_SUCCESS;
}
