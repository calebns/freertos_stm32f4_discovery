/**
 * @file	startup.S
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

/*
 * Defines the Thread mode privilege level:
 * 0 = Privileged
 * 1 = Unprivileged.
 */
#define CONTROL_nPRIV_PRIV			(0x00)
#define CONTROL_nPRIV_UNPRIV		(0x01)

/*
 * Defines the currently active stack pointer: In Handler mode this bit reads
 * as zero and ignores writes. The Cortex-M4 updates this bit automatically on
 * exception return.
 * 0 = MSP is the current stack pointer
 * 1 = PSP is the current stack pointer.
 */
#define CONTROL_SPSEL_MSP		(0x00)
#define CONTROL_SPSEL_PSP		(0x02)

.text
.balign 2
.syntax unified
.thumb
.thumb_func
.global Reset_Handler

Reset_Handler:

	/* Setup user-process stack */
	ldr		r0, =__process_stack_end
	msr		PSP, r0

	/* Thread-mode is privileged and uses PSP */
	movs	r0, #(CONTROL_SPSEL_PSP | CONTROL_nPRIV_PRIV)
	msr		CONTROL, r0
	isb

	/* Copy __data_init_start to __data_start */
	ldr		r1, =__data_init_start
	ldr		r2, =__data_start
	ldr		r3, =__data_end
data_init_start:
	ldr		r0, [r1], #4
	str		r0, [r2], #4
	cmp		r2, r3
	bls		data_init_start

	/* Zero BSS section */
	movs	r0, #0
	ldr		r1, =__bss_start
	ldr		r2, =__bss_end
zero_bss_start:
	str		r0, [r1], #4
	cmp		r1, r2
	bls		zero_bss_start

#ifdef __USES_CXX
	/* Call c++ constructors */
	ldr		r0, =__libc_init_array
	blx		r0
#endif

	/* Jump to C-code */
	ldr		r0, =low_level_init
	blx		r0

#ifdef __USES_CXX
	/* Call static objects destructors */
	ldr		r0, =__libc_fini_array
	blx		r0
#endif

	/* Forever loop */
	b		.
