#include "map.h"
OverMap Overmap;
Map Map1, Map2;
void Map_Init(void)
{
	
	Map_Positon_X = Place_real[CH_X];
	Map_Positon_Y = Place_real[CH_Y];
	//��ͼһ��ʼ��;
	Map1.Left = Map_Positon_X - 1680;
	Map1.Up = Map_Positon_Y - 0;
	Map1.Down = Map_Positon_Y - 1600;
	//��ͼ����ʼ��;
	Map2.Left = Map_Positon_X;
	Map2.Right = Map_Positon_X + 70;
	Map2.Up = Map_Positon_Y + 2200;
	Map2.Down = Map_Positon_Y - 1680;

	Map_Init_Flag = 1;
}
void Over_Map(void)
{
	Map_Positon_X = Place_real[CH_X];
	Map_Positon_Y = Place_real[CH_Y];
	if (Map_Init_Flag == 1)	//��ͼ��ʼ��֮��ſ�ʼ�ж��Ƿ��޷�;
	{
		if (Map_Positon_X >0)	//���Ұ�ߵ�ͼ;
		{
			if (Map_Positon_X > Map2.Right)	//�����ҵ�ͼ
			{
				Overmap.Over_X2_Right = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y > Map2.Up)		//�������ϱ߽�;
			{
				Overmap.Over_Y2_Up = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y < Map2.Down)		//�������±߽�;
			{
				Overmap.Over_Y2_Down = 1;
				CarMove_Flag = 0;
			}
			else
			{
				Overmap.Over_X2_Right = 0;
				Overmap.Over_Y2_Down = 0;
				Overmap.Over_Y2_Down = 0;
				CarMove_Flag = 1;
			}
		}
		else               //�����ͼ;
		{
			if (Map_Positon_X < Map1.Left)			//������߽�;
			{
				Overmap.Over_X1 = 1;
				CarMove_Flag = 0;
			}
			else if (Map_Positon_Y > Map1.Up)		//�������ϱ߽�;
			{
				Overmap.Over_Y1_Up = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y >  Map1.Up + 50)		//50�ǳ��Ŀ��;���ұߵ�ͼ��Map_X2_Left����;
			{
				Overmap.Over_X2_Left = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y < Map1.Down)		//�������±߽�;
			{
				Overmap.Over_Y1_Down = 1;
				CarMove_Flag = 0;
			}
			else
			{
				Overmap.Over_X1 = 0;
				Overmap.Over_Y1_Up = 0;
				Overmap.Over_X2_Left = 0;
				Overmap.Over_Y1_Down = 0;
				CarMove_Flag = 1;
			}
		}
	}
	else
	{
		Overmap.Over_X1 = 0;
		Overmap.Over_Y1_Up = 0;
		Overmap.Over_Y1_Down = 0;
		Overmap.Over_X2_Left = 0;
		Overmap.Over_X2_Right = 0;
		Overmap.Over_Y2_Up = 0;
		Overmap.Over_Y2_Down = 0;
	}
	
}
void Map_Limit(void)
{

}