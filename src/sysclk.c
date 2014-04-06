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

#include <stdint.h>
#include <core/stm32f4xx.h>
#include <config.h>
#include <sysclk.h>

static uint32_t g_sysclk_freq;
static uint32_t g_ahb_freq;
static uint32_t g_apb1_freq;
static uint32_t g_apb2_freq;
static uint32_t g_vco_freq;

/**
 * @brief	Configure Flash
 *
 * Configure Flash subsystem with desired maximum frequency. Perform cache
 * initialization.
 *
 * @param sysfreq			System frequency
 * @param max_flash_freq	Maximum Flash operation frequency
 */
static void
setup_flash(void)
{
	uint32_t wait_states;

	/* Setup flash waitstates */
	wait_states = FREQUENCY / MAX_FLASH_FREQ;
	wait_states &= FLASH_ACR_LATENCY;

	/* Disable caches and prefetcher */
	FLASH->ACR = wait_states;
	/* Reset flash controller */
	FLASH->ACR |= FLASH_ACR_DCRST | FLASH_ACR_ICRST;
	/* Enable caches and prefetch */
	FLASH->ACR |= FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
}

/**
 * @brief Starts the PLL
 *
 * Configure and enable PLL. This function uses defines provided in the
 * configuration header: PLL_PLLM, PLL_PLLN, PLL_PLLQ, PLL_PLLP.
 * Frequencies are calculated as follows:
 * Fvco = CRYSTAL * (PLL_PLLN / PLL_PLLM)
 * Fsysclk = Fvco / PLL_PLLP
 * Fusb = Fsysclk / PLL_PLLQ
 * Fahb = Fsysclk
 * Fapb1 = Fahb / 4
 * Fapb2 = Fahb / 2
 */
void
sysclk_init(void)
{

	/* Enable HSE */
	RCC->CR |= RCC_CR_HSEON;

	/* Configure Flash prior to PLL modifications */
	setup_flash();

	/* Configure PLL */
	RCC->PLLCFGR = (PLL_PLLM << RCC_PLLCFGR_PLLM_bit) |
		(PLL_PLLN << RCC_PLLCFGR_PLLN_bit) |
		((PLL_PLLP / 2 - 1) << RCC_PLLCFGR_PLLP_bit) |
		(PLL_PLLQ << RCC_PLLCFGR_PLLQ_bit) | RCC_PLLCFGR_PLLSRC_HSE;

	/*
	 * Set divisors:
	 * AHB  - no prescaler
	 * APB1 - divide by 4
	 * APB2 - divide by 2
	 */
	RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_HPRE_DIV1;

	/* Wait until HSE clock is ready */
	while ((RCC->CR & RCC_CR_HSERDY) == 0) {};

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0) {};

	/* Switch SYSCLK to PLL source */
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {};

	/* Setup global variables */
	g_vco_freq = ((uint64_t)CRYSTAL * (uint64_t)PLL_PLLN) / (uint64_t)PLL_PLLM;
	g_sysclk_freq = g_vco_freq / PLL_PLLP;
	g_ahb_freq = g_sysclk_freq;
	g_apb1_freq = g_ahb_freq / 4;
	g_apb2_freq = g_ahb_freq / 2;
}

/**
 * @brief	Get frequency for specified clock source
 *
 * @param type	Type of clock
 *
 * @return	The clock frequency, 0 on error.
 */
uint32_t
sysclk_get_clock(sysclk_clock_type_e type)
{

	switch (type) {
	case SYSCLK_CLOCK_SYS:
		return g_sysclk_freq;
	case SYSCLK_CLOCK_AHB:
		return g_ahb_freq;
	case SYSCLK_CLOCK_APB1:
		return g_apb1_freq;
	case SYSCLK_CLOCK_APB2:
		return g_apb2_freq;
	case SYSCLK_CLOCK_VCO:
		return g_vco_freq;
	default:
		return 0;
	}
}

/**
 * @brief	Delays execution for specified busy-loop cycles
 *
 * @param cycles	Busy loop cycles count
 */
void
sysclk_delay(uint32_t count)
{
	while (count != 0)
		count --;
}
