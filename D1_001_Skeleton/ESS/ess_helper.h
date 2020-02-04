/**
 *	Helper library for STM32F4 Discovery board
 *
 *	@author 	Andrew Markham
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 */
#ifndef ESS_HELPER_H
#define ESS_HELPER_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

/**
 * This function initializes GPIO pins to defaults and enables clock sources
 * This function also enables clock for GPIO
 *
 * No return
 */
void ess_helper_init(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
