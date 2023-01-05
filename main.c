#include <string.h>
#include "aes.h"

// Test vectors
static const uint8_t key[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
static const uint8_t in[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
static const uint8_t expected_out[] = {
  0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
  0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A
};

int main(void) {
// Initialize context
struct AES_ctx ctx;
AES_init_ctx(&ctx, key);

// Encrypt input
uint8_t out[sizeof(in)];
memcpy(out, in, sizeof(in));
AES_ECB_encrypt(&ctx, out);

// Check if output matches expected output
if (memcmp(out, expected_out, sizeof(out)) != 0) {
  // Error: Encryption failed.
  return -1;
}

// Decrypt output
AES_ECB_decrypt(&ctx, out);

// Check if decrypted output matches original input
if (memcmp(out, in, sizeof(out)) != 0) {
  // Error: Encryption failed.
  return -1;
}

return 0;
}
