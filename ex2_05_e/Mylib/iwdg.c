#include "iwdg.h"

static IWDG_HandleTypeDef hiwdg;

HAL_StatusTypeDef iwdg_init(void)
{
	hiwdg.Init.Prescaler = 0;
	hiwdg.Init.Reload = 3200; //1s
	hiwdg.Instance = IWDG;
	
	return HAL_IWDG_Init(&hiwdg);
}

void reload_iwdg(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
