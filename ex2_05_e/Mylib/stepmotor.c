#include "stepmotor.h"
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "os_task.h"
#include "mymath.h"

float step_l_diff = 0, step_r_diff = 0;
int32_t step_l = 0 , step_r = 0;
int32_t dir_l = 1, dir_r = 1;
int32_t position_x=0, position_y=0;
float line_l, line_r;

void stepmotor_init(void)
{
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 |  GPIO_PIN_14 |  GPIO_PIN_15;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	SET_L_CLK_LOW();
	SET_R_CLK_LOW();
	//cal_line_lenght(position_x, position_y, &line_l, &line_r);

}

void tim3_delay_us(uint16_t num)
{
	uint32_t time;
	uint8_t ex = 0;
	if (num > 1000)
		num = 1000;
	time = TIM3->CNT;
	while (!ex)
	{
		if (TIM3->CNT > time)
			if (TIM3->CNT - time >= num)
				ex = 1;
		if (TIM3->CNT < time)
			if (TIM3->CNT + 5000 - time >= num)
				ex = 1;
	}
}

void cal_line_lenght(int32_t pos_x, int32_t pos_y, float *lenght_l, float *lenght_r)
{
	*lenght_l = sqrt((pos_x + 120)*(pos_x + 120) + (pos_y + 120)*(pos_y + 120));
	*lenght_r = sqrt((920 - pos_x)*(920 - pos_x) + (pos_y + 120)*(pos_y + 120));
}

void cal_line_inc(int32_t pos_x, int32_t pos_y, float *lenght_l, float *lenght_r,int32_t dx,int32_t dy)
{
	float L1, R1, L2, R2;
	cal_line_lenght(pos_x, pos_y, &L1, &R1);
	cal_line_lenght(pos_x+dx, pos_y+dy, &L2, &R2);
	*lenght_l = L2 - L1;
	*lenght_r = R2 - R1;
}

void cal_step(int32_t dx, int32_t dy)
{
	float d_lenght_l, d_lenght_r;
	int32_t delay;
	cal_line_inc(position_x, position_y, &d_lenght_l, &d_lenght_r, dx, dy);

	position_x += dx;
	position_y += dy;

	step_l_diff += d_lenght_l;
	step_r_diff += d_lenght_r;

	dir_l = step_l_diff > 0 ? 1 : -1;
	dir_r = step_r_diff > 0 ? 1 : -1;

	step_l = (int32_t)(step_l_diff / STEP_LEN);
	step_r = (int32_t)(step_r_diff / STEP_LEN);

	line_l += step_l*STEP_LEN;
	line_r += step_r*STEP_LEN;

	step_l_diff -= (float)(step_l*STEP_LEN);
	step_r_diff -= (float)(step_r*STEP_LEN);

	delay = fabs(step_l) > fabs(step_r) ? fabs(step_l) : fabs(step_r);
	delay = delay * 2 + 4;
	osDelay(delay);
}


int16_t motor_tick = 0;
void alloc_clk(void)
{
	motor_tick++;
	if (motor_tick == 2)
		motor_tick = 0;

	if (motor_tick == 0)
	{
		if (step_l != 0)
		{
			if (step_l > 0){
				step_l--;
				SET_L_DIR_FORWARD();	
			}
			else{
				step_l++;
				SET_L_DIR_BACKWARD();
			}
			SET_L_CLK_HIGH();
		}
		if (step_r != 0)
		{
			if (step_r > 0) {
				step_r--;
				SET_R_DIR_FORWARD();
			}
			else {
				step_r++;
				SET_R_DIR_BACKWARD();
			}
			SET_R_CLK_HIGH();
		}
	}
	else
	{
		SET_L_CLK_LOW();
		SET_R_CLK_LOW();
	}
}

float rate;
void call_step(int8_t zf)
{
	switch (zf)
	{
	case 1:	SET_R_DIR_FORWARD();	SET_L_DIR_FORWARD();	tim3_delay_us(80* rate);
			SET_R_CLK_HIGH();		SET_L_CLK_HIGH();		tim3_delay_us(5);
			SET_R_CLK_LOW();		SET_L_CLK_LOW();		//tim3_delay_us(80); 
			position_x += 1;
			break;
	case 2:	SET_R_DIR_BACKWARD();	SET_L_DIR_BACKWARD();	tim3_delay_us(80* rate);
			SET_R_CLK_HIGH();		SET_L_CLK_HIGH();		tim3_delay_us(5);
			SET_R_CLK_LOW();		SET_L_CLK_LOW();		//tim3_delay_us(80);
			position_x -= 1;
			break;
	case 3:	SET_R_DIR_FORWARD();	SET_L_DIR_BACKWARD();	tim3_delay_us(80* rate);
			SET_R_CLK_HIGH();		SET_L_CLK_HIGH();		tim3_delay_us(5);
			SET_R_CLK_LOW();		SET_L_CLK_LOW();		//tim3_delay_us(80);
			position_y += 1;
			break;
	case 4:	SET_R_DIR_BACKWARD();	SET_L_DIR_FORWARD();	tim3_delay_us(80* rate);
			SET_R_CLK_HIGH();		SET_L_CLK_HIGH();		tim3_delay_us(5);
			SET_R_CLK_LOW();		SET_L_CLK_LOW();		//tim3_delay_us(80);
			position_y -= 1;
			break;
	default:
			break;
	}
}

void Linear_interpolation(int32_t xe,int32_t ye)
{
	
	int32_t fm = 0,nxy,d;
	int8_t xoy,zf;
	int32_t xr = abs(xe - position_x), yr = abs(ye - position_y);
	d = sqrt(xr*xr + yr*yr);
	nxy = xr+yr;
	rate = (float)d / (float)nxy;
	if (xe - position_x >=0)
	{
		if (ye - position_y >= 0) {
			xoy = 1;
		}
		else {
			xoy = 4;
		}
	} 
	else
	{
		if (ye - position_y >= 0) {
			xoy = 2;
		}
		else {
			xoy = 3;
		}
	}

	while (nxy != 0)
	{
		nxy--;

		if (fm >= 0){
			if (xoy==1 || xoy==4){
				zf = 1;
			} 
			else{
				zf = 2;
			}
			fm -= yr;
		} 
		else{
			if (xoy == 1 || xoy == 2){
				zf = 3;
			}
			else{
				zf = 4;
			}
			fm += xr;
		}
		call_step(zf);
		//osDelay(10);
	}
}

void plot_circular(int32_t x, int32_t y,int32_t r)
{
	int16_t i;
	int32_t x_set;
	int32_t y_set;
	for (i = 0; i < 64; i++)
	{
		x_set = x + r*cos(PI * 2 / 64.0*i);
		y_set = y + r*sin(PI * 2 / 64.0*i);

		Linear_interpolation(x_set, y_set);
		//osDelay(5);
	}
	x_set = x + r;
	y_set = y;
	Linear_interpolation(x_set, y_set);
	//osDelay(5);
}