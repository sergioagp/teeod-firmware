#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_defines.h>
#include <tee_sharedmem.h>

/* Data and Key Storage API  - Persistent Object Functions */
TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void *objectID,
				      uint32_t objectIDLen, uint32_t flags,
				      TEE_ObjectHandle attributes,
				      const void *initialData,
				      uint32_t initialDataLen,
				      TEE_ObjectHandle *object) {
  // Validate input parameters
  if (objectID == NULL || objectIDLen == 0 || object == NULL) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  // Check storage ID
  if (storageID != TEE_STORAGE_PRIVATE) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  //TODO: Check if object already exists

  // Create new object
  TEE_Attribute* newObject = malloc(sizeof( TEE_Attribute));
  newObject->attributeID = storageID;
  newObject->content.ref.buffer = (uint32_t*) TEE_SECURE_STORAGE_MEM;

  // Set object attributes if provided

  // Persist object


  // Return object handle
  *object = (TEE_ObjectHandle*) newObject;

  return TEE_SUCCESS;
}


TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void *objectID,
				                            uint32_t objectIDLen, uint32_t flags,
				                            TEE_ObjectHandle *object) {
  // Create new object
  TEE_Attribute* newObject = malloc(sizeof( TEE_Attribute));
  newObject->attributeID = storageID;
  newObject->content.ref.buffer = (uint32_t*) TEE_SECURE_STORAGE_MEM;
  newObject->content.ref.length = 0;
  // Set object attributes if provided

  // Persist object


  // Return object handle
  *object = (TEE_ObjectHandle*) newObject;

  return TEE_SUCCESS;

}


void TEE_CloseObject(TEE_ObjectHandle object) {

}

TEE_Result TEE_CloseAndDeletePersistentObject1(TEE_ObjectHandle object) {

}


/* Data and Key Storage API  - Data Stream Access Functions */

TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void *buffer,
			      uint32_t size, uint32_t *count) {
    if (!object || !buffer || !count) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    
    TEE_Attribute* object_ptr = (TEE_Attribute* )object;
    if (object_ptr->content.ref.length > size) {
        memcpy(buffer, (uint32_t*)object_ptr->content.ref.buffer, size);
        *count = size;
    } else {
        memcpy(buffer, object_ptr->content.ref.buffer, object_ptr->content.ref.length);
        *count = object_ptr->content.ref.length;
    }
    
    return TEE_SUCCESS;
}

TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void *buffer,
			       uint32_t size) {
  if (!object || !buffer) {
      return TEE_ERROR_BAD_PARAMETERS;
  }

  TEE_Attribute* object_ptr = (TEE_Attribute* ) object;
  
  memcpy(object_ptr->content.ref.buffer, buffer, size);
  object_ptr->content.ref.length += size;

  return TEE_SUCCESS;
}

TEE_Result TEE_SeekObjectData(TEE_ObjectHandle object, int32_t offset,
			      TEE_Whence whence) {

}
