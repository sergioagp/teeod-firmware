#include <string.h>
#include "aes-ecb.h"

// Test vectors
const uint8_t key[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
const uint8_t in[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
const uint8_t expected_out[] = {
  0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
  0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A
};

int main(void) {
  uint8_t out[AES_BLOCK_SIZE];
  uint8_t in2[AES_BLOCK_SIZE];

  // Decrypt the output block
  aes_128_ecb_decrypt(key, expected_out, in2);

  // Encrypt the input block
  aes_128_ecb_encrypt(key, in2, out);



  // Compare the encrypted and decrypted blocks with the expected values
  if (memcmp(out, expected_out, AES_BLOCK_SIZE) != 0) {
    // Error: Encrypted output does not match expected value
    return -1;
  }

  if (memcmp(in2, in, AES_BLOCK_SIZE) != 0) {
    // Error: Decrypted output does not match expected value
    return -1;
  }

  return 0;
}
