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
#include <string.h>
#include <core/stm32f4xx.h>
#include <config.h>
#include <sysclk.h>
#include <gpio.h>
#include <usart.h>
#include <sys/stat.h>

#define MAX_MSG_LEN	32
#define IS_ALPHANUMERIC(x)	((((x) >= 'a') && ((x) <= 'z')) ||				\
		(((x) >= 'A') && ((x) <= 'Z')) ||									\
		(((x) >= '0') && ((x) <= '9')) || ((x) == ' ') || ((x) == '\t'))

static volatile uint32_t	g_red_timeout = 1000;
static volatile uint32_t	g_green_timeout = 900;
static usart_cfg_t			usartcfg;
static uint64_t				g_ticks;

void
EXCV_SysTick(void)
{
	g_ticks++;
}

void
IRQ_USART2(void)
{
	static char msg[MAX_MSG_LEN];
	static uint8_t length;
	char t = usart_getc(&usartcfg);
	char cmd[32];
	uint32_t val = 0;

	if (IS_ALPHANUMERIC(t)) {
		if (length < (MAX_MSG_LEN - 1))
			msg[length++] = t;
	}

	usart_putc(&usartcfg, t);

	if (t == '\r') {
		usart_putc(&usartcfg, '\n');

		sscanf(msg, "%s %lu", cmd, &val);

		if ((strcmp(cmd, "red") == 0) && (val != 0)) {
			g_red_timeout = val;
			printf("OK\n");
		} else if ((strcmp(cmd, "green") == 0) && (val != 0)) {
			g_green_timeout = val;
			printf("OK\n");
		} else {
			printf("ERROR\n");
		}

		memset(msg, 0, sizeof(msg));
		length = 0;
	}
}

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
		/* Force sending CR with new line */
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
configure_peripherials(void)
{
	gpio_pin_cfg_t	pincfg;

	gpio_init();

	usartcfg.device = USART_DEVICE_2;
	usartcfg.baudrate = 115200;
	usart_init(&usartcfg);

	pincfg.mode = GPIO_CFG_MODE_OUTPUT;
	pincfg.speed = GPIO_CFG_SPEED_25MHZ;
	pincfg.type = GPIO_CFG_TYPE_PP;
	gpio_pin_cfg(LED_GPIO, LED_pin_red, &pincfg);
	gpio_pin_cfg(LED_GPIO, LED_pin_green, &pincfg);
}

void
process(void)
{
	uint8_t red_state = 0, green_state = 0;
	uint64_t green_last = 0, red_last = 0;

	while (1) {
		if (g_ticks - green_last > g_green_timeout) {
			green_last = g_ticks;
			gpio_pin_set(LED_GPIO, LED_pin_green, green_state);
			green_state = 1 - green_state;
		}
		if (g_ticks - red_last > g_red_timeout) {
			red_last = g_ticks;
			gpio_pin_set(LED_GPIO, LED_pin_red, red_state);
			red_state = 1 - red_state;
		}
	}
}

/**
 * @brief Main program entry
 */
int
main(void)
{

	/* Configure system peripherals */
	configure_peripherials();

	/* Configure system tick */
	SysTick_Config(FREQUENCY/1000);

	/* Be nice, welcome! */
	printf("\nSTM32F4 discovery: Welcome!\n");

	process();

	/* Should not go here... */
	for (;;);

	return (0);
}

