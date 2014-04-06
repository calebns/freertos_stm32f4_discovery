/**
 * @file	sysclk.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#ifndef SYSCLK_H_
#define SYSCLK_H_

#include <stdint.h>
#include <core/stm32f4xx.h>
#include <config.h>

typedef enum {
	SYSCLK_CLOCK_SYS,
	SYSCLK_CLOCK_AHB,
	SYSCLK_CLOCK_APB1,
	SYSCLK_CLOCK_APB2,
	SYSCLK_CLOCK_VCO
} sysclk_clock_type_e;

void sysclk_delay(uint32_t count);
void sysclk_init(void);
uint32_t sysclk_get_clock(sysclk_clock_type_e type);

#endif /* SYSCLK_H_ */
