/**
 * @file	gpio.c
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
#include <gpio.h>

/**
* @brief	Initialize GPIO
*
* Function enables all GPIO ports available on the processor.
*/
void
gpio_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN
			| RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN
			| RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN
			| RCC_AHB1ENR_GPIOIEN;
}

/**
* @brief Configures pin.
*
* Configure pin to desired functionality.
*
* @param port_ptr	Pointer to PORT register structure
* @param pin		Pin number to be configured
* @param cfg		Pointer to the configuration structure
*/
void
gpio_pin_cfg(GPIO_TypeDef *port_ptr, uint32_t pin, gpio_pin_cfg_t *cfg)
{
	uint32_t val;

	val = port_ptr->MODER;
	val &= ~(GPIO_MODER_MODER0 << (pin * 2));
	val |= ((cfg->mode) << (pin * 2));
	port_ptr->MODER = val;

	val = port_ptr->OTYPER;
	val &= ~(GPIO_OTYPER_OT_0 << pin);
	val |= (cfg->type << pin);
	port_ptr->OTYPER = val;

	val = port_ptr->OSPEEDR;
	val &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pin * 2));
	val |= (cfg->speed << (pin * 2));
	port_ptr->OSPEEDR = val;

	val = port_ptr->PUPDR;
	val &= ~(GPIO_PUPDR_PUPDR0 << (pin * 2));
	val |= (cfg->pull << (pin * 2));
	port_ptr->PUPDR = val;

	val = port_ptr->AFR[pin / 8];
	val &= ~(0xf << ((pin % 8) * 4));
	val |= (cfg->alt << ((pin % 8) * 4));
	port_ptr->AFR[pin / 8] = val;
}

/**
 * @brief	Set PIN value
 *
 * @param port_ptr	Pointer to PORT register structure
 * @param pin		Pin number to be configured
 * @param val		Value to be set
 */
void
gpio_pin_set(GPIO_TypeDef *port_ptr, uint32_t pin, uint8_t val)
{
	uint32_t reg;

	/* Set port value */
	reg = port_ptr->ODR;
	reg &= ~(1 << pin);
	reg |= (val << pin);
	port_ptr->ODR = reg;
}

/**
 * @brief	Get PIN value
 *
 * @param port_ptr	Pointer to PORT register structure
 * @param pin		Pin number to be configured
 *
 * @return	Value of specified PIN
 */
uint32_t
gpio_pin_get(GPIO_TypeDef *port_ptr, uint32_t pin)
{
	uint32_t reg;

	/* Get port value */
	reg = port_ptr->IDR;

	return ((reg >> pin) & 0x01);
}
