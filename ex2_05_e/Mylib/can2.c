#include "can2.h"
#include "arm_math.h"
#include <math.h>
#include "map.h"
#define  add_test

CAN_HandleTypeDef hcan2;
CanTxMsgTypeDef   can2_TxMsg;
CanRxMsgTypeDef   can2_pRxMsg;
short RC_ch3 = 0;
short RC_ch2 = 0;
short RC_ch1 = 0;
uint8_t RC_s1 = 0;
uint8_t RC_s2 = 0;
uint8_t Lock_enable=0;
PLACE_TYPE Lock_angle=0;



extern short speedset[4];
extern int Position_real[3]; //相对位置

PLACE_TYPE Place_real[3] = { 0 };//全局位置
extern short accdata[4];
extern short Map_X1, Map_Y1_Up, Map_Y1_Down;		//第一张地图X,Y坐标;
extern short Map_X2_Left, Map_X2_Right, Map_Y2_Up, Map_Y2_Down;//第二张地图X,Y坐标,由于车放中间，y有两个值;
extern OverMap Overmap;
extern double Place_Offset[3];
extern short CarMove_Flag;	//车移动标志;
HAL_StatusTypeDef can2_init(void)
{
	HAL_StatusTypeDef state;
	GPIO_InitTypeDef gpio_init;
	CAN_FilterConfTypeDef can2_filter;
	
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_CAN2_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	
	gpio_init.Pin = GPIO_PIN_12 | GPIO_PIN_13;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Alternate = GPIO_AF9_CAN2;
	HAL_GPIO_Init(GPIOB, &gpio_init);
	
	hcan2.Instance = CAN2;
	hcan2.Init.TTCM = DISABLE;
	hcan2.Init.ABOM = DISABLE;
	hcan2.Init.AWUM = DISABLE;
	hcan2.Init.NART = DISABLE;
	hcan2.Init.RFLM = DISABLE;
	hcan2.Init.TXFP = ENABLE;
	hcan2.Init.Mode = CAN_MODE_NORMAL;
	hcan2.Init.SJW = CAN_SJW_1TQ;
	hcan2.Init.BS1 = CAN_BS1_9TQ;
	hcan2.Init.BS2 = CAN_BS2_4TQ;
	hcan2.Init.Prescaler = CAN2_Prescaler / (1 + 9 + 4);
	hcan2.pTxMsg = &can2_TxMsg;
	hcan2.pRxMsg = &can2_pRxMsg;
	
	state = HAL_CAN_Init(&hcan2);
	
	can2_filter.FilterNumber = 14;
	can2_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can2_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can2_filter.FilterIdHigh = 0x0000;
	can2_filter.FilterIdLow = 0x0000;
	can2_filter.FilterMaskIdHigh = 0x0000;
	can2_filter.FilterMaskIdLow = 0x0000;
	can2_filter.FilterFIFOAssignment = 0;
	can2_filter.FilterActivation = ENABLE;
	
	HAL_CAN_ConfigFilter(&hcan2, &can2_filter);
	
	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);

	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);


	return state;
}

#pragma region CalAccel

void CalAccel(short *ch, short *out, short *acc, short len)
{
	int i = 0;
	int a = 0;
	int d = 0;
	for (i = 0; i < len; i++)
	{
		d = ch[i] - out[i];
		a = acc[i];// +abs(d) / 20;
		if (d > a){
			out[i] += a;
		}
		else if (d < -a){
			out[i] -= a;
		}
		else{
			out[i] = ch[i];
		}
	}
}

float Place_temp[4];
#define X_Scale 50.0f
#define Y_Scale 50.0f
#define A_Scale 50.0f

