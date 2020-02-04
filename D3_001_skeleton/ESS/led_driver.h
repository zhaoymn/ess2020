/**
 *	LED Driver
 *
 *	@author 	Andrew Markham
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 */
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

// Generic struct to encapsulate LED state. 
struct LEDstruct
{
	volatile uint32_t * port; // the LED register
	uint32_t pin;  // 0..15
};
// Typedef for an LED struct - this is a shorthand
// that defines a new type for encapsulating LED state
typedef struct LEDstruct LED_t;
	
// Function Declarations
void led_init(LED_t * led, volatile uint32_t * port, uint32_t pin);
void led_on(LED_t * led);
void led_off(LED_t * led);

#endif

