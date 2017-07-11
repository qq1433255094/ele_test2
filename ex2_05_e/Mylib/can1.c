#include "can1.h"
#include "arm_math.h"

CAN_HandleTypeDef hcan1;
CanTxMsgTypeDef   can1_TxMsg;
CanRxMsgTypeDef   can1_pRxMsg;
PTZmoto_TypeDef	  PTZmoto[3];

HAL_StatusTypeDef can1_init(void)
{
	HAL_StatusTypeDef state;
	GPIO_InitTypeDef gpio_init;
	CAN_FilterConfTypeDef can1_filter;

	__HAL_RCC_CAN1_CLK_ENABLE();
	//__HAL_RCC_CAN2_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();

	gpio_init.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &gpio_init);

	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

	hcan1.Instance = CAN1;
	hcan1.Init.TTCM = DISABLE;
	hcan1.Init.ABOM = DISABLE;
	hcan1.Init.AWUM = DISABLE;
	hcan1.Init.NART = DISABLE;
	hcan1.Init.RFLM = DISABLE;
	hcan1.Init.TXFP = ENABLE;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SJW = CAN_SJW_1TQ;
	hcan1.Init.BS1 = CAN_BS1_9TQ;
	hcan1.Init.BS2 = CAN_BS2_4TQ;
	hcan1.Init.Prescaler = CAN1_Prescaler / (1 + 9 + 4);
	hcan1.pTxMsg = &can1_TxMsg;
	hcan1.pRxMsg = &can1_pRxMsg;

	state = HAL_CAN_Init(&hcan1);

	can1_filter.FilterNumber = 0;
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can1_filter.FilterIdHigh = 0x0000;
	can1_filter.FilterIdLow = 0x0000;
	can1_filter.FilterMaskIdHigh = 0x0000;
	can1_filter.FilterMaskIdLow = 0x0000;
	can1_filter.FilterFIFOAssignment = 0;
	can1_filter.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(&hcan1, &can1_filter);

	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
	
	return state;
}

HAL_StatusTypeDef can1_send(uint8_t *msg, uint8_t len, uint8_t id)
{
	uint8_t i = 0;
	HAL_StatusTypeDef state;


	can1_TxMsg.StdId = (id);
	can1_TxMsg.ExtId = 0;
	can1_TxMsg.IDE = CAN_ID_STD;
	can1_TxMsg.RTR = CAN_RTR_DATA;
	can1_TxMsg.DLC = len;

	for (i = 0; i < len; i++)
	{
		can1_TxMsg.Data[i] = msg[i];
	}

	state = HAL_CAN_Transmit(&hcan1, 10);
	return state;
}

void Send_to_PTZmoto(short Ip, short Ir, short Iy)
{
	can1_TxMsg.StdId = 0x200;
	can1_TxMsg.ExtId = 0;
	can1_TxMsg.IDE = CAN_ID_STD;
	can1_TxMsg.RTR = CAN_RTR_DATA;
	can1_TxMsg.DLC = 8;

	can1_TxMsg.Data[0] = Ip >> 8 & 0x00ff;
	can1_TxMsg.Data[1] = Ip & 0x00ff;

	can1_TxMsg.Data[2] = Ir >> 8 & 0x00ff;
	can1_TxMsg.Data[3] = Ir & 0x00ff;

	can1_TxMsg.Data[4] = Iy >> 8 & 0x00ff;
	can1_TxMsg.Data[5] = Iy & 0x00ff;

	can1_TxMsg.Data[6] = 0;
	can1_TxMsg.Data[7] = 0;

	HAL_CAN_Transmit(&hcan1, 10);
}

void CAN1_RX0_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan1);
}

char times = 1;
char timep = 1;
short speed = 0;
short speedreal = 0;
short speeds = 0;
int positions = 0;
short di = 0;
short errorsum = 0;
short errornow = 0;
short errorlast = 0;
short errordiff = 0;
float datasin = 0;
float Wsin = 1;

float Pset = 10;
float Iset = 2;
float Dset = 1;
float aset = 0.6;

float Pset2 = 0.25;
float Iset2 = 0.01;
float Dset2 = 1;

short errorsump = 0;
short errornowp = 0;
short errorlastp = 0;
short errordiffp = 0;

int slimt = 1000;


void CAN1_RX0_Handler()
{
	times++;
	if (times==5)
	{
		times = 0;
	}
	if (can1_pRxMsg.StdId == 0x201)
	{
		short last_angle;
		short speed_temp;
		PTZmoto[0].id = can1_pRxMsg.StdId;
		PTZmoto[0].hall = can1_pRxMsg.Data[6];
		PTZmoto[0].Iset = (can1_pRxMsg.Data[4] << 8) + can1_pRxMsg.Data[5];
		PTZmoto[0].Ireal = (can1_pRxMsg.Data[2] << 8) + can1_pRxMsg.Data[3];
		last_angle = PTZmoto[0].angle;
		PTZmoto[0].angle = (can1_pRxMsg.Data[0] << 8) + can1_pRxMsg.Data[1];

		di = PTZmoto[0].Ireal - PTZmoto[0].Iset;

		speed_temp = PTZmoto[0].angle - last_angle;
		if (speed_temp > 4096)
		{
			PTZmoto[0].count--;
			PTZmoto[0].speed = -8192 + speed_temp ;
		} 
		else if (speed_temp < -4096)
		{
			PTZmoto[0].count++;
			PTZmoto[0].speed = 8192 + speed_temp;
		}
		else
		{
			PTZmoto[0].speed = speed_temp;
		}
		PTZmoto[0].position += PTZmoto[0].speed;

		speed+=PTZmoto[0].speed;

		if (times==0)
		{
			short out = 0;

			errorlast = speedreal;
			speedreal = aset*speed + (1- aset)*speedreal;

			errordiff = speedreal - errorlast;

			datasin = arm_sin_f32(HAL_GetTick() / 1000.0 * Wsin * 2 * PI);
			//if ((HAL_GetTick() / 3000)%2)
			//{
			//	datasin = 1;
			//}
			//else
			//	datasin = 0;
			
			
			//speeds = datasin * 100;
			//speeds = 120;
			errornow = speeds - speedreal;
			errorsum += errornow;
			//errorsum = fabs(errornow)<20? (errorsum +errornow): errorsum;
			
			//float ferr = fabs(errornow)<50 ? (50 - fabs(errornow)) / 50.0 : 0.0;
			//errorsum += errornow*ferr;

			errorsum = errorsum > slimt ? slimt : errorsum;
			errorsum = errorsum < -slimt ? -slimt : errorsum;

			//out = errornow * 2+ errorsum*1.5;
			out = errornow * Pset + errorsum * Iset + errordiff * Dset;
			out = out > 5000 ? 5000 : out;
			out = out < -5000 ? -5000 : out;

			Send_to_PTZmoto(out, 0, 0);

			//timep++;
			//if (timep==5)
			//{
			//	timep = 0;
			//}

			/*if (timep == 0)
			{
				positions = datasin * 1000;
				errornowp = positions - PTZmoto[0].position;
				errorsump += errornowp;
				speeds = Pset2*errornowp + Iset2*errorsump;
			}*/



			speed = 0;
		}
	}


	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
}