//49.57
void Lock_Direction(PLACE_TYPE angleset)
{
	Car_Move_Position_To(CH_A,angleset,20,ABS);
	Lock_angle=angleset;
	Lock_enable=1;
}
void UnLock_Direction()
{
	Lock_enable=0;
}
void CalPosition(short speedset[], short *accdata)
{
	static short ToCar[4];
	static int Position_data[4];

	if (Overmap.Over_X2_Right == 1)//如果超出右边地图;
	{
		speedset[CH_X] = -200;
	}

	CalAccel(speedset, ToCar, accdata, 4);
#if (USE_LOCAL_PLACE ==1 )
	Position_data[CH_X] += ToCar[CH_X];
	Position_data[CH_Y] += ToCar[CH_Y];
	Position_data[CH_A] += ToCar[CH_A];
	
#if (USE_DOUBLE_PLACE ==1 )
	Place_real[CH_X] += ToCar[CH_Y] / Y_Scale * sin(Position_real[CH_A] / 180.0*PI) + ToCar[CH_X] / X_Scale * cos(Position_real[CH_A] / 180.0*PI);
	Place_real[CH_Y] += ToCar[CH_Y] / Y_Scale * cos(Position_real[CH_A] / 180.0*PI) - ToCar[CH_X] / X_Scale * sin(Position_real[CH_A] / 180.0*PI);
	Place_real[CH_A] += ToCar[CH_A] / A_Scale;
#else
	Place_real[CH_X] += ToCar[CH_Y] / Y_Scale * arm_sin_f32(Position_real[CH_A] / 180.0*PI) + ToCar[CH_X] / X_Scale * arm_cos_f32(Position_real[CH_A] / 180.0*PI);
	Place_real[CH_Y] += ToCar[CH_Y] / Y_Scale * arm_cos_f32(Position_real[CH_A] / 180.0*PI) - ToCar[CH_X] / X_Scale * arm_sin_f32(Position_real[CH_A] / 180.0*PI);
	Place_real[CH_A] += ToCar[CH_A] / A_Scale;
#endif // USE_DOUBLE_PLACE==1

	

	Position_real[CH_X] = Position_data[CH_X] / X_Scale;
	Position_real[CH_Y] = Position_data[CH_Y] / Y_Scale;
	Position_real[CH_A] = Position_data[CH_A] / A_Scale;

	//Place_real[CH_X] = Place_temp[CH_X] / 230;
	//Place_real[CH_Y] = Place_temp[CH_Y] / 40.8;
	//Place_real[CH_A] = Position_real[CH_A];
#endif
	
	if(Lock_enable)
	{
		int16_t limit = 30;
		ToCar[CH_A] = (Lock_angle - Place_real[CH_A])*8;

		ToCar[CH_A] = ToCar[CH_A]>limit ? limit : ToCar[CH_A];
		ToCar[CH_A] = ToCar[CH_A]<-limit ? -limit : ToCar[CH_A];

	}
	
	CAN2_Send(ToCar);
}
#pragma endregion

HAL_StatusTypeDef can2_send(uint8_t *msg, uint8_t len, uint8_t id)
{	
	uint8_t i = 0;
	HAL_StatusTypeDef state;
	
	
	can2_TxMsg.StdId = (id << 3);
	can2_TxMsg.ExtId = 0;
	can2_TxMsg.IDE = CAN_ID_STD;
	can2_TxMsg.RTR = CAN_RTR_DATA;
	can2_TxMsg.DLC = len;
	
	for (i = 0; i < len; i++)
	{
		can2_TxMsg.Data[i] = msg[i];
	}
	
	state = HAL_CAN_Transmit(&hcan2, 10);
	return state;
}

void CAN2_Send(short *ch)
{
	uint8_t CAN_CH[8];
	short temp[4];
	temp[0] = ch[0] / 2.60 ;
	temp[1] = ch[1] / 2.90 ;
	temp[2] = ch[2] * 3.10 ;

	CAN_CH[0] = (temp[0]) >> 8;
	CAN_CH[1] = (temp[0]) & 0x00ff;

	CAN_CH[2] = (temp[1]) >> 8;
	CAN_CH[3] = (temp[1]) & 0x00ff;

	CAN_CH[4] = (temp[2]) >> 8;
	CAN_CH[5] = (temp[2]) & 0x00ff;

	CAN_CH[6] = (temp[3]) >> 8;
	CAN_CH[7] = (temp[3]) & 0x00ff;

	can2_send(CAN_CH, 8, 0);
}

