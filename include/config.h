/** \file config.h
 * \brief Basic configuration of the project
 * \author Freddie Chopin, http://www.freddiechopin.info/
 * \date 2012-03-17
 */

/******************************************************************************
* project: stm32f4_blink_led
* chip: STM32F407VG
* compiler: arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors) 4.6.2
* 	20110921 (release) [ARM/embedded-4_6-branch revision 182083]
******************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

/*
+=============================================================================+
| global definitions
+=============================================================================+
*/

#define CRYSTAL				8000000ul	///< quartz crystal resonator which is connected to the chip
#define FREQUENCY			168000000ul	///< desired target frequency of the core

//f(VCO clock) = f(PLL clock input) � (PLLN / PLLM)
//� f(PLL general clock output) = f(VCO clock) / PLLP
//� f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
#define PLL_PLLN			84
#define PLL_PLLM			2
#define PLL_PLLP			2
#define PLL_PLLQ			7
#define MAX_FLASH_FREQ		30000000UL // calculate wait_states (30M is valid for 2.7V to 3.6V voltage range, use 24M for 2.4V to 2.7V, 18M for 2.1V to 2.4V or 16M for  1.8V to 2.1V)

#define LED_GPIO							GPIOD		///< GPIO port to which the LED is connected
#define LED_pin_red								13			///< pin number of the LED
#define LED_pin_green							12			///< pin number of the LED


#endif /* CONFIG_H_ */
