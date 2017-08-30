
#include "os_task.h"
#include "uart4.h"
#include "usart6.h"
#include "usart3.h"
#include "tim3.h"
#include "tim4.h"
#include "button.h"
#include "rng.h"
#include "mymath.h"
#include <math.h>
#include "arm_math.h"
#include "oled.h"
#include "stepmotor.h"
#include "ff.h"
#include "GUI.h"


extern int32_t position_x, position_y;
FRESULT fr;
FIL fp;
uint8_t stat;
DIR dir;
FILINFO fno;
FATFS fs;
uint8_t sd_buff[512];
uint8_t str[128];

CREATE_TASK_DEFINE(TEST);
CREATE_TASK_DEFINE(DISPLAY);

int8_t get_gcode(FIL *fp);

void os_task_init(void)
{
	stepmotor_init();

	fr = f_mount(&fs, "", 0);
	fr = f_opendir(&dir, "/");

	fr = f_open(&fp, "0001.gcode", FA_READ);



	CREATE_TASK_HANDLE(TEST, osPriorityAboveNormal);
	CREATE_TASK_HANDLE(DISPLAY, osPriorityNormal);
}

void os_task_start ( void )
{
	/* Start scheduler */
	//osThreadSuspend(os_ThreadHandle(TEST));
	osKernelStart ();
}


float w1, w2;


os_exec(TEST){
	(void)argument;
	int32_t tick = 0;
	for (; ; )
	{
		//plot_circular(0, 0, 5000);
		//tim3_delay_us(100);
		//TOGGLE_L_CLK();
		tim3_delay_us(500);
		osDelay(10);
	}
}
os_exec(DISPLAY) {
	(void)argument;
	//int32_t tick = HAL_GetTick();
	int8_t sta = 0;
	for (; ; )
	{
		

		//OLED_Print(0, 6, "1234");
		//sta = f_gets(str, 100, &fp);
		get_gcode(&fp);
		//tim3_delay_us(500);
		//tim3_delay_us(500);
		//tim3_delay_us(500);
		osDelay(10);
		//while (f_gets(str, 100, &fp) != 0)
		//{
		//	OLED_Print(0, 6, str);
		//	//HAL_Delay(100);
		//	osDelay(100);
		//}
		//fr = f_close(&fp);
		//osThreadSuspend(os_ThreadHandle(DISPLAY));
	}
}

int8_t get_gcode(FIL *fp)
{
	char enable = 0;
	//char str[128];
	char seek = 0;
	char i = 0;
	char start = 0;
	char cmd = 0;
	float val = 0;
	float point = 1;
	char dir = 1;

	float data = 0;

	if (f_gets(str, 100, fp) == 0)
		return -1;
	//OLED_Print(0, 6, str);

	for (i = 0; i < 120; i++)
	{
		if (str[seek] == 'G' || str[seek] == 'g' ||
			str[seek] == 'M' || str[seek] == 'm' ||
			str[seek] == 'T' || str[seek] == 't' ||
			str[seek] == 'F' || str[seek] == 'f' ||
			str[seek] == 'S' || str[seek] == 's' ||
			str[seek] == 'X' || str[seek] == 'x' ||
			str[seek] == 'Y' || str[seek] == 'y' ||
			str[seek] == 'Z' || str[seek] == 'z' ||
			str[seek] == 'A' || str[seek] == 'a' ||
			str[seek] == 'B' || str[seek] == 'b' ||
			str[seek] == 'C' || str[seek] == 'c' ||
			str[seek] == 'E' || str[seek] == 'e'
			) {
			enable = 1;
			cmd = str[seek];
			start = 0;
			seek++;
			val = 0;
			point = 1;
			dir = 1;

			while (str[seek] == ' ' || str[seek] == '-' || str[seek] == '.' || (str[seek] >= '0' && str[seek] <= '9'))
			{
				if (str[seek] >= '0' && str[seek] <= '9')
				{
					val = val * 10 + (str[seek] - '0');
					if (start == 1)
					{
						point = point * 10;
					}
				}
				else if (str[seek] == '.')
				{
					start = 1;
				}
				else if (str[seek] == '-')
				{
					dir = -1;
				}
				seek++;
			}
			data = val / point;
			if (cmd == 'M' && (int32_t)val == 84)
			{
				//OLED_PrintN(0, 6, "M ", val);
				return -1;

			}
			if (cmd == 'Z')
			{
				//OLED_PrintN(0, 6, "Z ", val);
			}
		}
		else {
			if (str[seek] == ' ')
			{
				seek++;
			}
			else if (str[seek] == ';' || str[seek] == '\n')
			{
				if (enable == 0)
				{
					if (f_gets(str, 100, fp) == 0)
						return -1;
					//	OLED_Print(0, 6, str);
					i = 0;
					seek = 0;
				}
				else {
					//OLED_Print(0, 6, str);
					return 0;
				}
			}
		}
		//seek++;
	}
	return 0;
}


