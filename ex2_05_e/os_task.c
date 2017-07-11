
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
	int8_t tick = 0;
	for ( ; ; )
	{	
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

		//Linear_interpolation(200, 800);
		//Linear_interpolation(200, 400);
		//Linear_interpolation(600, 400);
		//Linear_interpolation(600, 800);
		plot_circular(400, 600, 250);
		Linear_interpolation(400, 800);
		
		while (1)
		{
			//cal_step(1, 0);
			osDelay(10);
		}
	}
}


