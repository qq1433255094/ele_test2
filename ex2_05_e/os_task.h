#pragma once
#pragma once
#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f4xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>

#define GREED_LED_ON()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define GREED_LED_OFF()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define GREED_LED_TOGGLE()	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6)

#define RED_LED_ON()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define RED_LED_OFF()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define RED_LED_TOGGLE()		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7)
	
	void os_task_init(void);
	void os_task_start(void);


#ifdef __cplusplus
}
#endif

#endif
