#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  


#define PWM_freq	8000
#define PWM_Period	5000
//#define PWM_CLOCK 1000000
#define PWM_CLOCK (PWM_freq*PWM_Period)
	
HAL_StatusTypeDef pwm_init(void);
void set_pwm_val(uint32_t channel, uint32_t val);
	
#ifdef __cplusplus
}
#endif

#endif
