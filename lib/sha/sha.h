/**
 * Copyright (c) 2000-2001 Aaron D. Gifford
 * Copyright (c) 2013-2014 Pavol Rusnak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __SHA2_H__
#define __SHA2_H__

#include <stdint.h>
#include <stddef.h>

#define   tee_SHA1_BLOCK_LENGTH		64
#define   tee_SHA1_DIGEST_LENGTH		20
#define   tee_SHA1_DIGEST_STRING_LENGTH	(tee_SHA1_DIGEST_LENGTH   * 2 + 1)
#define tee_SHA256_BLOCK_LENGTH		64
#define tee_SHA256_DIGEST_LENGTH		32
#define tee_SHA256_DIGEST_STRING_LENGTH	(tee_SHA256_DIGEST_LENGTH * 2 + 1)
#define tee_SHA512_BLOCK_LENGTH		128
#define tee_SHA512_DIGEST_LENGTH		64
#define tee_SHA512_DIGEST_STRING_LENGTH	(tee_SHA512_DIGEST_LENGTH * 2 + 1)

typedef struct _tee_SHA1_CTX {
	uint32_t	state[5];
	uint64_t	bitcount;
	uint32_t	buffer[tee_SHA1_BLOCK_LENGTH/sizeof(uint32_t)];
} tee_SHA1_CTX;
typedef struct _tee_SHA256_CTX {
	uint32_t	state[8];
	uint64_t	bitcount;
	uint32_t	buffer[tee_SHA256_BLOCK_LENGTH/sizeof(uint32_t)];
} tee_SHA256_CTX;
typedef struct _tee_SHA512_CTX {
	uint64_t	state[8];
	uint64_t	bitcount[2];
	uint64_t	buffer[tee_SHA512_BLOCK_LENGTH/sizeof(uint64_t)];
} tee_SHA512_CTX;

/*** ENDIAN REVERSAL MACROS *******************************************/
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321
#endif

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#define REVERSE32(w,x)	{ \
	uint32_t tmp = (w); \
	tmp = (tmp >> 16) | (tmp << 16); \
	(x) = ((tmp & 0xff00ff00UL) >> 8) | ((tmp & 0x00ff00ffUL) << 8); \
}
#define REVERSE64(w,x)	{ \
	uint64_t tmp = (w); \
	tmp = (tmp >> 32) | (tmp << 32); \
	tmp = ((tmp & 0xff00ff00ff00ff00ULL) >> 8) | \
	      ((tmp & 0x00ff00ff00ff00ffULL) << 8); \
	(x) = ((tmp & 0xffff0000ffff0000ULL) >> 16) | \
	      ((tmp & 0x0000ffff0000ffffULL) << 16); \
}
#endif /* BYTE_ORDER == LITTLE_ENDIAN */

extern const uint32_t tee_sha256_initial_hash_value[8];
extern const uint64_t tee_sha512_initial_hash_value[8];

void tee_sha1_Transform(const uint32_t* state_in, const uint32_t* data, uint32_t* state_out);
void tee_sha1_Init(tee_SHA1_CTX *);
void tee_sha1_Update(tee_SHA1_CTX*, const uint8_t*, size_t);
void tee_sha1_Final(tee_SHA1_CTX*, uint8_t[tee_SHA1_DIGEST_LENGTH]);
char* tee_sha1_End(tee_SHA1_CTX*, char[tee_SHA1_DIGEST_STRING_LENGTH]);
void tee_sha1_Raw(const uint8_t*, size_t, uint8_t[tee_SHA1_DIGEST_LENGTH]);
char* tee_sha1_Data(const uint8_t*, size_t, char[tee_SHA1_DIGEST_STRING_LENGTH]);

void tee_sha256_Transform(const uint32_t* state_in, const uint32_t* data, uint32_t* state_out);
void tee_sha256_Init(tee_SHA256_CTX *);
void tee_sha256_Update(tee_SHA256_CTX*, const uint8_t*, size_t);
void tee_sha256_Final(tee_SHA256_CTX*, uint8_t[tee_SHA256_DIGEST_LENGTH]);
char* tee_sha256_End(tee_SHA256_CTX*, char[tee_SHA256_DIGEST_STRING_LENGTH]);
void tee_sha256_Raw(const uint8_t*, size_t, uint8_t[tee_SHA256_DIGEST_LENGTH]);
char* tee_sha256_Data(const uint8_t*, size_t, char[tee_SHA256_DIGEST_STRING_LENGTH]);

void tee_sha512_Transform(const uint64_t* state_in, const uint64_t* data, uint64_t* state_out);
void tee_sha512_Init(tee_SHA512_CTX*);
void tee_sha512_Update(tee_SHA512_CTX*, const uint8_t*, size_t);
void tee_sha512_Final(tee_SHA512_CTX*, uint8_t[tee_SHA512_DIGEST_LENGTH]);
char* tee_sha512_End(tee_SHA512_CTX*, char[tee_SHA512_DIGEST_STRING_LENGTH]);
void tee_sha512_Raw(const uint8_t*, size_t, uint8_t[tee_SHA512_DIGEST_LENGTH]);
char* tee_sha512_Data(const uint8_t*, size_t, char[tee_SHA512_DIGEST_STRING_LENGTH]);

#endif
