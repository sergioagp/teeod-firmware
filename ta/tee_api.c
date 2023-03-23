#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_defines.h>

TEE_Result TEE_CheckMemoryAccessRights(uint32_t accessFlags, void *buffer,
				       uint32_t size) {
  (void) accessFlags;
  (void) buffer;
  (void) size;
  return TEE_SUCCESS;
}

int32_t TEE_MemCompare(const void *buffer1, const void *buffer2, uint32_t size)
{
    const unsigned char *ptr1 = buffer1;
    const unsigned char *ptr2 = buffer2;
    uint32_t i;

    for (i = 0; i < size; i++) {
        if (ptr1[i] != ptr2[i]) {
            return (ptr1[i] < ptr2[i]) ? -1 : 1;
        }
    }

    return 0;
}

void *TEE_MemFill(void *buff, uint32_t x, uint32_t size)
{
	return memset(buff, x, size);
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
  buffer = NULL;
}

void *TEE_MemMove(void *dest, const void *src, uint32_t size)
{
	return memmove(dest, src, size);
}
