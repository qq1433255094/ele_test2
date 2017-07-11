#include "ad.h"

#define ADC4                ((ADC_TypeDef *) (ADC1_BASE+1))

ADC_HandleTypeDef hadc;

void adc_init(void)
{
	
	ADC_ChannelConfTypeDef channel;
	GPIO_InitTypeDef gpio_init;
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio_init.Pin=GPIO_PIN_0;
	gpio_init.Mode=GPIO_MODE_ANALOG;
	gpio_init.Speed=GPIO_SPEED_FREQ_LOW;
	gpio_init.Pull=GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	hadc.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution=ADC_RESOLUTION_12B;
	hadc.Init.ScanConvMode=DISABLE;
	hadc.Init.ContinuousConvMode=ENABLE;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;	
	hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;				
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DataAlign=ADC_DATAALIGN_RIGHT;
	
	hadc.Instance=ADC4;	
	HAL_ADC_Init(&hadc);
	
	channel.Channel=ADC_CHANNEL_0;
	HAL_ADC_ConfigChannel(&hadc,&channel);
	
	HAL_ADC_Start(&hadc);
}

uint32_t adc_get(void)
{
		while(HAL_ADC_PollForConversion(&hadc,2));
		return HAL_ADC_GetValue(&hadc);
}



