#include "dac.h"

DAC_HandleTypeDef hdac;

void dac_init(void)
{	
	GPIO_InitTypeDef gpio_init;
	DAC_ChannelConfTypeDef channel;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_DAC_CLK_ENABLE();
		
	gpio_init.Pin=GPIO_PIN_4;
	gpio_init.Mode=GPIO_MODE_ANALOG;
	gpio_init.Speed=GPIO_SPEED_FREQ_HIGH;
	gpio_init.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	hdac.Instance=DAC;
	HAL_DAC_Init(&hdac);
	
	channel.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;
	channel.DAC_Trigger=DAC_TRIGGER_SOFTWARE;
	HAL_DAC_ConfigChannel(&hdac,&channel,DAC_CHANNEL_1);
}

void dac_set(uint32_t data)
{
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,data);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
}


