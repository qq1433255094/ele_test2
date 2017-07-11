#ifndef __AD_H__
#define __AD_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx_hal.h"

void adc_init(void);
uint32_t adc_get(void);

#ifdef __cplusplus
}
#endif

#endif



