#include "led_driver.h"
#include "stm32f4xx.h"
#include "pwm_driver.h"
struct pwm_state
{
	uint32_t counter;
	uint32_t ch0_compare;
	uint32_t ch1_compare;
	uint32_t ch2_compare;
	uint32_t ch3_compare;
	LED_t * ch0;
	LED_t * ch1;
	LED_t * ch2;
	LED_t * ch3;
};

struct pwm_state state;

void pwm_driver_init(LED_t * ch0, LED_t *ch1, LED_t * ch2, LED_t * ch3)
{
	// Start counter at zero
	state.counter = 0;
	// Set all channels to off (i.e. compare value of 0)
	state.ch0_compare = 0;
	state.ch1_compare = 0;
	state.ch2_compare = 0;
	state.ch3_compare = 0;
	// store the pointers for each led driver
	state.ch0 = ch0;
	state.ch1 = ch1;
	state.ch2 = ch2;
	state.ch3 = ch3;
}


void pwm_driver_set(uint8_t channel, uint8_t value)
{
	// bounds check
	if (value > PWM_MAX)
	{
		value = PWM_MAX;
	}
	// update the compare register
	switch (channel)
	{
		case(0):
			state.ch0_compare = value;
			break;
		case(1):
			state.ch1_compare = value;
			break;
		case(2):
			state.ch2_compare = value;
			break;
		case(3):
			state.ch3_compare = value;
			break;
	}
}

void pwm_driver_update(void)
{
	// Update Ch0
	if (state.ch0_compare > state.counter)
	{
		led_on(state.ch0);
	}
	else
	{
		led_off(state.ch0);
	}
	// Update Ch1
	if (state.ch1_compare > state.counter)
	{
		led_on(state.ch1);
	}
	else
	{
		led_off(state.ch1);
	}
	// Update Ch2
	if (state.ch2_compare > state.counter)
	{
		led_on(state.ch2);
	}
	else
	{
		led_off(state.ch2);
	}
	// Update Ch3
	if (state.ch3_compare > state.counter)
	{
		led_on(state.ch3);
	}
	else
	{
		led_off(state.ch3);
	}
	// update counter value
  if (state.counter++ > PWM_MAX)
	{
		state.counter = 0;
	}
	
}