void Car_Move_Place_To(uint8_t ch, int val, short speed_set, uint8_t mode)
{
	PLACE_TYPE P_Start = Place_real[ch];
	PLACE_TYPE P_End;
	int8_t dir = 1;
	float speed = 0;
	float a = 50;
	int16_t Time = 10;

	switch (ch){
	case CH_X: a = 200; break;	//自动移动的速度;
	case CH_Y: a = 200; break;
	case CH_A: a = 20; UnLock_Direction(); break;
	default:
		break;
	}

	if (mode == ABS) {
		P_End = val;
	}
	else {
		P_End = P_Start + val;	//计算运动结束位置
	}

	if (P_End < P_Start){
		dir = -1;	//判断运动方向  
	}
	
	while (((Place_real[ch] - P_Start)*dir < (speed_set*1.0*speed_set / 2.0 / a))		//加速条件1 当前已经移动的距离小于加速位移
			&& (2 * (int32_t)Place_real[ch] * dir < (int32_t)(P_Start + P_End)*dir))	//加速条件2 当前已经移动的距离小于总距离的一半	//乘法运算速度大于除法运算速度;
	{
		if (CarMove_Flag == 0)				//已经超过地图范围
			break;							//不再加速;
		//if (CarMove_Flag == 2)				//到达地图临界区;
		//	break;							//不再加速;

		speed = speed + a*Time / 1000.0;    //加速									
		if (speed > (float)speed_set)	    //速度限幅;
		{
			speed = speed_set;
		}
		speedset[ch] = speed*dir;		    //给定运动方向
		osDelay(Time);
	}

	while ((P_End - Place_real[ch])*dir > (speed_set*1.0*speed_set / 2.0 / a))  //恒速条件 当前位置与结束位置大于于加速位移
	{
		if (CarMove_Flag == 0)				//已经超过地图范围
			break;							//不再匀速;
		//if (CarMove_Flag == 2)				//到达地图临界区;
		//	break;							//不再匀速;
		osDelay(Time);						//匀速
	}

	while (speed > 0 && (P_End - Place_real[ch])*dir > 0)						//减速条件1 当前位置与结束位置之差大于0													//减速条件2 到达地图临界区;  两者只要一个满足则减速;
	{
		if (CarMove_Flag == 0)		//已经超过地图范围
			break;					//不再移动;
		speed = speed - a*Time / 1000.0; //减速
		speedset[ch] = speed*dir;		//给定运动方向
		osDelay(Time);
	}
	//while (CarMove_Flag == 0)		//超出地图,往回移;
	//{
	//	if (dir > 0)
	//		speedset[ch] = -100;	//匀速回来;
	//	else
	//		speedset[ch] = 100;
	//}
		
	speedset[ch] = 0;
}

void Car_Move_Position_To(uint8_t ch, int val, short speed_set, uint8_t mode)
{
	int32_t P_Start = Position_real[ch];
	int32_t P_End;
	int8_t dir = 1;
	float speed = 0;
	float a = 50;
	int16_t Time = 10;

	switch (ch){
	case CH_X: a = 200; break;
	case CH_Y: a = 200; break;
	case CH_A: a = 20; UnLock_Direction(); break;
	default:
		break;
	}

	if (mode == ABS) {
		P_End = val;
	}
	else {
		P_End = P_Start + val;  //计算运动结束位置
	}

	if (P_End < P_Start){
		dir = -1;		//判断运动方向
	}

	while (((Position_real[ch] - P_Start)*dir < (speed_set*1.0*speed_set / 2.0 / a))		//加速条件1 当前已经移动的距离小于加速位移
			&& ( 2 *(int32_t) Position_real[ch]*dir  < (int32_t)(P_Start + P_End)*dir ))	//加速条件2 当前已经移动的距离小于总距离的一半
	{
		speed = speed + a*Time / 1000.0; //加速
		speedset[ch] = speed*dir;		//给定运动方向
		osDelay(Time);
	}

	while ((P_End - Position_real[ch])*dir > (speed_set*1.0*speed_set / 2.0 / a)) //恒速条件 当前位置与结束位置之差大于加速位移
	{
		osDelay(Time); //恒速
	}

	while (speed > 0 && (P_End - Position_real[ch])*dir > 0) //减速条件 当前位置与结束位置之差大于0
	{
		speed = speed - a*Time / 1000.0; //减速
		speedset[ch] = speed*dir;		//给定运动方向
		osDelay(Time);
	}

	speedset[ch] = 0;
}

