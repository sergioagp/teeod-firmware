#ifndef AES_ECB_H
#define AES_ECB_H

#include <stdint.h>
#include <stdlib.h>


// Key size in bytes
#define AES_128_KEY_SIZE 16
#define AES_192_KEY_SIZE 24
#define AES_256_KEY_SIZE 32

// Round size in bytes
#define AES_128_ROUND_SIZE 176
#define AES_192_ROUND_SIZE 208
#define AES_256_ROUND_SIZE 240

#define AES_BLOCK_SIZE AES_128_KEY_SIZE
#define AES_ROUND_SIZE AES_128_ROUND_SIZE 

// Structure for holding the context of an AES-ECB encryption/decryption
typedef struct {
uint8_t key[AES_BLOCK_SIZE]; // key
uint8_t round_keys[AES_ROUND_SIZE]; // round keys
} aes_ecb_ctx_t;

// Initialize the AES-ECB context with the given key
void aes_ecb_init(aes_ecb_ctx_t* ctx, const uint8_t* key);

// Encrypt a single block of data (16 bytes) with AES-ECB
void aes_ecb_encrypt(aes_ecb_ctx_t* ctx, const uint8_t* in, uint8_t* out);

// Decrypt a single block of data (16 bytes) with AES-ECB
void aes_ecb_decrypt(aes_ecb_ctx_t* ctx, const uint8_t* in, uint8_t* out);

#endif // AES_ECB_H