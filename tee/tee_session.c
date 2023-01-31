#include "tee_session.h"
#include <stdlib.h>

TEE_Session* session_hash_table[SESSION_HASH_SIZE];

uint32_t generate_unique_session_id() {
    static uint32_t next_id = 0;
    return next_id++;
}

TEE_Session* create_session() {
    TEE_Session *session = (TEE_Session *) malloc(sizeof(TEE_Session));
    // Generate a unique session ID
    session->session_id = generate_unique_session_id();
    // Initialize other session variables as needed
    // ...

    // store the session in a hash table
    uint32_t index = session->session_id % SESSION_HASH_SIZE;
    session_hash_table[index] = session;
    return session;
}

TEE_Session* get_session(uint32_t session_id) {
    // Lookup the session in a data structure, such as a hash table
    // using the session_id as the key
    uint32_t index = session_id % SESSION_HASH_SIZE;
    return session_hash_table[index];
}

void delete_session(uint32_t session_id) {
    TEE_Session *session = get_session(session_id);
    // Free any allocated memory for the session
    free(session);
    // Remove the session from the data structure
    uint32_t index = session_id % SESSION_HASH_SIZE;
    session_hash_table[index] = NULL;
}