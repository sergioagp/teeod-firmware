#include <stdlib.h>
#include <string.h>
#include <tee_api.h>
#include <tee_api_defines.h>
/* Date & Time API */

#include <stdint.h>

#define SYSTICK_CTRL (*(volatile uint32_t )0xE000E010)
#define SYSTICK_LOAD ((volatile uint32_t )0xE000E014)
#define SYSTICK_VAL ((volatile uint32_t )0xE000E018)
#define SYSTICK_CALIB ((volatile uint32_t *)0xE000E01C)

void TEE_GetSystemTime(TEE_Time *time)
{
  time->seconds = SYSTICK_VAL / 10000000;
  time->millis = (SYSTICK_VAL % 10000000) / 10;
}