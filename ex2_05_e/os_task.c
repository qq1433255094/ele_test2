
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


extern int32_t position_x, position_y;

osThreadId		TESTThreadHandle;

static void TEST ( void const * argument );


void os_task_init(void)
{
	stepmotor_init();

	osThreadDef ( TESTHandle,TEST,osPriorityNormal,0,configMINIMAL_STACK_SIZE );


	TESTThreadHandle	= osThreadCreate ( osThread ( TESTHandle ),NULL );
}

void os_task_start ( void )
{
	/* Start scheduler */

	//osThreadSuspend ( TESTThreadHandle );

	osKernelStart ();


}


float w1, w2;




void TEST ( void const * argument )
{
	( void )
	argument;
	int32_t tick = 0;
	for ( ; ; )
	{	
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

		//Linear_interpolation(200, 800);
		//Linear_interpolation(200, 400);
		//Linear_interpolation(600, 400);
		//Linear_interpolation(600, 800);
		//plot_circular(400, 600, 250);
		//Linear_interpolation(400, 800);
		

			//cal_step(1, 0);
			//osDelay(10);
			//int32_t time = (int32_t)TIM3->CNT;
			//while ((int32_t)TIM3->CNT  - time < 1)
			//{
			//	asm("bkpt 255");
			//}
			//int32_t time2 = (int32_t)TIM3->CNT;

			//time = (int32_t)TIM3->CNT;
			//if (TIM3->CNT ==999)
			//{
			//	asm("bkpt 255");
			//}
			//uint32_t time = 0;
			//while (TIM3->CNT<10)
			//{
			//}
			//time = TIM3->CNT;
			//asm("bkpt 255");
		//Linear_interpolation(5000, 5000);
		//Linear_interpolation(0, 0);
		//Linear_interpolation(-5000, 5000);
		//Linear_interpolation(0, 0);
		//Linear_interpolation(-5000, -5000);
		//Linear_interpolation(0, 0);
		//Linear_interpolation(5000, -5000);
		//Linear_interpolation(0, 0);
		//Linear_interpolation(10000, 10000);
		//Linear_interpolation(0, 10000);
		//Linear_interpolation(0, 0);
		//for (int16_t i= 0;i<5000;i++)
		//{
		//	call_step(3);
		//}
		//call_step(1);
		//osDelay(200);
		plot_circular(0, 0, 5000);
		//osThreadSuspend(TESTThreadHandle);


	}
}


