#include "button.h"

void button_init (void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();

  /* Configure GPIO pin: PA0 (USER) */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

uint8_t button_read(void) {
  uint8_t val = 0;

  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET) {
    val =0;
  }
	else val =1;

  return val;
}

button_type button_state;
button_type button_temp;
button_type button_tempstate=NONE;
uint32_t	button_up_time[2]={0},button_down_time[2]={0};
uint8_t		button_buff[2];

void button_Getstate(uint32_t time)
{
	button_buff[0]=button_buff[1];
	button_buff[1]=button_read();
	if(button_buff[0]==0 && button_buff[1]==1)
	{
		button_tempstate = UP;
		button_up_time[0]=button_up_time[1];
		button_up_time[1]=time;
	}
	if(button_buff[0]==1 && button_buff[1]==0)
	{
		button_tempstate = DOWN;
		button_down_time[0]=button_down_time[1];
		button_down_time[1]=time;
	}
	
	if(button_down_time[1]>button_up_time[1] && (button_down_time[1]-button_up_time[1])<=100)
	{
		button_temp=PLUSE;
	}
	if(button_temp==PLUSE && (time-button_down_time[1])>30 && button_up_time[1]<button_down_time[1] &&
		(button_up_time[1]-button_down_time[0])>30 &&(time-button_down_time[1])<32)
	{
		button_state=PLUSE;
		button_temp=NONE;
	}
		if(button_temp==PLUSE && (time-button_down_time[1])<30 && button_up_time[1]>button_down_time[1])
	{
		button_state=DOUBLE;
		button_temp=NONE;
	}
		if((time-button_up_time[1])>100 && button_up_time[1]>button_down_time[1] && 
			(button_up_time[1]-button_down_time[1])>30)
	{
		button_state=LONG;
		button_temp=NONE;
	}
}
	
	
