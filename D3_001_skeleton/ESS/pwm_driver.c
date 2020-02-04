#include "pwm_driver.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
// Hardware driver integrated with Timer4 ISR

// Initialize Timer 4 for interrupts
void TMR4_Init_ISR(void) {
	// Setup the nested vector interrupt controller
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/*	
		timer_tick_frequency = 84000000 / (0 + 1) = 84000000 
	*/	
		TIM_BaseStruct.TIM_Prescaler = 0;
		/* Count up */
		TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		Set timer period when it must reset
		First you have to know max value for timer
		In our case it is 16bit = 65535
		Frequency = timer_tick_frequency / (TIM_Period + 1)
		If you get TIM_Period larger than max timer value (in our case 65535),
		you have to choose larger prescaler and slow down timer tick frequency
	*/
    TIM_BaseStruct.TIM_Period = 8399; 
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
		/* TIM Interrupt enable */
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	/* Start count on TIM4 */
    TIM_Cmd(TIM4, ENABLE);
}

// This is triggered when the counter overflows
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		pwm_driver_update();
	}
}



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
	// Start the timer
	TMR4_Init_ISR();
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
