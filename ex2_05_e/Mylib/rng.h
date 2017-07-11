#pragma once
#ifndef __RNG_H__
#define __RNG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

	void rng_init(void);
	uint8_t get_rng_u8(void);
	uint32_t get_rng_u32(void);

#ifdef __cplusplus
}
#endif

#endif
