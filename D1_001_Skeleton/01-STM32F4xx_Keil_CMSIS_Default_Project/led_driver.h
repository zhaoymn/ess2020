#ifndef LED_DRIVER_H
#define LED_DRIVER_H
/* Include core modules */
#include "stm32f4xx.h"
struct LEDstruct
{
	volatile uint32_t *port;
	uint32_t pin;
};

typedef struct LEDstruct LED_t;
void led_init(LED_t *led, volatile uint32_t *port, uint32_t pin);
void led_on(LED_t *led);
void led_off(LED_t *led);
#endif
