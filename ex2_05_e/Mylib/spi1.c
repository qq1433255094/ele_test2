#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  
#include "stm32f4xx_hal_rcc.h"

#include "spi1.h"

SPI_HandleTypeDef hspi1;

/*
The SPI HAL driver can be used as follows:

      (#) Declare a SPI_HandleTypeDef handle structure, for example:
          SPI_HandleTypeDef  hspi;

      (#)Initialize the SPI low level resources by implementing the HAL_SPI_MspInit() API:
          (##) Enable the SPIx interface clock
          (##) SPI pins configuration
              (+++) Enable the clock for the SPI GPIOs
              (+++) Configure these SPI pins as alternate function push-pull
          (##) NVIC configuration if you need to use interrupt process
              (+++) Configure the SPIx interrupt priority
              (+++) Enable the NVIC SPI IRQ handle
          (##) DMA Configuration if you need to use DMA process
              (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive stream
              (+++) Enable the DMAx clock
              (+++) Configure the DMA handle parameters
              (+++) Configure the DMA Tx or Rx stream
              (+++) Associate the initialized hdma_tx handle to the hspi DMA Tx or Rx handle
              (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx or Rx stream

      (#) Program the Mode, BidirectionalMode , Data size, Baudrate Prescaler, NSS
          management, Clock polarity and phase, FirstBit and CRC configuration in the hspi Init structure.

      (#) Initialize the SPI registers by calling the HAL_SPI_Init() API:
          (++) This API configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
              by calling the customized HAL_SPI_MspInit() API.
     [..]
       Circular mode restriction:
      (#) The DMA circular mode cannot be used when the SPI is configured in these modes:
          (##) Master 2Lines RxOnly
          (##) Master 1Line Rx
      (#) The CRC feature is not managed when the DMA circular mode is enabled
      (#) When the SPI DMA Pause/Stop features are used, we must use the following APIs
          the HAL_SPI_DMAPause()/ HAL_SPI_DMAStop() only under the SPI callbacks
     [..]
       Master Receive mode restriction:
      (#) In Master unidirectional receive-only mode (MSTR =1, BIDIMODE=0, RXONLY=0) or
          bidirectional receive mode (MSTR=1, BIDIMODE=1, BIDIOE=0), to ensure that the SPI
          does not initiate a new transfer the following procedure has to be respected:
          (##) HAL_SPI_DeInit()
          (##) HAL_SPI_Init()

*/



void AccelInit(void)
{
	GPIO_InitTypeDef GPIO_Init;
	
	__GPIOE_CLK_ENABLE();
	
	GPIO_Init.Pin   = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull  = GPIO_PULLDOWN;
	GPIO_Init.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_Init);	
	Accel_CS_HIGH();
	Si4332_CS_HIGH();
}

void Si4332Init(void)
{
	GPIO_InitTypeDef GPIO_Init;
	
	__GPIOE_CLK_ENABLE();
	
	GPIO_Init.Pin   = GPIO_PIN_3;
	GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull  = GPIO_PULLDOWN;
	GPIO_Init.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_Init);	
	Accel_CS_HIGH();
}

HAL_StatusTypeDef spi1_init(void)
{
	//init gpio
	GPIO_InitTypeDef GPIO_Init;
	HAL_StatusTypeDef state;
	
	__SPI1_CLK_ENABLE(); //(##) Enable the SPIx interface clock. 
												
	__GPIOA_CLK_ENABLE();//(##) I2S pins configuration:
	
	GPIO_Init.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Init.Mode  = GPIO_MODE_AF_PP;
	GPIO_Init.Pull  = GPIO_PULLDOWN;
	GPIO_Init.Speed = GPIO_SPEED_MEDIUM;
	GPIO_Init.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);		
	
	AccelInit();
	
	hspi1.Instance = SPI1;
	
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
	hspi1.Init.Mode = SPI_MODE_MASTER;

	state=HAL_SPI_Init(&hspi1);
	
	return state;
}

void SPIx_Error(void)
{
	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); 
}

#define SPI_TIMEOUT 100

uint8_t SPI1_WriteRead(uint8_t Byte)
{
	uint8_t receivedbyte = 0;
  
	/* Send a Byte through the SPI peripheral */
	/* Read byte from the SPI bus */
	if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, SPI_TIMEOUT) != HAL_OK)
	{
		SPIx_Error();
	}
  
	return receivedbyte;
}


