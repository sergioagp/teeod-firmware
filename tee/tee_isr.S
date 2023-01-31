# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

//#include "platform.h"
//#include "common.h"

//#include "tee_entry.h"

# Make sure that the GNU assembler is using a modern syntax for Arm THUMB.
.syntax unified
.extern __tee_entry


# -------------------------------------------------------------
.section        .data.tee_isr
# -------------------------------------------------------------

# -------------------------------------------------------------
.section        .text.tee_isr
# -------------------------------------------------------------

# -------------------------------------------------------------
.align 2
.global         __tee_setup
.type           __tee_setup, %function
__tee_setup:
	push    {lr}
    ldr     r0, =0xE000E100
    //ldr     r1, =TEE_INT_Msk
    str     r1, [r0]
    pop     {pc}

.global         TEE_IRQHandler
.type           TEE_IRQHandler, %function
TEE_IRQHandler:
    //  clear interrupt
	push    {lr}
    //ldr     r0, =ASM_TEE_MSG_ARG_INTCLR_ADDR
    ldr     r1, =0
    str     r1, [r0]      
    bl      __tee_entry
    @ // validate DONE before go back to idle
    @ ldr     r1, =ASM_TEE_MSG_ARG_DONE_ADDR
    @ str     r0, [r1]         
    pop     {pc}
