#pragma once
#ifndef __MYMATH_H__
#define __MYMATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#define	abs(x) (x =(x > 0) ? x:-x)
	typedef struct
	{
		float x_last;
		float p_last;
		float Q;
		float R;
		float kg;
		float x_mid;
		float x_now;
		float p_mid;
		float p_now;
	} Kalman;

	float Kalman_filter(Kalman* data, float measure);
	float Q_rsqrt(float number);


#ifdef __cplusplus
}
#endif

#endif
