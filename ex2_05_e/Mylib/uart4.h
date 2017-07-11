#pragma once
#ifndef __UART4_H__
#define __UART4_H__

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include <string.h>
#include <stdarg.h>

	void uart4_init(void);
	void uart4_send(const char *b);
	void UART4_Handler(void);



	 
#ifdef __cplusplus
}
#endif
	 
#endif
