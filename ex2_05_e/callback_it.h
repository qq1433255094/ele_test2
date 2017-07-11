#pragma once
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif 

	/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

	/* Exported types ------------------------------------------------------------*/
	/* Exported constants --------------------------------------------------------*/
	/* Exported macro ------------------------------------------------------------*/
	/* Exported functions ------------------------------------------------------- */

	void NMI_Handler(void);
	void HardFault_Handler(void);
	void MemManage_Handler(void);
	void BusFault_Handler(void);
	void UsageFault_Handler(void);
	void SVC_Handler(void);
	void DebugMon_Handler(void);
	void PendSV_Handler(void);
	void SysTick_Handler(void);
	void TIM3_IRQHandler(void);
	void TIM4_IRQHandler(void);
	void CAN2_RX0_IRQHandler()
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */