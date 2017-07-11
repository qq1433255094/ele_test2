#include "uart4.h"

UART_HandleTypeDef huart4;
uint8_t receive4[10], transmit4[10];

/**
  * @brief 初始化串口4
  * @param none
  * @note none
  * @retval none
  */
void uart4_init(void)
{
	GPIO_InitTypeDef gpio_init;

	HAL_NVIC_SetPriority(UART4_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
	
	__HAL_RCC_UART4_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio_init.Alternate = GPIO_AF8_UART4;
	gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpio_init);
	
	huart4.Init.BaudRate = 115200;
	huart4.Init.WordLength = USART_WORDLENGTH_8B;
	huart4.Init.StopBits = USART_STOPBITS_1;
	huart4.Init.Parity = USART_PARITY_NONE;
	huart4.Init.Mode = USART_MODE_TX_RX;
	huart4.Instance = UART4;
	//receive4[0] = READY;
	HAL_UART_Init(&huart4);
	HAL_UART_Receive_IT(&huart4, receive4, 1);
}

/**
  * @brief 串口4发送字符串
  * @param *b：字符串数组指针
  * @note none
  * @retval none
  */
void uart4_send(const char *b)
{
	char len = strlen(b);
	HAL_UART_Transmit(&huart4, (uint8_t *)b, len, len*2);
}

void UART4_IRQHandler(void)
{
	huart4.pRxBuffPtr = receive4;
	HAL_UART_IRQHandler(&huart4);
}

void UART4_Handler(void)
{

	HAL_UART_Receive_IT(&huart4, huart4.pRxBuffPtr, 1);
}

/////////////////////////////////////////////