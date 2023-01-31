#ifndef TEE_SESSION_H
#define TEE_SESSION_H

#include <stdint.h>

#define SESSION_HASH_SIZE 8

typedef struct {
  uint32_t session_id;
  // Other session-specific variables
  // ...
} TEE_Session;

TEE_Session* create_session();
TEE_Session* get_session(uint32_t session_id);
void delete_session(uint32_t session_id);

#endif