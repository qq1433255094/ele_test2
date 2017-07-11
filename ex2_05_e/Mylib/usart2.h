#ifndef __USART2_H__
#define __USART2_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*#include <stdarg.h>*/
#define USE_PRINTF

void usart2_init(void);
void usart2_send(char *b);
void usart2_ptint(char *str, int data, int mode);
void usart2_ptintf(char *str, float data, int len);
void _printf(char *fmt, ...);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
