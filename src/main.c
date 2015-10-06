/**
 * @file	main.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/stm32f4xx.h>
#include <config.h>
#include <sysclk.h>
#include <gpio.h>
#include <usart.h>
#include <sys/stat.h>
#include <FreeRTOS.h>
#include <task.h>

static usart_cfg_t		usartcfg;

/**
 * @brief	Syscall for console write
 *
 * Used as an entry for newlib _write syscall. It automatically
 * adds '\r' along with each '\n'. By default, put all
 * characters to configured USART port.
 *
 * @param file	File descriptor (unused)
 * @param ptr	Message pointer
 * @param len	Length of the buffer
 *
 * @return Number of characters successfully printed.
 */
int
_write(int file, char *ptr, int len)
{
	int todo;

	if (file) {};

	for (todo = 0; todo < len; todo++) {
		usart_putc(&usartcfg, *ptr);
		/* Froce sending CR with new line */
		if (*ptr == '\n')
			usart_putc(&usartcfg, '\r');
		ptr++;
	}
	return len;
}

/**
 * @brief Configure peripherals
 *
 * Use configuration in separate function rather than in main
 * to save stack space for one-time-used configuration variables.
 * Configure: USART and GPIO
 */
static void
configure_peripherals(void)
{
	gpio_pin_cfg_t	pincfg;

	gpio_init();

	usartcfg.device = USART_DEVICE_2;
	usartcfg.baudrate = 115200;

	usart_init(&usartcfg);

	pincfg.mode = GPIO_CFG_MODE_OUTPUT;
	pincfg.speed = GPIO_CFG_SPEED_25MHZ;
	pincfg.type = GPIO_CFG_TYPE_PP;
	gpio_pin_cfg(LED_GPIO, LED_pin, &pincfg);
}

static void
task1(void* unused)
{

	/* Blink LED and print dot mark each iteration */
	while (1) {
		sysclk_delay(10000000);
		gpio_pin_set(LED_GPIO, LED_pin, 1);
		sysclk_delay(10000000);
		gpio_pin_set(LED_GPIO, LED_pin, 0);
		usart_putc(&usartcfg, '.');
	}
}

/**
 * @brief Main program entry
 */
int
main(void)
{

	/* Configure system peripherals */
	configure_peripherals();

	/* Be nice, welcome! */
	printf("\nSTM32F4 discovery: Welcome!\n");

	/* Create the 'echo' task, which is also defined within this file. */
	xTaskCreate(task1, "Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Should not get here... */
	return (0);
}

void
vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	(void) pxTask;
	(void) pcTaskName;

	for( ;; );
}

