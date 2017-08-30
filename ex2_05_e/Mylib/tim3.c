#include "tim3.h"
#include "stm32f4xx_hal.h"
#include "button.h"
#include "tim4.h"

TIM_HandleTypeDef htim3;

void tim3_init(void)
{
  uint16_t prescalervalue = 0;
  uint32_t tmpvalue = 0;

  /* TIM4 clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_SetPriority(TIM3_IRQn, 6, 0);  
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  
  /* -----------------------------------------------------------------------
  TIM4 Configuration: Output Compare Timing Mode:  
    To get TIM4 counter clock at 550 KHz, the prescaler is computed as follows:
    Prescaler = (TIM4CLK / TIM4 counter clock) - 1
    Prescaler = ((f(APB1) * 2) /550 KHz) - 1
  
    CC update rate = TIM4 counter clock / CCR_Val = 32.687 Hz
    ==> Toggling frequency = 16.343 Hz  
  ----------------------------------------------------------------------- */
  
  /* Compute the prescaler value */
  tmpvalue = HAL_RCC_GetPCLK1Freq();
  prescalervalue = (uint16_t) ((tmpvalue * 2) / 1000000) - 1; //1Mhz
  
  /* Time base configuration */
  htim3.Instance = TIM3;
  htim3.Init.Period = 5000-1; //5ms
  htim3.Init.Prescaler = prescalervalue;
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    /* Initialization Error */
 //   Error_Handler();
  }
  
  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
  {
    /* Start Error */
 //   Error_Handler();
  }
}

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}

uint32_t tim3_cnt=0,tim3_freq=LED_1HZ,tim3_cnt_button=0;

uint8_t tim3_state = 1;

void  TIM3_Handler(void)
{
	/*if (tim3_state == 1)
	{
		tim3_state = 2;
		TIM3->ARR = 10000 - 1;
	}
	else if (tim3_state == 2)
	{
		tim3_state = 1;
		TIM3->ARR = 5000 - 1;
	}*/

	tim3_cnt++;
	tim3_cnt_button++;
	update_buttonstate(tim3_cnt_button);
}


