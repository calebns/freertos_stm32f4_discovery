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
#include <queue.h>

static usart_cfg_t		usartcfg;

#define MAX_MSG_LEN	32

#define IS_ALPHANUMERIC(x)	((((x) >= 'a') && ((x) <= 'z')) ||				\
		(((x) >= 'A') && ((x) <= 'Z')) ||									\
		(((x) >= '0') && ((x) <= '9')) || ((x) == ' ') || ((x) == '\t'))

struct uart_msg
{
	char length;
	char msg[MAX_MSG_LEN];
};

QueueHandle_t uart_msg_queue;

static volatile uint32_t g_red_timeout = 1000;
static volatile uint32_t g_green_timeout = 900;


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

void
IRQ_USART2(void)
{
	static struct uart_msg msg;
	char t = usart_getc(&usartcfg);

	if (IS_ALPHANUMERIC(t)) {
		if (msg.length < (MAX_MSG_LEN - 1))
			msg.msg[msg.length++] = t;
	}

	usart_putc(&usartcfg, t);

	if (t == '\r') {
		usart_putc(&usartcfg, '\n');

		if (uart_msg_queue)
			xQueueSend(uart_msg_queue, &msg, 0);
		memset(&msg, 0, sizeof(msg));
	}
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
	gpio_pin_cfg(LED_GPIO, LED_pin_red, &pincfg);
	gpio_pin_cfg(LED_GPIO, LED_pin_green, &pincfg);
}

static void
task_uart(void* unused)
{
	static struct uart_msg msg;
	BaseType_t ret;
	char cmd[16];
	uint32_t val;

	/* Parse commands */
	while (1) {
		val = 0;
		memset(&msg, 0, sizeof(msg));
		ret = xQueueReceive(uart_msg_queue, &msg, portMAX_DELAY);
		if (ret == pdTRUE) {
			sscanf(msg.msg, "%s %d", cmd, &val);

			if ((strcmp(cmd, "red") == 0) && (val != 0)) {
				g_red_timeout = val;
				printf("OK\n");
			} else if ((strcmp(cmd, "green") == 0) && (val != 0)) {
				g_green_timeout = val;
				printf("OK\n");
			} else {
				printf("ERROR\n");
			}
		}
	}
}

static void
task_red(void* unused)
{
	/* Blink LED */
	while (1) {
		vTaskDelay(g_red_timeout);
		gpio_pin_set(LED_GPIO, LED_pin_red, 1);
		vTaskDelay(g_red_timeout);
		gpio_pin_set(LED_GPIO, LED_pin_red, 0);
	}
}

static void
task_green(void* unused)
{
	/* Blink LED */
	while (1) {
		vTaskDelay(g_green_timeout);
		gpio_pin_set(LED_GPIO, LED_pin_green, 1);
		vTaskDelay(g_green_timeout);
		gpio_pin_set(LED_GPIO, LED_pin_green, 0);
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

	/* Create UART queue */
	uart_msg_queue = xQueueCreate(8, sizeof(struct uart_msg));
	if (uart_msg_queue == NULL) {
		printf("ERROR: failed to initialize queue\n");
		return (-1);
	}

	/* Create the 'echo' task, which is also defined within this file. */
	xTaskCreate(task_uart, "UART", 1024, NULL, 16, NULL);
	xTaskCreate(task_red, "LED: RED", configMINIMAL_STACK_SIZE, NULL, 250, NULL);
	xTaskCreate(task_green, "LED: GREEN", configMINIMAL_STACK_SIZE, NULL, 250, NULL);

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