void move_to(int x, int y, int f, uint8_t mode)
{
	double origin[2],car_a;
	Car_Move_Position_To(CH_A, 0, 10, ABS);
	if (mode==ABS){
		x = x - Place_real[CH_X];
		y = y - Place_real[CH_Y];
	}
	if (x==0){
		Car_Move_Position_To(CH_Y, y, f, INC);
		return;
	}
	if (y==0){
		Car_Move_Position_To(CH_X, x, f, INC);
		return;
	}
	origin[CH_X] = Place_real[CH_X];
	origin[CH_Y] = Place_real[CH_Y];

	car_a = atanf((float)y / (float)x);

	if (x < 0)
	{
		car_a = car_a + PI;
	}
	else if (y < 0)
	{
		car_a = PI * 2 + car_a;
	}

	speedset[CH_X] = cos(car_a)*f;
	speedset[CH_Y] = sin(car_a)*f;

	if ((int32_t)x>0)
	{
		while ((Place_real[CH_X] - origin[CH_X] -(float)x) < -10)
		{
			osDelay(5);
		}
	}
	else
	{
		while ((Place_real[CH_X] - origin[CH_X] - (float)x) > 10)
		{
			osDelay(5);
		}
	}
	
	speedset[CH_X] = 0;
	speedset[CH_Y] = 0;

}


HAL_CAN_StateTypeDef CAN2_GetState(void)
{
	return hcan2.State;
}

void CAN2_RX0_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_RX0_Handler()
{
	switch (can2_pRxMsg.StdId)
	{
	case 0x01:
		RC_ch1 = *((short *)(&can2_pRxMsg.Data[0]));
		RC_s1 = can2_pRxMsg.Data[2];
		RC_s2 = can2_pRxMsg.Data[3];
		RC_ch2 = *((short *)(&can2_pRxMsg.Data[4]));
		RC_ch3 = *((short *)(&can2_pRxMsg.Data[6]));
		break;
	case 0x10:
		Position_real[CH_X] = *(float *)(&can2_pRxMsg.Data[0]);
		Place_real[CH_X] = *(float *)(&can2_pRxMsg.Data[4]) - Place_Offset[CH_X];
		break;
	case 0x11:
		Position_real[CH_Y] = *(float *)(&can2_pRxMsg.Data[0]);
		Place_real[CH_Y] = *(float *)(&can2_pRxMsg.Data[4]) - Place_Offset[CH_Y];
		break;
	case 0x12:
		Position_real[CH_A] = *(float *)(&can2_pRxMsg.Data[0]);
		Place_real[CH_A] = *(float *)(&can2_pRxMsg.Data[4]) - Place_Offset[CH_A];
		break;
	default:
		break;
	}
	
	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
}

/*
 
 can2_data[0] = RC_Ctl.rc.ch3 & 0xff;// +(RC_Ctl.rc.s1 << 14);
 can2_data[1] = RC_Ctl.rc.ch3 >> 8;
 can2_data[2] = RC_Ctl.rc.s1;
 can2_data[3] = RC_Ctl.rc.s2;
 Can2SendMsg(can2_data, 4, 0x01);

 Sendfdata[0] = position.x;
 Sendfdata[1] = realposition.x;
 Can2SendMsg((u8 *)Sendfdata, 8, 0x10);

 Sendfdata[0] = position.y;
 Sendfdata[1] = realposition.y;
 Can2SendMsg((u8 *)Sendfdata, 8, 0x11);

 Sendfdata[0] = position.a;
 Sendfdata[1] = realposition.a;
 Can2SendMsg((u8 *)Sendfdata, 8, 0x12);

 */
