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
#include "enocoro.h"
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t keystream[STRING_LENGTH] = {0};
ENOCORO_Ctx ctx;
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

CMSE_NS_ENTRY void key_stream(uint8_t* key, uint8_t* iv){
	uint8_t key_temp[16];
	uint8_t iv_temp[16];
	memcpy(key_temp, key, 16);
	memcpy(iv_temp, iv, 16);

	memset(&ctx, 0, sizeof(ENOCORO_Ctx));

	/* setup init */
	ENOCORO_init(&ctx, key_temp, ENOCORO128_KEY_BYTE_SIZE, iv_temp, ENOCORO_IV_BYTE_SIZE);

	/* output randum value */
	ENOCORO_keystream(&ctx, keystream, STRING_LENGTH);

	return;
}

CMSE_NS_ENTRY void encrypt_decrypt(uint8_t* plain){
	uint8_t plain_temp[STRING_LENGTH];
	memcpy(plain_temp, plain, STRING_LENGTH);

	for (int i = 0; i < STRING_LENGTH; i++)
		plain_temp[i] ^= keystream[i];

	memcpy(plain, plain_temp, STRING_LENGTH);
	return;
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

