#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>
#include "led_driver.h"
#include "delay.h"

#define PWM_MAX 100
void pwm_driver_init(LED_t * ch0, LED_t *ch1, LED_t * ch2, LED_t * ch3);
void pwm_driver_set(uint8_t channel, uint8_t value);
void pwm_driver_update(void);



#endif
