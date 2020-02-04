#include "led_driver.h"

void led_on(LED_t *led){
	*led->port |= 0x1 << (led->pin);
}
void led_off(LED_t *led){
	*led->port &= ~(0x1 << led->pin);
}
void led_init(LED_t *led, volatile uint32_t *port, uint32_t pin){
	led->port = port;
	led->pin = pin;
	led_off(led);
}
