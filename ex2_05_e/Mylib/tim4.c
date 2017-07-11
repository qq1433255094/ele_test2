#include "tim4.h"
#include "stm32f4xx_hal.h"
#include "can2.h"

extern short speedset[4];
extern int Position_real[3];
extern short accdata[4];


TIM_HandleTypeDef htim4;
//TIM_OC_InitTypeDef tim4_oc;

void tim4_init(void)
{
  uint16_t prescalervalue = 0;
  uint32_t tmpvalue = 0;

  /* TIM4 clock enable */
  __HAL_RCC_TIM4_CLK_ENABLE();

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_SetPriority(TIM4_IRQn, 6, 0);  
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  
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
  prescalervalue = (uint16_t) ((tmpvalue * 2) / 1000000) - 1;
  
  /* Time base configuration */
  htim4.Instance = TIM4;
  htim4.Init.Period = 20000;
  htim4.Init.Prescaler = prescalervalue;
  htim4.Init.ClockDivision = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;

  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
	  /* Initialization Error */
	  //Error_Handler();
  }

  if (HAL_TIM_Base_Start_IT(&htim4) != HAL_OK)
  {
	  /* Start Error */
	 // Error_Handler();
  }
}

void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim4);
}

uint32_t tim4_cnt = 0;

void  TIM4_Handler(void)
{
	tim4_cnt++;
}
