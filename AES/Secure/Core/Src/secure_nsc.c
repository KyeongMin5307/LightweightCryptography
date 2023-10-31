/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"
#include "aes.h"
/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STRING_LENGTH 64
#define NUM_ENC 4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct AES_ctx ctx;
uint8_t key_tz[16];
uint8_t cipher[64];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

CMSE_NS_ENTRY void init_ctx(uint8_t* key){
	memcpy(key_tz, key, sizeof(key_tz));
	AES_init_ctx(&ctx, key_tz);
	return;
}

CMSE_NS_ENTRY void encrypt(uint8_t* plain, uint8_t* cipher){
	uint8_t temp[STRING_LENGTH];
	memcpy(temp, plain, STRING_LENGTH);

	for (int i = 0; i < NUM_ENC; ++i)
		AES_ECB_encrypt(&ctx, temp + (i * 16));

	memcpy(cipher, temp, STRING_LENGTH);
	return;
}

CMSE_NS_ENTRY void decrypt(uint8_t* cipher, uint8_t* plain){
	uint8_t temp[STRING_LENGTH];
	memcpy(temp, cipher, STRING_LENGTH);

	for (int i = 0; i < NUM_ENC; ++i)
		AES_ECB_encrypt(&ctx, temp + (i * 16));

	memcpy(plain, temp, STRING_LENGTH);
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

