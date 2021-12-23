/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
#include "gsm.h"
#include "com.h"
#include "rs485.h"
#include "analog.h"
#include "digital.h"
#include "spi_interface.h"
#include "usart_ring.h"
#include "logic_func.h"
#include "cJSON.h"
#include "input_JSON.h"
#include "input_data.h"

#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLAG_MCU_Pin GPIO_PIN_6
#define FLAG_MCU_GPIO_Port GPIOE
#define PWRON_Pin GPIO_PIN_13
#define PWRON_GPIO_Port GPIOC
#define CS__M_Pin GPIO_PIN_0
#define CS__M_GPIO_Port GPIOC
#define MISO_M_Pin GPIO_PIN_2
#define MISO_M_GPIO_Port GPIOC
#define MOSI_M_Pin GPIO_PIN_3
#define MOSI_M_GPIO_Port GPIOC
#define O0_Pin GPIO_PIN_0
#define O0_GPIO_Port GPIOA
#define AIN0_Pin GPIO_PIN_3
#define AIN0_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_4
#define AIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_5
#define AIN2_GPIO_Port GPIOA
#define AIN3_Pin GPIO_PIN_6
#define AIN3_GPIO_Port GPIOA
#define O1_Pin GPIO_PIN_0
#define O1_GPIO_Port GPIOB
#define O2_Pin GPIO_PIN_1
#define O2_GPIO_Port GPIOB
#define O3_Pin GPIO_PIN_7
#define O3_GPIO_Port GPIOE
#define O4_Pin GPIO_PIN_8
#define O4_GPIO_Port GPIOE
#define O5_Pin GPIO_PIN_9
#define O5_GPIO_Port GPIOE
#define O6_Pin GPIO_PIN_10
#define O6_GPIO_Port GPIOE
#define O7_Pin GPIO_PIN_11
#define O7_GPIO_Port GPIOE
#define S2_Pin GPIO_PIN_12
#define S2_GPIO_Port GPIOE
#define S3_Pin GPIO_PIN_13
#define S3_GPIO_Port GPIOE
#define S4_Pin GPIO_PIN_14
#define S4_GPIO_Port GPIOE
#define S1_Pin GPIO_PIN_15
#define S1_GPIO_Port GPIOE
#define SCK_M_Pin GPIO_PIN_10
#define SCK_M_GPIO_Port GPIOB
#define IN7_Pin GPIO_PIN_8
#define IN7_GPIO_Port GPIOD
#define IN6_Pin GPIO_PIN_9
#define IN6_GPIO_Port GPIOD
#define IN5_Pin GPIO_PIN_10
#define IN5_GPIO_Port GPIOD
#define IN4_Pin GPIO_PIN_11
#define IN4_GPIO_Port GPIOD
#define IN3_Pin GPIO_PIN_12
#define IN3_GPIO_Port GPIOD
#define IN2_Pin GPIO_PIN_13
#define IN2_GPIO_Port GPIOD
#define IN1_Pin GPIO_PIN_14
#define IN1_GPIO_Port GPIOD
#define IN0_Pin GPIO_PIN_15
#define IN0_GPIO_Port GPIOD
#define PWM0_Pin GPIO_PIN_6
#define PWM0_GPIO_Port GPIOC
#define PWM1_Pin GPIO_PIN_7
#define PWM1_GPIO_Port GPIOC
#define PWM2_Pin GPIO_PIN_8
#define PWM2_GPIO_Port GPIOC
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOC
#define SIM_RxD_Pin GPIO_PIN_9
#define SIM_RxD_GPIO_Port GPIOA
#define SIM_TxD_Pin GPIO_PIN_10
#define SIM_TxD_GPIO_Port GPIOA
#define PKEY_Pin GPIO_PIN_11
#define PKEY_GPIO_Port GPIOA
#define COM_RxD_Pin GPIO_PIN_10
#define COM_RxD_GPIO_Port GPIOC
#define COM_TxD_Pin GPIO_PIN_11
#define COM_TxD_GPIO_Port GPIOC
#define WR0_Pin GPIO_PIN_0
#define WR0_GPIO_Port GPIOD
#define WR1_Pin GPIO_PIN_1
#define WR1_GPIO_Port GPIOD
#define PWR0_Pin GPIO_PIN_2
#define PWR0_GPIO_Port GPIOD
#define PWR1_Pin GPIO_PIN_3
#define PWR1_GPIO_Port GPIOD
#define RS485_DE_Pin GPIO_PIN_4
#define RS485_DE_GPIO_Port GPIOD
#define RS485_RxD_Pin GPIO_PIN_5
#define RS485_RxD_GPIO_Port GPIOD
#define RS485_TxD_Pin GPIO_PIN_6
#define RS485_TxD_GPIO_Port GPIOD
#define RS485_RE__Pin GPIO_PIN_7
#define RS485_RE__GPIO_Port GPIOD
#define SCK_Pin GPIO_PIN_3
#define SCK_GPIO_Port GPIOB
#define MISO_Pin GPIO_PIN_4
#define MISO_GPIO_Port GPIOB
#define MOSI_Pin GPIO_PIN_5
#define MOSI_GPIO_Port GPIOB
#define CS1__Pin GPIO_PIN_6
#define CS1__GPIO_Port GPIOB
#define CD_Pin GPIO_PIN_7
#define CD_GPIO_Port GPIOB
#define WP__Pin GPIO_PIN_0
#define WP__GPIO_Port GPIOE
#define CS2__Pin GPIO_PIN_1
#define CS2__GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define RS485_Tx	GPIOD->ODR |= GPIO_ODR_ODR_4 | GPIO_ODR_ODR_7		//конфа на передачу DE = 1,RE# = 1;
#define RS485_Rx	GPIOD->ODR &= ~(GPIO_ODR_ODR_4 | GPIO_ODR_ODR_7)	//конфа на прием DE = 0,RE# = 0;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
