#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "can2.h"
	extern short Map_X1, Map_Y1_Up, Map_Y1_Down;		//��һ�ŵ�ͼX,Y����;
	extern short Map_X2_Left, Map_X2_Right, Map_Y2_Up, Map_Y2_Down;//�ڶ��ŵ�ͼX,Y����,���ڳ����м䣬y������ֵ;
	extern short Map_Init_Flag;    //0Ϊδ��ʼ����ͼ;1Ϊ��ʼ����ͼ;
	int Map_Positon_X, Map_Positon_Y;

	extern int Position_Offset[3];
	extern double Place_Offset[3];
	extern short CarMove_Flag;
	extern PLACE_TYPE	Place_real[3];

	typedef struct
	{
		short Left;
		short Right;
		short Up;
		short Down;
	}Map;
	typedef struct
	{
		short Over_X1;
		short Over_Y1_Up;
		short Over_Y1_Down;
		short Over_X2_Right;
		short Over_X2_Left;
		short Over_Y2_Up;
		short Over_Y2_Down;
	}OverMap;
	void Map_Init(void);
	void Over_Map(void);
	void Map_Limit(void);
#ifdef __cplusplus
}
#endif
