/**
 * @file	usart.h
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#include <core/stm32f4xx.h>

typedef enum {
	USART_DEVICE_1,
	USART_DEVICE_2,
	USART_DEVICE_3,
	USART_DEVICE_4,
	USART_DEVICE_5,
	USART_DEVICE_6,
} usart_device_e;

typedef struct {
	usart_device_e	device;
	uint32_t		baudrate;
	USART_TypeDef	*regs;
} usart_cfg_t;

void usart_init(usart_cfg_t *cfg);
void usart_putc(usart_cfg_t *cfg, const uint32_t chr);
uint8_t usart_getc(usart_cfg_t *cfg);


#endif /* USART_H_ */
