/**
 * @file	usart.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#include <stdint.h>
#include <stdlib.h>
#include <core/stm32f4xx.h>
#include <gpio.h>
#include <usart.h>
#include <sysclk.h>

static USART_TypeDef*
usart_get_reg_ptr(usart_device_e dev)
{

	switch (dev) {
	case USART_DEVICE_1:
		return USART1;
	case USART_DEVICE_2:
		return USART2;
	case USART_DEVICE_3:
		return USART3;
	case USART_DEVICE_4:
		return UART4;
	case USART_DEVICE_5:
		return UART5;
	case USART_DEVICE_6:
		return USART6;
	}

	return NULL;
}

void
usart_init(usart_cfg_t *cfg)
{
	USART_TypeDef *usart;
	gpio_pin_cfg_t pincfg;
	uint32_t sysclk = 0;

	usart = usart_get_reg_ptr(cfg->device);
	cfg->regs = usart;

	pincfg.mode = GPIO_CFG_MODE_ALTERNATE;
	pincfg.speed = GPIO_CFG_SPEED_50MHZ;
	pincfg.type = GPIO_CFG_TYPE_PP;

	switch (cfg->device) {
	case USART_DEVICE_1:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB2);
		pincfg.alt = GPIO_CFG_ALT_7;
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		break;
	case USART_DEVICE_2:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB1);
		pincfg.alt = GPIO_CFG_ALT_7;
		gpio_pin_cfg(GPIOA, 2, &pincfg);
		gpio_pin_cfg(GPIOA, 3, &pincfg);
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		NVIC_EnableIRQ(USART2_IRQn);
		break;
	case USART_DEVICE_3:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB1);
		pincfg.alt = GPIO_CFG_ALT_7;
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		break;
	case USART_DEVICE_4:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB1);
		pincfg.alt = GPIO_CFG_ALT_8;
		RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
		break;
	case USART_DEVICE_5:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB1);
		pincfg.alt = GPIO_CFG_ALT_8;
		RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
		break;
	case USART_DEVICE_6:
		sysclk = sysclk_get_clock(SYSCLK_CLOCK_APB2);
		pincfg.alt = GPIO_CFG_ALT_8;
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
		break;
	}

	/* Calculate bitrate, oversampling by 16 */
	usart->BRR = (sysclk + (cfg->baudrate / 2)) / cfg->baudrate;
	usart->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
}

/**
 * @brief	Send single character via UART
 *
 * @param cfg	Configuration structure of the UART device
 * @param chr	Character to be sent
 */
void
usart_putc(usart_cfg_t *cfg, const uint32_t chr)
{

	/* Wait for transfer completion */
	while (!(cfg->regs->SR & USART_SR_TXE)) {};

	/* Send char */
	cfg->regs->DR = chr;
}

/**
 * @brief	Receive single character from UART
 *
 * @param cfg	Configuration structure of the UART device
 *
 * @return		Character
 */
uint8_t
usart_getc(usart_cfg_t *cfg)
{
	return (cfg->regs->DR);
}

