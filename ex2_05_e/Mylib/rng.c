#include "rng.h"

RNG_HandleTypeDef hrng;

void rng_init(void)
{
	__HAL_RCC_RNG_CLK_ENABLE();

	hrng.Instance = RNG;
	HAL_RNG_Init(&hrng);

	//HAL_NVIC_SetPriority(HASH_RNG_IRQn, 6, 0);
	//HAL_NVIC_EnableIRQ(HASH_RNG_IRQn);
}


uint8_t get_rng_u8(void)
{
	uint32_t rng;
	HAL_RNG_GenerateRandomNumber(&hrng, &rng);
	return (uint8_t)rng;
}

uint32_t get_rng_u32(void)
{
	uint32_t rng;
	HAL_RNG_GenerateRandomNumber(&hrng, &rng);
	return rng;
}


