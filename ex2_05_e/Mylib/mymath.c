#include "mymath.h"

float Kalman_filter(Kalman* data, float measure)
{
	data->x_mid = data->x_last;										//x_last=x(k-1|k-1),x_mid=x(k|k-1)
	data->p_mid = data->p_last + data->Q;							//p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
	data->kg = data->p_mid / (data->p_mid + data->R);				//kg为kalman filter，R为噪声
	data->x_now = data->x_mid + data->kg*(measure - data->x_mid);	//估计出的最优值
	data->p_now = (1 - data->kg)*data->p_mid;						//最优值对应的covariance
	data->p_last = data->p_now;										//更新covariance值
	data->x_last = data->x_now;										//更新系统状态值
	return data->x_now;
}

float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;							//   evil floating   point  bit level   hacking   
	i = 0x5f3759df - (i >> 1);				//   what   the   func?   
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));	//   1st   iteration   
	y = y * (threehalfs - (x2 * y * y));	//   2nd   iteration,   this   can   be removed   
	return y;
}