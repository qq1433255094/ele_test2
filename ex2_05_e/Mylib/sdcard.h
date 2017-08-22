/**
  ******************************************************************************
  * @file    stm324x9i_eval_sd.h
  * @author  MCD Application Team
  * @version V2.2.3
  * @date    22-April-2016
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm324x9i_eval_sd.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDCARD_H
#define __SDCARD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sd.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */
    
/** @addtogroup STM324x9I_EVAL_SD
  * @{
  */    

/** @defgroup STM324x9I_EVAL_SD_Exported_Types STM324x9I EVAL SD Exported Types
  * @{
  */

/** 
  * @brief SD Card information structure 
  */
#define SD_CardInfo HAL_SD_CardInfoTypeDef
/**
  * @}
  */
   
/** 
  * @brief  SD status structure definition  
  */     
#define   MSD_OK         0x00
#define   MSD_ERROR      0x01
   
/** @defgroup STM324x9I_EVAL_SD_Exported_Constants STM324x9I EVAL SD Exported Constants
  * @{
  */ 
#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

#define SD_DATATIMEOUT           ((uint32_t)100000000)
    
/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __DMA2_CLK_ENABLE
#define SD_DMAx_Tx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM                 DMA2_Stream6  
#define SD_DMAx_Rx_STREAM                 DMA2_Stream3  
#define SD_DMAx_Tx_IRQn                   DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Stream3_IRQn
#define SD_DMAx_Tx_IRQHandler             DMA2_Stream6_IRQHandler   
#define SD_DMAx_Rx_IRQHandler             DMA2_Stream3_IRQHandler 
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)
/**
  * @}
  */
  
/** @defgroup STM324x9I_EVAL_SD_Exported_Macro STM324x9I EVAL SD Exported Macro
  * @{
  */ 
/**
  * @}
  */
   
/** @defgroup STM324x9I_EVAL_SD_Exported_Functions STM324x9I EVAL SD Exported Functions
  * @{
  */   
	 typedef enum
	 {
		 SD_TRANSFER_OK = 0U,  /*!< Transfer success      */
		 SD_TRANSFER_BUSY = 1U,  /*!< Transfer is occurring */
		 SD_TRANSFER_ERROR = 2U   /*!< Transfer failed       */

	 }HAL_SD_TransferStateTypedef;

	 typedef enum
	 {
		 /**
		 * @brief  SD specific error defines
		 */
		 SD_CMD_CRC_FAIL = (1U),   /*!< Command response received (but CRC check failed)              */
		 SD_DATA_CRC_FAIL = (2U),   /*!< Data block sent/received (CRC check failed)                   */
		 SD_CMD_RSP_TIMEOUT = (3U),   /*!< Command response timeout                                      */
		 SD_DATA_TIMEOUT = (4U),   /*!< Data timeout                                                  */
		 SD_TX_UNDERRUN = (5U),   /*!< Transmit FIFO underrun                                        */
		 SD_RX_OVERRUN = (6U),   /*!< Receive FIFO overrun                                          */
		 SD_START_BIT_ERR = (7U),   /*!< Start bit not detected on all data signals in wide bus mode   */
		 SD_CMD_OUT_OF_RANGE = (8U),   /*!< Command's argument was out of range.                          */
		 SD_ADDR_MISALIGNED = (9U),   /*!< Misaligned address                                            */
		 SD_BLOCK_LEN_ERR = (10U),  /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
		 SD_ERASE_SEQ_ERR = (11U),  /*!< An error in the sequence of erase command occurs.            */
		 SD_BAD_ERASE_PARAM = (12U),  /*!< An invalid selection for erase groups                        */
		 SD_WRITE_PROT_VIOLATION = (13U),  /*!< Attempt to program a write protect block                     */
		 SD_LOCK_UNLOCK_FAILED = (14U),  /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
		 SD_COM_CRC_FAILED = (15U),  /*!< CRC check of the previous command failed                     */
		 SD_ILLEGAL_CMD = (16U),  /*!< Command is not legal for the card state                      */
		 SD_CARD_ECC_FAILED = (17U),  /*!< Card internal ECC was applied but failed to correct the data */
		 SD_CC_ERROR = (18U),  /*!< Internal card controller error                               */
		 SD_GENERAL_UNKNOWN_ERROR = (19U),  /*!< General or unknown error                                     */
		 SD_STREAM_READ_UNDERRUN = (20U),  /*!< The card could not sustain data transfer in stream read operation. */
		 SD_STREAM_WRITE_OVERRUN = (21U),  /*!< The card could not sustain data programming in stream mode   */
		 SD_CID_CSD_OVERWRITE = (22U),  /*!< CID/CSD overwrite error                                      */
		 SD_WP_ERASE_SKIP = (23U),  /*!< Only partial address space was erased                        */
		 SD_CARD_ECC_DISABLED = (24U),  /*!< Command has been executed without using internal ECC         */
		 SD_ERASE_RESET = (25U),  /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
		 SD_AKE_SEQ_ERROR = (26U),  /*!< Error in sequence of authentication.                         */
		 SD_INVALID_VOLTRANGE = (27U),
		 SD_ADDR_OUT_OF_RANGE = (28U),
		 SD_SWITCH_ERROR = (29U),
		 SD_SDIO_DISABLED = (30U),
		 SD_SDIO_FUNCTION_BUSY = (31U),
		 SD_SDIO_FUNCTION_FAILED = (32U),
		 SD_SDIO_UNKNOWN_FUNCTION = (33U),

		 /**
		 * @brief  Standard error defines
		 */
		 SD_INTERNAL_ERROR = (34U),
		 SD_NOT_CONFIGURED = (35U),
		 SD_REQUEST_PENDING = (36U),
		 SD_REQUEST_NOT_APPLICABLE = (37U),
		 SD_INVALID_PARAMETER = (38U),
		 SD_UNSUPPORTED_FEATURE = (39U),
		 SD_UNSUPPORTED_HW = (40U),
		 SD_ERROR = (41U),
		 SD_OK = (0U)

	 }HAL_SD_ErrorTypedef;

uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ITConfig(void);
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
uint8_t BSP_SD_ReadBlocks(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_ReadBlocks_DMA(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
void    BSP_SD_IRQHandler(void);
void    BSP_SD_DMA_Tx_IRQHandler(void);
void    BSP_SD_DMA_Rx_IRQHandler(void);
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);
uint8_t BSP_SD_IsDetected(void); 
   
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM324x9I_EVAL_SD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
