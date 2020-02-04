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
/* Include helper library */
#include "ess_helper.h"
/* LED library */
#include "led_driver.h"
/* Loop delays */
#include "loop_delay.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	// Main loop 
	while (1) {
		
	}
}
