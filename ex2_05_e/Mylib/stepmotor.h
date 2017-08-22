#ifndef __STEPMOTPR_H_
#define __STEPMOTPR_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"

#define STEP_LEN (12.3*PI/400.0)

#define FORWARD GPIO_PIN_SET
#define BACKWARD GPIO_PIN_RESET

#define R_MOTOR_DIR_PIN GPIO_PIN_12
#define R_MOTOR_DIR_PORT GPIOD
#define L_MOTOR_DIR_PIN GPIO_PIN_14
#define L_MOTOR_DIR_PORT GPIOD
	
#define R_MOTOR_CLK_PIN GPIO_PIN_13
#define R_MOTOR_CLK_PORT GPIOD
#define L_MOTOR_CLK_PIN GPIO_PIN_15
#define L_MOTOR_CLK_PORT GPIOD
	
#define SET_L_DIR_FORWARD() HAL_GPIO_WritePin(L_MOTOR_DIR_PORT, L_MOTOR_DIR_PIN, FORWARD)
#define SET_L_DIR_BACKWARD() HAL_GPIO_WritePin(L_MOTOR_DIR_PORT, L_MOTOR_DIR_PIN, BACKWARD)
	
#define SET_R_DIR_FORWARD() HAL_GPIO_WritePin(R_MOTOR_DIR_PORT, R_MOTOR_DIR_PIN, FORWARD)
#define SET_R_DIR_BACKWARD() HAL_GPIO_WritePin(R_MOTOR_DIR_PORT, R_MOTOR_DIR_PIN, BACKWARD)

#define SET_L_CLK_HIGH() HAL_GPIO_WritePin(L_MOTOR_CLK_PORT, L_MOTOR_CLK_PIN, GPIO_PIN_SET)
#define SET_L_CLK_LOW() HAL_GPIO_WritePin(L_MOTOR_CLK_PORT, L_MOTOR_CLK_PIN, GPIO_PIN_RESET)

#define SET_R_CLK_HIGH() HAL_GPIO_WritePin(R_MOTOR_CLK_PORT, R_MOTOR_CLK_PIN, GPIO_PIN_SET)
#define SET_R_CLK_LOW() HAL_GPIO_WritePin(R_MOTOR_CLK_PORT, R_MOTOR_CLK_PIN, GPIO_PIN_RESET)
	
#define TOGGLE_L_CLK() HAL_GPIO_TogglePin(L_MOTOR_CLK_PORT, L_MOTOR_CLK_PIN)
#define TOGGLE_R_CLK() HAL_GPIO_TogglePin(R_MOTOR_CLK_PORT, R_MOTOR_CLK_PIN)
	 
	void stepmotor_init(void);
	void cal_line_lenght(int32_t pos_x, int32_t pos_y, float *lenght_l, float *lenght_r);
	void cal_line_inc(int32_t pos_x, int32_t pos_y, float *lenght_l, float *lenght_r, int32_t dx, int32_t dy);
	void cal_step(int32_t dx, int32_t dy);
	void Linear_interpolation(int32_t xe, int32_t ye);
	void plot_circular(int32_t x, int32_t y, int32_t r);
	void alloc_clk(void);
	void call_step(int8_t zf);
	void tim3_delay_us(uint16_t num);
	
#ifdef __cplusplus
}
#endif

#endif




