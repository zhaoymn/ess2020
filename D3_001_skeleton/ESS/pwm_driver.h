/**
 *	Four Chabbnnel PWM driver
 *
 *	@author 	Andrew Markham
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 */
#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>
#include "led_driver.h"

// Set the maximum pwm value to 100%
#define PWM_MAX 100

/**
 * Initialize the PWM driver.
 *
 * Pass in the LED struct for each channel.
 *
 * @param ch0 LED_t struct for channel 0
 * @param ch1 LED_t struct for channel 1
 * @param ch2 LED_t struct for channel 2
 * @param ch3 LED_t struct for channel 3
 * @warning This assumes that led_init() has been called for each channel before this function is called.
 *
 * No return
 */
void pwm_driver_init(LED_t * ch0, LED_t *ch1, LED_t * ch2, LED_t * ch3);

/**
 * Set the brightness for a particular channel
 *
 * @param channel Channel number (0, 1, 2 or 3)
 * @param value Percentage brightness (0 to 100)
 * @warning This assumes that pwm_driver_init() has been called for each channel before this function is called.
 *
 * No return
 */
void pwm_driver_set(uint8_t channel, uint8_t value);

/**
 * Update PWM output for each channel
 * This function is called periodically. It checks the status of each pwm channel and decides whether to turn it on or off.
 *
 * @warning This assumes that pwm_driver_init() has been called for each channel before this function is called.
 *
 * No return
 */
void pwm_driver_update(void);



#endif
