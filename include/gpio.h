/**
 * @file	gpio.h
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

typedef enum {
	GPIO_CFG_MODE_INPUT		= 0x0,
	GPIO_CFG_MODE_OUTPUT	= 0x1,
	GPIO_CFG_MODE_ALTERNATE	= 0x2,
	GPIO_CFG_MODE_ANALOG	= 0x3,
} gpio_pin_cfg_mode_e;

typedef enum {
	GPIO_CFG_TYPE_PP		= 0x0,
	GPIO_CFG_TYPE_OD		= 0x1,
} gpio_pin_cfg_type_e;

typedef enum {
	GPIO_CFG_SPEED_2MHZ		= 0x0,
	GPIO_CFG_SPEED_25MHZ	= 0x1,
	GPIO_CFG_SPEED_50MHZ	= 0x2,
	GPIO_CFG_SPEED_100MHZ	= 0x3,
} gpio_pin_cfg_speed_e;

typedef enum {
	GPIO_CFG_PULL_NO		= 0x0,
	GPIO_CFG_PULL_UP		= 0x1,
	GPIO_CFG_PULL_DOWN		= 0x2,
} gpio_pin_cfg_pull_e;

typedef enum {
	GPIO_CFG_ALT_0			= 0x0,
	GPIO_CFG_ALT_1			= 0x1,
	GPIO_CFG_ALT_2			= 0x2,
	GPIO_CFG_ALT_3			= 0x3,
	GPIO_CFG_ALT_4			= 0x4,
	GPIO_CFG_ALT_5			= 0x5,
	GPIO_CFG_ALT_6			= 0x6,
	GPIO_CFG_ALT_7			= 0x7,
	GPIO_CFG_ALT_8			= 0x8,
	GPIO_CFG_ALT_9			= 0x9,
	GPIO_CFG_ALT_10			= 0xa,
	GPIO_CFG_ALT_11			= 0xb,
	GPIO_CFG_ALT_12			= 0xc,
	GPIO_CFG_ALT_13			= 0xd,
	GPIO_CFG_ALT_14			= 0xe,
	GPIO_CFG_ALT_15			= 0xf,
} gpio_pin_cfg_alt_e;

typedef struct {
	gpio_pin_cfg_mode_e		mode;
	gpio_pin_cfg_type_e		type;
	gpio_pin_cfg_speed_e	speed;
	gpio_pin_cfg_pull_e		pull;
	gpio_pin_cfg_alt_e		alt;
} gpio_pin_cfg_t;

void gpio_init(void);
void gpio_pin_cfg(GPIO_TypeDef *port_ptr, uint32_t pin, gpio_pin_cfg_t *cfg);
uint32_t gpio_pin_get(GPIO_TypeDef *port_ptr, uint32_t pin);
void gpio_pin_set(GPIO_TypeDef *port_ptr, uint32_t pin, uint8_t val);

#endif /* GPIO_H_ */
