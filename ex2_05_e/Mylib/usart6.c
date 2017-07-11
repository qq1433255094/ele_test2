
#include "usart6.h"


UART_HandleTypeDef huart6;
uint8_t 		receive6[20], transmit6[20];




/**
* @brief 初始化串口6
* @param none
* @note none
* @retval none
*/
void usart6_init(void)
{
	GPIO_InitTypeDef gpio_init;

	HAL_NVIC_SetPriority(USART6_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

	__HAL_RCC_USART6_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	gpio_init.Alternate = GPIO_AF8_UART4;
	gpio_init.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init.Mode		= GPIO_MODE_AF_PP;
	gpio_init.Speed 	= GPIO_SPEED_FREQ_HIGH;
	gpio_init.Pull		= GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &gpio_init);

	huart6.Init.BaudRate = 9600;
	huart6.Init.WordLength = USART_WORDLENGTH_8B;
	huart6.Init.StopBits = USART_STOPBITS_1;
	huart6.Init.Parity	= USART_PARITY_NONE;
	huart6.Init.Mode	= USART_MODE_TX_RX;
	huart6.Instance 	= USART6;

	HAL_UART_Init(&huart6);

	//HAL_UART_Receive_IT(&huart6, receive6, 8);
}


/**
* @brief 串口6发送字符串
* @param *b：字符串数组指针
* @note none
* @retval none
*/
void usart6_send(const char * b)
{
	char			len = strlen(b);

	HAL_UART_Transmit(&huart6, (uint8_t *) b, len, len * 2);
}



void USART6_IRQHandler(void)
{
	//huart6.pRxBuffPtr = receive6;
	HAL_UART_IRQHandler(&huart6);
}






void UART6_Handler(void)
{

}


