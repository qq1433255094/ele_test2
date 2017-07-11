#ifndef __IWDG_H__
#define __IWDG_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  

HAL_StatusTypeDef iwdg_init(void);
void reload_iwdg(void);

#ifdef __cplusplus
}
#endif

#endif