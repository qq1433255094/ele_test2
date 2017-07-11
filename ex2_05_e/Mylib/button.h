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

void button_init (void);
uint8_t button_read(void);
void button_Getstate(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif



