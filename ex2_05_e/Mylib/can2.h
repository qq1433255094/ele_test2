#ifndef __CAN2_H__
#define __CAN2_H__

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  
#include "os_task.h"

#define CAN2_FREQ 1000000
#define CAN2_Prescaler (SystemCoreClock / 4 / CAN2_FREQ)

#define USE_DOUBLE_PLACE 1
#define USE_DEFAULT_SPEED 0
#define USE_LOCAL_PLACE 0

#if (USE_DOUBLE_PLACE ==1 )
	#define PLACE_TYPE double
#else
	#define PLACE_TYPE float
#endif // USE_DOUBLE_PLACE==1



#define CH_X 0
#define CH_Y 1
#define CH_A 2
#define RC_X RC_ch1
#define RC_Y RC_ch2
#define RC_A RC_ch3


#define ABS 0
#define INC 1

	HAL_StatusTypeDef can2_init(void);
	HAL_StatusTypeDef can2_send(uint8_t *msg, uint8_t len, uint8_t id);
	void CalPosition(short speedset[], short *accdata);
	HAL_CAN_StateTypeDef CAN2_GetState(void);
	void CAN2_Send(short *ch);
	void CAN2_RX0_Handler();

	void Lock_Direction(PLACE_TYPE angleset);
	void UnLock_Direction();
	void Car_Move_Place_To(uint8_t ch, int val, short speed_set, uint8_t mode);
	void Car_Move_Position_To(uint8_t ch, int val, short speed_set, uint8_t mode);
	void move_to(int x, int y, int f, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif