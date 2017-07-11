#pragma once
#ifndef __USART6_H__
#define __USART6_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include <string.h>
#include <stdarg.h>


	void usart6_init(void);
	void usart6_send(const char *b);
	void UART6_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
