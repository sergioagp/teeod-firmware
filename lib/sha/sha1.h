#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20

typedef struct {
  uint32_t state[5];
  uint64_t count;
  uint8_t buffer[SHA1_BLOCK_SIZE];
} SHA1_CTX;

void sha1_Init(SHA1_CTX *context);
void sha1_Update(SHA1_CTX *context, const uint8_t *data, size_t len);
void sha1_Final(SHA1_CTX *context, uint8_t digest[SHA1_DIGEST_SIZE]);

#endif // SHA1_H