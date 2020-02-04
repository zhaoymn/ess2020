/**
 *	STM32F4 Discovery Skeleton Project
 *
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
/* Include helper library */
#include "ess_helper.h"
// led driver
#include "led_driver.h"
// loop delay
#include "loop_delay.h"
// pwm driver
#include "pwm_driver.h"
// spi driver
#include "spi_driver.h"
// Use a define for the address of the PORTD output register
#define PORTD   ((volatile uint32_t*)0x40020C14)

LED_t led_green;
LED_t led_orange;
LED_t led_blue;
LED_t led_red;

int main(void) {
	uint8_t ch0 = 0;
	uint8_t ch1 = PWM_MAX;
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	// Set up the leds
	led_init(&led_green,PORTD,12);
	led_init(&led_orange,PORTD,13);
	led_init(&led_red,PORTD,14);
	led_init(&led_blue,PORTD,15);
	// set up the pwm driver
	pwm_driver_init(&led_green, &led_orange, &led_red, &led_blue);
	pwm_driver_set(0,40);
	while (1) {
			
	}
}
