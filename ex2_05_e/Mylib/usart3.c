#include "usart3.h"
#include "arm_math.h"
#include "can2.h"

UART_HandleTypeDef huart3;
uint8_t receive3[10], transmit3[10];

struct ROLL roll;
short buffer[7];



/**
* @brief 初始化串口3
* @param none
* @note none
* @retval none
*/
void usart3_init(void)
{
	GPIO_InitTypeDef gpio_init;

	roll.offset = 0;
	roll.len = 14;
	roll.p = (char *)buffer;

	HAL_NVIC_SetPriority(USART3_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_init.Alternate = GPIO_AF7_USART3;
	gpio_init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &gpio_init);

	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = USART_WORDLENGTH_8B;
	huart3.Init.StopBits = USART_STOPBITS_1;
	huart3.Init.Parity = USART_PARITY_NONE;
	huart3.Init.Mode = USART_MODE_TX_RX;
	huart3.Instance = USART3;

	HAL_UART_Init(&huart3);
	HAL_UART_Receive_IT(&huart3, receive3, 1);
}

/**
* @brief 串口3发送字符串
* @param *b：字符串数组指针
* @note none
* @retval none
*/

void Insert(struct ROLL *p, char num)
{
	p->p[p->offset++] = num;
	if (p->offset == p->len)
	{
		p->offset = 0;
	}
}

short Read(struct ROLL *p, char num)
{
	char shift = p->offset;
	shift += num;

	if (shift >= p->len)
	{
		shift -= p->len;
	}

	return p->p[shift];
}

void usart3_send(const char *b)
{
	char len = strlen(b);
	HAL_UART_Transmit(&huart3, (uint8_t *)b, len, len * 2);
}

void USART3_IRQHandler(void)
{
	huart3.pRxBuffPtr = receive3;
	HAL_UART_IRQHandler(&huart3);
}



void UART3_Handler(void)
{
	uint8_t have_receive = 0;

	Insert(&roll, receive3[0]);

	HAL_UART_Receive_IT(&huart3, huart3.pRxBuffPtr, 1);
}


