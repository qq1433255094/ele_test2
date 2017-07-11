#ifndef __CAN1_H__
#define __CAN1_H__

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  

#define CAN1_FREQ 1000000
#define CAN1_Prescaler (SystemCoreClock / 4 / CAN1_FREQ)

	typedef struct 
	{
		short id;
		short Iset;
		short Ireal;
		char hall;
		short angle;
		short speed;
		short count;
		int position;
	}PTZmoto_TypeDef;

HAL_StatusTypeDef can1_init(void);
void CAN1_RX0_Handler();

#ifdef __cplusplus
}
#endif

#endif