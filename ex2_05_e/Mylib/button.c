#include "button.h"

button_HandleTypeDef button1;

void button_init (void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  /* Configure GPIO pin: PA0 (USER) */

  button1.state = NONE;
  button1.temp = NONE;
  button1.read = NONE;

  button1.buff[0] = 0;
  button1.buff[1] = 0;

  button1.port = GPIOA;
  button1.pin = GPIO_PIN_0;

  GPIO_InitStruct.Pin = button1.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(button1.port, &GPIO_InitStruct);

}

uint8_t button_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
  uint8_t val = 0;

  if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET) {
    val =0;
  }
	else val =1;

  return val;
}

//button_type button_state;
//button_type button_temp;
//button_type button_tempstate=NONE;
//uint32_t	button_up_time[2]={0},button_down_time[2]={0};
//uint8_t		button_buff[2];
//time uint 5ms
#define LONG_TIME 100
#define DOUBLE_TIME 30
void button_Getstate(uint32_t time, button_HandleTypeDef *button)
{
	button->buff[0]=button->buff[1];
	button->buff[1]=button_read(button->port,button->pin);
	if(button->buff[0]==0 && button->buff[1]==1)
	{
		button->read = UP;
		button->uptime[0]=button->uptime[1];
		button->uptime[1]=time;
	}
	if(button->buff[0]==1 && button->buff[1]==0)
	{
		button->read = DOWN;
		button->downtime[0]=button->downtime[1];
		button->downtime[1]=time;
	}
	
	if(button->downtime[1]>button->uptime[1] && (button->downtime[1]-button->uptime[1])<= LONG_TIME)
	{
		button->temp=PLUSE;
	}
	if(button->temp==PLUSE && (time-button->downtime[1])>DOUBLE_TIME && button->uptime[1]<button->downtime[1] &&
		(button->uptime[1]-button->downtime[0])>DOUBLE_TIME &&(time-button->downtime[1])<(DOUBLE_TIME+2))
	{
		button->state=PLUSE;
		button->temp=NONE;
	}
		if(button->temp==PLUSE && (time-button->downtime[1])<DOUBLE_TIME && button->uptime[1]>button->downtime[1])
	{
		button->state=DOUBLEPLUSE;
		button->temp=NONE;
	}
		if((time-button->uptime[1])>LONG_TIME && button->uptime[1]>button->downtime[1] &&
			(button->uptime[1]-button->downtime[1])>DOUBLE_TIME)
	{
		button->state= LONGPLUSE;
		button->temp=NONE;
	}
}

void update_buttonstate(uint32_t time)
{
	button_Getstate(time, &button1);
}
	
	
