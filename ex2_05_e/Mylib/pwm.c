#include "pwm.h"

TIM_HandleTypeDef htim1;


HAL_StatusTypeDef pwm_init(void)
{
	HAL_StatusTypeDef state;
	GPIO_InitTypeDef gpio_init;
	TIM_OC_InitTypeDef oc_init;
	
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio_init.Pin = GPIO_PIN_8;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF1_TIM1;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	
	
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = SystemCoreClock / PWM_CLOCK - 1;
	htim1.Init.Period = PWM_Period-1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	
	state=HAL_TIM_PWM_Init(&htim1);
	
	oc_init.OCMode = TIM_OCMODE_PWM1;
	oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_init.OCNPolarity = TIM_OCNPOLARITY_LOW;
	oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	oc_init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	oc_init.Pulse = 0;
	
	HAL_TIM_PWM_ConfigChannel(&htim1, &oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	
	
	return state;
}

void set_pwm_val(uint32_t channel, uint32_t val)
{
	__HAL_TIM_SetCompare(&htim1, channel, (val > PWM_Period ? PWM_Period : val));
}
