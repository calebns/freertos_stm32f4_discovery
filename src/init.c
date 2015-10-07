/**
 * @file	init.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#include <stdint.h>
#include <sys/stat.h>
#include <core/stm32f4xx.h>
#include <config.h>
#include <sysclk.h>

static void		fpu_enable(void);

extern void		main(void);



/**
 * @brief Enables FPU
 *
 * Enables FPU in Cortex-M4 for both privileged and user mode. This is
 * done by enabling CP10 and CP11 coprocessors in CPACR register (possible only
 * when in privileged mode).
 */
static void
fpu_enable(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));// set CP10 and CP11 Full Access
#endif
}

#define AIRC_VECTKEY_MASK    ((u32)0x05FA0000)

/**
 * @brief Low level init
 *
 * This function is the first C routine called from assembler. It must
 * configure PLL and coprocessors. At the end, main() is called.
 */
void
low_level_init(void)
{

	/* Perform low-level initialization */
	fpu_enable();

	/* Set NVIC vector table to FLASH */
	SCB->VTOR = 0x8000000;

	/* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup_4 value */
	SCB->AIRCR = AIRC_VECTKEY_MASK | 0x300;

	sysclk_init();

	/* Run user entry function */
	main();

	/* Should never get here... */
	for (;;) {};
}

caddr_t
_sbrk(int incr)
{
	/* Defined by the linker */
	extern char __heap_start;
	extern char __heap_end;
	static char *heap_end = (void*)0;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &__heap_start;
	}
	prev_heap_end = heap_end;

	if (heap_end + incr > &__heap_end) {
		/* Heap and stack collision */
		return (caddr_t) 0;
	}

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}
