#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include <tee_api.h>
#include <tee_ipc.h>
#include <tee_entry.h>
#include <string.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define TEST_OBJECT_ID "TestObject"
#define TEST_OBJECT_ID_LEN 11
#define TEST_STORAGE_ID 0x12345678
#define TEST_FLAGS TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ
#define TEST_ATTRIBUTES 0
#define TEST_INITIAL_DATA "Initial Data"
#define TEST_INITIAL_DATA_LEN 12
#define TEST_SIZE 20
#define TEST_OFFSET 6


void tee_service_routine()
{
  //TODO: change from pulling program to interrupt service
  TEE_Operation *op = (TEE_Operation *)TEE_IPC_ADRR;
  /* Loop forever */
	for(;;) {
    while(op->type == 0);
    __tee_entry();
    op->ret =  0x00000000;
    op->type = 0;
    //fflush(stdout);
  }
}


int main() {

  printf("\033[2J");  // sends the ASCII escape sequence to clear the console screen
  TEE_Result res;
  TEE_ObjectHandle object;

  // Create persistent object
  res = TEE_CreatePersistentObject(TEST_STORAGE_ID, TEST_OBJECT_ID, TEST_OBJECT_ID_LEN, TEST_FLAGS,
  TEST_ATTRIBUTES, TEST_INITIAL_DATA, TEST_INITIAL_DATA_LEN, &object);
  if (res != TEE_SUCCESS) {
    printf("Error in creating persistent object: %x\n", res);
  return 1;
  }

  // Open persistent object
  res = TEE_OpenPersistentObject(TEST_STORAGE_ID, TEST_OBJECT_ID, TEST_OBJECT_ID_LEN, TEST_FLAGS, &object);
  if (res != TEE_SUCCESS) {
    printf("Error in opening persistent object: %x\n", res);
  return 1;
  }

  char buf_wr[12] = TEST_INITIAL_DATA;

  // Write to object
  res = TEE_WriteObjectData(object, buf_wr, TEST_INITIAL_DATA_LEN);
  if (res != TEE_SUCCESS) {
    printf("Error in writing to object: %x\n", res);
  return 1;
  }

  // Seek object data
  res = TEE_SeekObjectData(object, TEST_OFFSET, TEE_DATA_SEEK_SET);
  if (res != TEE_SUCCESS) {
    printf("Error in seeking object data: %x\n", res);
  return 1;
  }

  // Read object data
  char buffer[TEST_INITIAL_DATA_LEN] ={0};
  uint32_t count;
  res = TEE_ReadObjectData(object, buffer, TEST_INITIAL_DATA_LEN, &count);
  if (res != TEE_SUCCESS) {
    printf("Error in reading object data: %x\n", res);
  return 1;
  }

  // Check if read data is the same as the initial data
  if (strncmp(buffer, &buf_wr[TEST_OFFSET], count) != 0) {
    printf("Error in checking read data\n");
  return 1;
  }

  // Close object
  TEE_CloseObject(object);

  // Close and delete persistent object
  res = TEE_CloseAndDeletePersistentObject1(object);
  if (res != TEE_SUCCESS) {
    printf("Error in closing and deleting persistent object: %x\n", res);
  }

  tee_service_routine();
  return 0;
}

