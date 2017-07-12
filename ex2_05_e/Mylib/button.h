#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx_hal.h"

typedef enum
{
	NONE=-1,
	UP=0,
	DOWN=1,
	PLUSE=2,
	LONG=3,
	DOUBLE=4
}button_type;

typedef struct
{
	button_type state;
	button_type temp;
	button_type read;
	uint32_t	uptime[2], downtime[2];
	uint8_t		buff[2];
	GPIO_TypeDef *port;
	uint16_t pin;
}button_HandleTypeDef;

void button_init (void);
uint8_t button_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void button_Getstate(uint32_t time, button_HandleTypeDef *button);
void update_buttonstate(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif



