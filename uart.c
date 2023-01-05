#include "uart.h"

volatile unsigned int * const RxR = (unsigned int *)0x40600000;
volatile char * const TxR = (char *)0x40600004;
volatile unsigned int * const STR = (unsigned int *)0x40600008;


#define UARTCLK 100000000
#define UARTBAUD 115200
#define UARTDIV (UARTCLK/(16*UARTBAUD))

void config_uart0(){
    // //zero out registers
    // *LCR=0; //set LCR first to access others
    // *IER=0;
    // *IIR=0;
    // *FCR=0;
    // *MCR=0;
    // *SCR=0;
    // //set baud
    // *LCR = 0x80;
    // *DLL = UARTDIV;
    // *DLM = 0x00;

    // //word length 8 bits, 1 stop, no parity
    // *LCR=0x03;
    // //Disable interrupts
    // *IER=0x0;

}

inline void putchar_uart0(char character){
    while ((*STR & 0x08) == 0x08){} //wait for it to finish
    *TxR= character; /* Transmit char */
}

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        while ((*STR & 0x08) == 0x08){} //wait for it to finish
        *TxR= (unsigned int)(*s++); /* Transmit char */
    }
}
