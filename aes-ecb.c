#include "aes-ecb.h"
#include <string.h>
#include <stdint.h>

#define mul_by_2(x) ((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00))
#define mul_by_3(x) (mul_by_2(x) ^ (x))


// Lookup tables for the key expansion algorithm
static const uint8_t sbox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// Lookup table for the rcon
static const uint8_t rcon[10] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

// Forward declarations
static void aes_ecb_key_expansion(uint8_t* round_keys, const uint8_t* key);
static void aes_ecb_add_round_key(uint8_t* state, const uint8_t* round_key);
static void aes_ecb_sub_bytes(uint8_t* state);
static void aes_ecb_shift_rows(uint8_t* state);
static void aes_ecb_mix_columns(uint8_t* state);

void aes_ecb_init(aes_ecb_ctx_t* ctx, const uint8_t* key) {
  // Copy the key to the context
  memcpy(ctx->key, key, AES_BLOCK_SIZE);

  // Generate the round keys from the key
  aes_ecb_key_expansion(ctx->round_keys, key);
}

void aes_ecb_encrypt(aes_ecb_ctx_t* ctx, const uint8_t* in, uint8_t* out) {
  // Initialize variables
  uint8_t i;
  uint8_t state[AES_BLOCK_SIZE];

  // Copy the input block to the state
  memcpy(state, in, AES_BLOCK_SIZE);

  // Perform the initial round
  aes_ecb_add_round_key(state, ctx->round_keys);

  // Perform the 9 main rounds
  for (i = 1; i <= 9; ++i) {
    aes_ecb_sub_bytes(state);
    aes_ecb_shift_rows(state);
    aes_ecb_mix_columns(state);
    aes_ecb_add_round_key(state, ctx->round_keys + i * AES_BLOCK_SIZE);
  }

  // Perform the final round
  aes_ecb_sub_bytes(state);
  aes_ecb_shift_rows(state);
  aes_ecb_add_round_key(state, ctx->round_keys + 10 * AES_BLOCK_SIZE);

  // Copy the state to the output block
  memcpy(out, state, AES_BLOCK_SIZE);
}


void aes_ecb_decrypt(aes_ecb_ctx_t* ctx, const uint8_t* in, uint8_t* out) {
  // Initialize variables
  uint8_t i;
  uint8_t state[AES_BLOCK_SIZE];

  // Copy the input block to the state
  memcpy(state, in, AES_BLOCK_SIZE);

  // Perform the initial round
  aes_ecb_add_round_key(state, ctx->round_keys + 10 * AES_BLOCK_SIZE);

  // Perform the 9 main rounds
  for (i = 9; i >= 1; --i) {
    aes_ecb_shift_rows(state);
    aes_ecb_sub_bytes(state);
    aes_ecb_add_round_key(state, ctx->round_keys + i * AES_BLOCK_SIZE);
    aes_ecb_mix_columns(state);
  }

  // Perform the final round
  aes_ecb_shift_rows(state);
  aes_ecb_sub_bytes(state);
  aes_ecb_add_round_key(state, ctx->round_keys);

  // Copy the state to the output block
  memcpy(out, state, AES_BLOCK_SIZE);
}


static void aes_ecb_key_expansion(uint8_t* round_keys, const uint8_t* key) {
  // Initialize variables
  uint8_t temp[4];
  uint8_t i, j;

  // Copy the key to the round keys
  memcpy(round_keys, key, AES_BLOCK_SIZE);

  // Loop through the remaining round keys
  for (i = AES_BLOCK_SIZE; i < AES_ROUND_SIZE; i += 4) {
    // Copy the previous word to the temp buffer
    memcpy(temp, round_keys + i - 4, 4);

    // Perform key expansion
    if (i % AES_BLOCK_SIZE == 0) {
      // Rotate the temp buffer and apply the sbox to each byte
      uint8_t t = temp[0];
      temp[0] = sbox[temp[1]] ^ rcon[i / AES_BLOCK_SIZE - 1];
      temp[1] = sbox[temp[2]];
      temp[2] = sbox[temp[3]];
      temp[3] = sbox[t];
    } else if (AES_BLOCK_SIZE > 6 && i % AES_BLOCK_SIZE == 4) {
      // Apply the sbox to each byte of the temp buffer
      temp[0] = sbox[temp[0]];
      temp[1] = sbox[temp[1]];
      temp[2] = sbox[temp[2]];
      temp[3] = sbox[temp[3]];
    }

    // XOR the temp buffer with the previous word
    for (j = 0; j < 4; ++j) {
      round_keys[i + j] = round_keys[i - AES_BLOCK_SIZE + j] ^ temp[j];
    }
  }
}

static void aes_ecb_add_round_key(uint8_t* state, const uint8_t* round_key) {
  // Initialize variables
  uint8_t i;
  // XOR each byte of the state with the corresponding byte of the round key
  for (i = 0; i < AES_BLOCK_SIZE; ++i) {
    state[i] ^= round_key[i];
  }
}

static void aes_ecb_sub_bytes(uint8_t* state) {
  // Initialize variables
  uint8_t i;
  // Substitute each byte of the state with the corresponding value from the sbox lookup table
  for (i = 0; i < AES_BLOCK_SIZE; ++i) {
    state[i] = sbox[state[i]];
  }
}

static void aes_ecb_shift_rows(uint8_t* state) {
  // Shift the second row by 1 byte to the left
  uint8_t temp = state[1];
  state[1] = state[5];
  state[5] = state[9];
  state[9] = state[13];
  state[13] = temp;

  // Shift the third row by 2 bytes to the left
  temp = state[2];
  state[2] = state[10];
  state[10] = temp;
  temp = state[6];
  state[6] = state[14];
  state[14] = temp;

  // Shift the fourth row by 3 bytes to the left
  temp = state[3];
  state[3] = state[15];
  state[15] = state[11];
  state[11] = state[7];
  state[7] = temp;
}

static void aes_ecb_mix_columns(uint8_t* state) {
  uint8_t i, j;
  uint8_t tmp[4];

  // Mix each column of the state
  for (i = 0; i < 4; ++i) {
    // Copy the column to a temporary buffer
    for (j = 0; j < 4; ++j) {
      tmp[j] = state[i + 4*j];
    }
    // Perform the mixing operation
    state[i]     = mul_by_2(tmp[0]) ^ mul_by_3(tmp[1]) ^ tmp[2] ^ tmp[3];
    state[i + 4] = tmp[0] ^ mul_by_2(tmp[1]) ^ mul_by_3(tmp[2]) ^ tmp[3];
    state[i + 8] = tmp[0] ^ tmp[1] ^ mul_by_2(tmp[2]) ^ mul_by_3(tmp[3]);
    state[i + 12] = mul_by_3(tmp[0]) ^ tmp[1] ^ tmp[2] ^ mul_by_2(tmp[3]);
  }
}
