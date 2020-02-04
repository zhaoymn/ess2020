#include "ess_helper.h"

// internal function to initialize leds and clocks
void _init_leds(void)
{
	// These are the defines for the LED and button pinouts for this particular board
	#define LED_GREEN					GPIO_Pin_12
	#define LED_ORANGE				GPIO_Pin_13
	#define LED_RED						GPIO_Pin_14
	#define LED_BLUE					GPIO_Pin_15
	#define LED_ALL						LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE
	
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOD
	#define TM_DISCO_LED_PORT			GPIOD
	#define TM_DISCO_LED_PINS			LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE
	
	// Typedef to initialize GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable GPIO peripheral clock for leds */
	RCC_AHB1PeriphClockCmd(TM_DISCO_LED_RCC, ENABLE);

	/**
	 * Pins set to PD12, PD13, PD14, PD15
	 * Output type is push-pull
	 * Mode is output
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitStruct.GPIO_Pin = TM_DISCO_LED_PINS;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Initialize pins */
	GPIO_Init(TM_DISCO_LED_PORT, &GPIO_InitStruct);
}

void _init_button(void)
{
	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_BUTTON_PORT		GPIOA
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_0
	#define TM_DISCO_BUTTON_PRESSED		Bit_SET
	
	// Typedef to initialize GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable GPIO peripheral clock for button */
	RCC_AHB1PeriphClockCmd(TM_DISCO_BUTTON_RCC, ENABLE);

	/**
	 * Pin set to PA0
	 * Output type is push-pull
	 * Mode is Input
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitStruct.GPIO_Pin = TM_DISCO_BUTTON_PIN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	/* Initialize pin */
	GPIO_Init(TM_DISCO_BUTTON_PORT, &GPIO_InitStruct);
}



void ess_helper_init(void)
{
	_init_leds();
	_init_button();
}


