#include "system.h"
#include "stdio.h"

// Define the base addresses of the SysTick registers
#define SYSTICK_CTRL (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_LOAD (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_VAL  (*((volatile uint32_t *)0xE000E018))

#define SBIT_ENABLE     0
#define SBIT_TICKINT    1
#define SBIT_CLKSOURCE  2
#define RELOAD_VALUE    99999

#define GPIO_DIR  (volatile uint32_t *)(GPIO_BASE_ADDRESS + GPIO_DIR_OFFSET)
// Define the bit position for the LED pin
#define LED_PIN 0


volatile uint32_t systick_counter_ms = 0;

// Initialize the SysTick timer
void SysTick_Init() {
    // Set the reload value to generate interrupts at the specified frequency
    SYSTICK_LOAD = RELOAD_VALUE;
    
    // Enable the SysTick timer with interrupts
    SYSTICK_CTRL = (1 << SBIT_ENABLE) | (1 << SBIT_TICKINT) | (1 << SBIT_CLKSOURCE);
}


void SystemInit(void)
{
  /* NOTE :SystemInit(): 
                         User can setups the default system clock (System clock source, PLL Multiplier
                         and Divider factors, AHB/APBx prescalers and Flash settings).
   */
  SysTick_Init();
 //  setvbuf(stdout, NULL, _IONBF, 0); // disable buffering stdout, needed so that enters to _write
  setbuf(stdout, NULL);
}

void SysTick_Handler(void)
{
	systick_counter_ms++;
}