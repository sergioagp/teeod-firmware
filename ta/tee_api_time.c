#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_defines.h>
/* Date & Time API */

#include <stdint.h>
#define SYSTICK_VAL  (*((volatile uint32_t *)0xE000E018))

extern uint32_t systick_counter_ms;
// Get System Time Function
void TEE_GetSystemTime(TEE_Time *time) {
    // Get Milliseconds Counter
    uint32_t ms = systick_counter_ms;

    // Fill TEE_Time Structure
    time->seconds = ms / 1000;
    time->millis = ms % 1000;
}
