#include "map.h"
OverMap Overmap;
Map Map1, Map2;
void Map_Init(void)
{
	
	Map_Positon_X = Place_real[CH_X];
	Map_Positon_Y = Place_real[CH_Y];
	//地图一初始化;
	Map1.Left = Map_Positon_X - 1680;
	Map1.Up = Map_Positon_Y - 0;
	Map1.Down = Map_Positon_Y - 1600;
	//地图二初始化;
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
	if (Map_Init_Flag == 1)	//地图初始化之后才开始判断是否限幅;
	{
		if (Map_Positon_X >0)	//在右半边地图;
		{
			if (Map_Positon_X > Map2.Right)	//超出右地图
			{
				Overmap.Over_X2_Right = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y > Map2.Up)		//超过右上边界;
			{
				Overmap.Over_Y2_Up = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y < Map2.Down)		//超出右下边界;
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
		else               //在左地图;
		{
			if (Map_Positon_X < Map1.Left)			//超出左边界;
			{
				Overmap.Over_X1 = 1;
				CarMove_Flag = 0;
			}
			else if (Map_Positon_Y > Map1.Up)		//超出左上边界;
			{
				Overmap.Over_Y1_Up = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y >  Map1.Up + 50)		//50是车的宽度;从右边地图的Map_X2_Left超出;
			{
				Overmap.Over_X2_Left = 1;
				CarMove_Flag = 0;
			}
			
			else if (Map_Positon_Y < Map1.Down)		//超出左下边界;
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