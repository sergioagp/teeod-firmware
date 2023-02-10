#include "sha1.h"
#include <stdint.h>
#include <string.h>

#define ROTL32(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define F1(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define F2(x, y, z) ((x) ^ (y) ^ (z))
#define F3(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define F4(x, y, z) ((x) ^ (y) ^ (z))

#define K1 0x5A827999
#define K2 0x6ED9EBA1
#define K3 0x8F1BBCDC
#define K4 0xCA62C1D6

void SHA1_Init(SHA1_CTX *context) {
  // Initialize the intermediate state of the SHA-1 hash function
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;

  // Initialize the count of the number of bytes processed
  context->count = 0;

  // Clear the input buffer
  memset(context->buffer, 0, SHA1_BLOCK_SIZE);
}

void SHA1_Transform(uint32_t state[5], const uint8_t block[SHA1_BLOCK_SIZE]) {
  uint32_t a, b, c, d, e;
  uint32_t w[80];

  // Initialize the message schedule
  for (size_t t = 0; t < 16; t++) {
    w[t] = (block[t * 4] << 24) | (block[t * 4 + 1] << 16) |
           (block[t * 4 + 2] << 8) | (block[t * 4 + 3]);
  }
  for (size_t t = 16; t < 80; t++) {
    w[t] = ROTL32(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);
  }

  // Initialize the five working variables
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];

  // Perform the main hash function
  for (size_t t = 0; t < 80; t++) {
    uint32_t f, k;
    if (t < 20) {
      f = (b & c) | (~b & d);
      k = 0x5A827999;
    } else if (t < 40) {
      f = b ^ c ^ d;
      k = 0x6ED9EBA1;
    } else if (t < 60) {
      f = (b & c) | (b & d) | (c & d);
      k = 0x8F1BBCDC;
    } else {
      f = b ^ c ^ d;
      k = 0xCA62C1D6;
    }
    uint32_t temp = ROTL32(a, 5) + f + e + k + w[t];
    e = d;
    d = c;
    c = ROTL32(b, 30);
    b = a;
    a = temp;
  }

  // Update the intermediate state
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
}

void SHA1_Update(SHA1_CTX *context, const uint8_t *data, size_t len) {
  size_t i;

  // Update the count of the number of bytes processed
  context->count += len;

  // Process as many complete blocks as possible
  for (i = 0; i + SHA1_BLOCK_SIZE <= len; i += SHA1_BLOCK_SIZE) {
    SHA1_Transform(context->state, data + i);
  }

  // Save the remaining bytes in the input buffer
  size_t remaining = len - i;
  memcpy(context->buffer, data + i, remaining);
}

void SHA1_Final(SHA1_CTX *context, uint8_t digest[SHA1_DIGEST_SIZE]) {
  // Pad the input data with a single "1" bit and as many "0" bits as necessary
  size_t index = context->count % SHA1_BLOCK_SIZE;
  context->buffer[index++] = 0x80;
  if (index > SHA1_BLOCK_SIZE - 8) {
    memset(context->buffer + index, 0, SHA1_BLOCK_SIZE - index);
    SHA1_Transform(context->state, context->buffer);
    memset(context->buffer, 0, SHA1_BLOCK_SIZE);
  } else {
    memset(context->buffer + index, 0, SHA1_BLOCK_SIZE - 8 - index);
  }

  // Append the total number of bits in the input data
  uint64_t count = context->count * 8;
  context->buffer[56] = count >> 56;
  context->buffer[57] = count >> 48;
  context->buffer[58] = count >> 40;
  context->buffer[59] = count >> 32;
  context->buffer[60] = count >> 24;
  context->buffer[61] = count >> 16;
  context->buffer[62] = count >> 8;
  context->buffer[63] = count;
  SHA1_Transform(context->state, context->buffer);

  // Return the final SHA-1 hash
  for (size_t i = 0; i < 5; i++) {
    uint32_t word = context->state[i];
    for (size_t j = 0; j < 4; j++) {
      *digest++ = word >> 24;
      word <<= 8;
    }
  }
}


