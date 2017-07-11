#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx_hal.h"

void dac_init(void);
void dac_set(uint32_t data);

#ifdef __cplusplus
}
#endif

#endif





