#ifndef AES_H
#define AES_H

#include <stdint.h>

#define AES_BLOCK_SIZE 16

#define Nb 4
#define Nk 4
#define Nr 10

#define Nr_key_add (4 * (Nr + 1))
#define Nb_inv_sbox (4 * Nb)

void aes_128_ecb_encrypt(const uint8_t* key, const uint8_t* plaintext, uint8_t* ciphertext);
void aes_128_ecb_decrypt(const uint8_t* key, const uint8_t* ciphertext, uint8_t* plaintext);

#endif // AES_H
