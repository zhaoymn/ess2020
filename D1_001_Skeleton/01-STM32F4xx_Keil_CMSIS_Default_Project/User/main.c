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
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_usart.h"
//#include "tm_stm32f4_usart.h"
//#include "tm_stm32f4_gpio.h"
/* Include helper library */
#include "ess_helper.h"
#include "stdio.h"

#include "led_driver.h"
#include "delay.h"
#include "pwm_driver.h"
#include "spi_driver.h"
#include "stm32f4xx_adc.h"


//printf support
int itm_debug(int c){
  return(ITM_SendChar(c));
}

int fputc(int ch, FILE *f) {
	return itm_debug(ch);
}

//initiate temperature sensor
void temp_Init(void){
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	//
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStruct);
	// ADC1 Configuration, ADC_Channel_TempSensor is actual channel 16
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1,
	ADC_SampleTime_144Cycles);
	// Enable internal temperature sensor
	ADC_TempSensorVrefintCmd(ENABLE);
	// Enable ADC conversion
	ADC_Cmd(ADC1, ENABLE);
}

uint32_t temp_read(void){
	uint32_t temp_value;
	ADC_SoftwareStartConv(ADC1); //Start the conversion
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Processing the conversion
	temp_value = ADC_GetConversionValue(ADC1); //Return the converted data
	return temp_value;
}


// Initialize Timer 4
void TMR4_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// timer_tick_frequency = 84000000 / (0 + 1) = 84000000
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
	TIM_BaseStruct.TIM_Period = 8399; // <your value here>
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	/* Start count on TIM4 */
	TIM_Cmd(TIM4, ENABLE);
}



// Loops until the timer has expired
void TMR4_WaitForExpiry(void)
{
	// Check the flag. When the timer is expired, the flag is SET.
	while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET)
	{
		//break;
	}
	// Reset flag for next expiry
	TIM_ClearFlag(TIM4, TIM_IT_Update);
}

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
	TIM_BaseStruct.TIM_Period = 8399; // <your value>
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
void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

// Initialize Timer 3 for interrupts
void TMR3Init_ISR(void) {
	// Setup the nested vector interrupt controller
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
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
	TIM_BaseStruct.TIM_Period = 8399; // <your value>
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM3 */
	TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);
	/* TIM Interrupt enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/* Start count on TIM3 */
	TIM_Cmd(TIM3, ENABLE);
}

// This is triggered when the counter overflows
void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}




// Initialize the tilt display using PWM driver
void display_init(void);
// Display a two-dimensional tilt.
// x_tilt varies from -90 deg to +90deg. y_tilt varies from -90 deg to +90deg.
void display_tilt(int8_t x_tilt, int8_t y_tilt);

void display_init(void){
	
}

void display_tilt(int8_t x_tilt, int8_t y_tilt){
	
}

struct acc3
{
	int16_t x;
	int16_t y;
	int16_t z;
};
typedef struct acc3 acc3_t;

int32_t AccInit(void){
	uint8_t who_am_i;
	SPIAcc_Init();
	
	who_am_i = SPIAcc_GetByte(0x0F);
	SPIAcc_SendByte(0x20,0x87);
	if(who_am_i == 0x3F) {
		return 0;
	} else {
		return -1;
	}
}

void AccRead(acc3_t * reading)
{
	uint8_t data_l;
	uint8_t data_h;
	data_l = SPIAcc_GetByte(0x28);
	data_h = SPIAcc_GetByte(0x29);
	reading->x = (data_h << 8) + data_l;
	data_l = SPIAcc_GetByte(0x2A);
	data_h = SPIAcc_GetByte(0x2B);
	reading->y = (data_h << 8) + data_l;
	data_l = SPIAcc_GetByte(0x2C);
	data_h = SPIAcc_GetByte(0x2D);
	reading->z = (data_h << 8) + data_l;
}

float calculate_temp_float(uint32_t temp_value){
	static const float to_V  = 3.3f / 4095;             // max resolution: V per step
	static const float slope = 2.5f / 1000;             // slope in V
	static const float v_cal = 1060.0f * (3.3f / 4095); // 26 deg == 1060
	static const float t_cal = 26;                      // calibration T
		
	float temp = (temp_value * to_V - v_cal) / slope + t_cal;

	return temp;
}

float calculate_temp_integer(uint32_t temp_value){
	
}




int main(void) {
	LED_t led_green;
	LED_t led_orange;
	LED_t led_blue;
	LED_t led_red;
	uint32_t loop_count = 0;
	uint32_t on_time = 0;
	uint8_t datax_l;
	uint8_t datax_h;
	int16_t datax;
	acc3_t reading;
	uint32_t t_value;
	float temp_float;
			uint32_t temp_value;
	uint32_t i;
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	
	
	led_init(&led_green, (uint32_t*)0x40020C14, 12);
	led_init(&led_orange, (uint32_t*)0x40020C14, 13);
	led_init(&led_blue, (uint32_t*)0x40020C14, 15);
	led_init(&led_red, (uint32_t*)0x40020C14, 14);
	
	pwm_driver_init(&led_green, &led_orange, &led_red, &led_blue);
	
	pwm_driver_set(0, 25);
	pwm_driver_set(1, 50);
	pwm_driver_set(2, 75);
	pwm_driver_set(3, 100);
	TMR4_Init_ISR();
	temp_Init();
	AccInit();
	printf("test\n");
	
	// Main loop 
	while (1) {
		/*
		AccRead(&reading);
		if (reading.x<0) {
				pwm_driver_set(2, 0);
			if (reading.x < -10000) {
				pwm_driver_set(0, 100);
			} else {
				pwm_driver_set(0, -reading.x*100/10000);
			}
		} else if(reading.x > 0) {
				pwm_driver_set(0, 0);
			if (reading.x > 10000) {
				pwm_driver_set(2, 100);
			} else {
				pwm_driver_set(2, reading.x*100/10000);
			}
		}
		
		if (reading.y<0) {
				pwm_driver_set(1, 0);
			if (reading.y < -10000) {
				pwm_driver_set(3, 100);
			} else {
				pwm_driver_set(3, -reading.y*100/10000);
			}
		} else if(reading.y > 0) {
				pwm_driver_set(3, 0);
			if (reading.y > 10000) {
				pwm_driver_set(1, 100);
			} else {
				pwm_driver_set(1, reading.y*100/10000);
			}
		}
		*/
		printf("test\n");
ADC_SoftwareStartConv(ADC1); //Start the conversion
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Processing the conversion
temp_value = ADC_GetConversionValue(ADC1); //Return the converted data
		t_value = temp_read();
		temp_float = calculate_temp_float(t_value);
		//TIM4_IRQHandler();
		for(i = 0; i < 10000; i ++) {
			pwm_driver_update();
		}
	}
	
}
