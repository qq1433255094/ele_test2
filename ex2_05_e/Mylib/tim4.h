#ifndef __TIM4_H_
#define __TIM4_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#define LED_1HZ  50
#define LED_2HZ  25
#define LED_10HZ 5
	 
void tim4_init(void);
void TIM4_Handler(void);

#ifdef __cplusplus
}
#endif

#endif




