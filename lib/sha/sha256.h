#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

#define SHA256_BLOCK_SIZE 32

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    unsigned long long bitlen;
    uint32_t state[8];
} SHA256_CTX;

void tee_sha256_init(SHA256_CTX *ctx);
void tee_sha256_update(SHA256_CTX *ctx, const uint8_t *data, size_t len);
void tee_sha256_final(SHA256_CTX *ctx, uint8_t *hash);

#endif /* SHA256_H */