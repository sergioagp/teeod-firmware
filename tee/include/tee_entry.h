#ifndef TEE_ENTRY_H
#define TEE_ENTRY_H
#include "tee_args.h"
/*
 * Standard call entry, __weak, overridable. If overridden should call
 * __tee_entry() at the end in order to handle the standard functions.
 *
 * These functions are called in a normal thread context.
 */
uint32_t tee_entry();
uint32_t __tee_entry();
void entry_open_session(struct mb_args * arg, size_t num_params);
void entry_close_session(struct mb_args * arg, size_t num_params);
void entry_invoke_command(struct mb_args * arg, size_t num_params);

#endif /* TEE_ENTRY_STD_H */
