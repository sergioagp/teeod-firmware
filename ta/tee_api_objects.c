#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_defines.h>
#include <tee_sharedmem.h>

typedef struct {
void* address;
uint32_t size;
uint32_t currpos;
} TEE_DataHandle;

/* Data and Key Storage API  - Persistent Object Functions */
TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void *objectID,
                                      uint32_t objectIDLen, uint32_t flags,
                                      TEE_ObjectHandle attributes,
                                      const void *initialData,
                                      uint32_t initialDataLen,
                                      TEE_ObjectHandle *object) {

  (void) storageID;
  (void) flags;
  (void) attributes;
  // Check if the input parameters are valid
  if (!objectID || !initialData || !object)
  return TEE_ERROR_BAD_PARAMETERS;

  // Check if the objectID length is within the allowed range
  if (objectIDLen > TEE_SECURE_STORAGE_SIZE)
  return TEE_ERROR_BAD_PARAMETERS;

  // Allocate memory for the TEE_DataHandle object
  TEE_DataHandle *objdata = (TEE_DataHandle *)malloc(sizeof(TEE_DataHandle));
  if (!objdata)
  return TEE_ERROR_OUT_OF_MEMORY;

  // Allocate memory for the object data
  objdata->address = (uint8_t*) malloc(sizeof(initialDataLen));
  // Copy the initial data to the object
  memcpy((uint8_t *) objdata->address, (uint8_t *) initialData, initialDataLen);

  // Set the size and current position of the object
  objdata->size = initialDataLen;
  objdata->currpos = 0;

  // Set the object handle to point to the TEE_DataHandle object
  *object = (TEE_ObjectHandle)objdata;

  return TEE_SUCCESS;
}


TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void *objectID,
				                            uint32_t objectIDLen, uint32_t flags,
				                            TEE_ObjectHandle *object) {
  (void) storageID;
  (void) objectID;
  (void) objectIDLen;
  (void) flags;

  // Set the initial data position in the data stream to 0
  TEE_DataHandle *objdata = (TEE_DataHandle *) (void*) object;
  objdata->currpos = 0;
  // Set the handle with the opened object
  // Return TEE_SUCCESS
  return TEE_SUCCESS;
}


void TEE_CloseObject(TEE_ObjectHandle object) {
  (void) object;
}

TEE_Result TEE_CloseAndDeletePersistentObject1(TEE_ObjectHandle object) {
  (void) object;
  return TEE_SUCCESS; 
}


/* Data and Key Storage API  - Data Stream Access Functions */

TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void *buffer, uint32_t size, uint32_t *count) {

  TEE_DataHandle *objdata = (TEE_DataHandle *)(void*)  object;
  if (!objdata->address || !buffer) {
    return TEE_ERROR_BAD_PARAMETERS;
  }
  if (objdata->currpos >= objdata->size) {
  *count = 0;
    return TEE_SUCCESS;
  }

  uint32_t bytes_to_read = objdata->size - objdata->currpos;
  if (bytes_to_read > size) {
    bytes_to_read = size;
  }

  memcpy((uint8_t *) buffer, (uint8_t *) objdata->address + objdata->currpos, bytes_to_read);
  objdata->currpos += bytes_to_read;
  *count = bytes_to_read;

  return TEE_SUCCESS;
}

TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void *buffer,
			       uint32_t size) {

  TEE_DataHandle *objdata = (TEE_DataHandle *) (void*) object;
  if (!objdata->address || !buffer) {
    return TEE_ERROR_BAD_PARAMETERS;
  }
  if (objdata->currpos + size > TEE_SECURE_STORAGE_SIZE) {
    return TEE_ERROR_OVERFLOW;
  }

  // Copy the data from buffer to the object
  memcpy((uint8_t *) objdata->address + objdata->currpos, (uint8_t *) buffer, size);

  // Update the current position
  objdata->currpos += size;

  return TEE_SUCCESS;
}


TEE_Result TEE_SeekObjectData(TEE_ObjectHandle object, int32_t offset,
			      TEE_Whence whence) {
  if (!object) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  TEE_DataHandle *objdata = (TEE_DataHandle *) (void*) object;

  switch (whence) {
    case TEE_DATA_SEEK_SET:
        if (offset > TEE_SECURE_STORAGE_SIZE)
            return TEE_ERROR_OVERFLOW;
        else
            objdata->currpos = offset;
        break;
    case TEE_DATA_SEEK_CUR:
        if (objdata->currpos + offset > TEE_SECURE_STORAGE_SIZE)
            return TEE_ERROR_OVERFLOW;
        else
            objdata->currpos += offset;
        break;
    case TEE_DATA_SEEK_END:
        if (objdata->size + offset > TEE_SECURE_STORAGE_SIZE)
            return TEE_ERROR_OVERFLOW;
        else
            objdata->currpos = objdata->size + offset;
        break;
    default:
        return TEE_ERROR_BAD_PARAMETERS;
  }

  return TEE_SUCCESS;
}
