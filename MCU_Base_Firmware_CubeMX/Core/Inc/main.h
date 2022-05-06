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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
#include "temperature_sensors.h"
//--------------------------delay_ns--------------------------
#include "delay.h"
//#define DEMCR_TRCENA    0x01000000
//
///* Core Debug registers */
//#define DEMCR           (*((volatile uint32_t *)0xE000EDFC))
//#define DWT_CTRL        (*(volatile uint32_t *)0xe0001000)
//#define CYCCNTENA       (1<<0)
//#define DWT_CYCCNT      ((volatile uint32_t *)0xE0001004)
//#define CPU_CYCLES      *DWT_CYCCNT
//#define CLK_SPEED         168000000 // EXAMPLE for CortexM4, EDIT as needed
//
//#define STOPWATCH_START { m_nStart = *((volatile unsigned int *)0xE0001004);}
//#define STOPWATCH_STOP  { m_nStop = *((volatile unsigned int *)0xE0001004);}
//------------------------------------------------------------
//----------------------------FATfs---------------------------
#include "../../Core/fatfs/Inc/sd_cmd.h"
#include "../../Core/fatfs/Inc/spi_sd.h"
#include <stdarg.h>
#include "../fatfs/Inc/sd_main.h"
//------------------------------------------------------------

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
//---------------------------1-WIRE---------------------------
#define AMT_TEMP_SENS	8
//------------------------------------------------------------
/*Таблица истонности переключения аналогового комутатора
 * S1_Pin = 0 - Включено измерение тока на 3-ем канале измерения ADC1_IN3 || S1_Pin = 1 - Включено измерение напряжения на 3-ем канале измерения ADC1_IN3
 * S2_Pin = 0 - Включено измерение тока на 4-ом канале измерения ADC1_IN4 || S2_Pin = 1 - Включено измерение напряжения на 4-ом канале измерения ADC1_IN4
 * S3_Pin = 0 - Включено измерение тока на 5-ом канале измерения ADC1_IN5 || S3_Pin = 1 - Включено измерение напряжения на 5-ом канале измерения ADC1_IN5
 * S4_Pin = 0 - Включено измерение тока на 6-ом канале измерения ADC1_IN6 || S4_Pin = 1 - Включено измерение напряжения на 6-ом канале измерения ADC1_IN6
 */

//Digital Input(Discrete signals) Digital Output(Open Drain)
struct ScaningDIN_UpdateOCD
{
	GPIO_TypeDef *D_IN;		//Адрес порта оптопары, который читаем для изменения состояния на порту выхода OCD
	uint8_t VAR_IN;			//Значение которое должно быть "флагом" для выполнения условия по выходу OCD(установка значения, например переключение реле)
	uint16_t DIN_Pin;		//Адрес разряда порта(D_IN) с которого читаем значение(VAR_IN), которое должно удовлетворять условию для выполнения условия
	GPIO_TypeDef *D_OUT;	//Адрес порта открытого(ых) коллектора(ов), который(е) устанавливаем в состояние на порту выхода OCD(VAR_OUT)
	uint8_t VAR_OUT;		//Значение которое устанавливается если выполняется условие, которое установлено на вход(VAR_IN)
	uint16_t OCD_Pin;		//Адрес разряда порта(D_OUT) на который устанавливаем значение(VAR_OUT), которое например переключает реле
	bool clrFlag;			//Флаг использования переменной
}DiDo[8];

//Voltage Analog Input Digital Output(Open Drain)
struct ScaningVAIN_UpdateOCD
{
	uint8_t A_IN;			//Номер канала аналогового входа который считываем
	double RANGE_LOW;		//Нижнее значение на входе A_IN удовлетворяющее условию
	double RANGE_HIGH;		//Верхнее значение на входе A_IN удовлетворяющее условию
	GPIO_TypeDef *D_OUT;	//Адрес порта открытого(ых) коллектора(ов), который(е) устанавливаем в состояние на порту выхода OCD(VAR_OUT)
	uint8_t VAR_OUT;		//Значение которое устанавливается если выполняется условие, которое установлено на вход(VAR_IN)
	uint16_t OCD_Pin;		//Адрес разряда порта(D_OUT) на который устанавливаем значение(VAR_OUT), которое например переключает реле
	bool clrFlag;			//Флаг использования переменной
}VAiDo[4];

//Configuration PWM output(Open Drain)
struct UpdatePWM
{
	uint32_t PWM_Channel;	//канал генерации Ш�?М
	uint16_t D_CYCLE[1];	//коэффициент заполнения Ш�?М
	bool clrFlag;			//Флаг использования переменной
}PWM[4];

//Temperature sensors Input Digital Output(Open Drain)
struct ScaningTSIN_UpdateOCD
{
	uint8_t DEVICE_NUMBER;	//Порядковый номер датчика при инициализации
	char ROM_RAW[16];		//Уникальный идентификационный номер
	double RANGE_TEMP_LOW;	//Нижнее значение температуры датчика по ROM_RAW удовлетворяющее условию
	double RANGE_TEMP_HIGH;	//Верхнее значение температуры датчика по ROM_RAW удовлетворяющее условию
	GPIO_TypeDef *D_OUT;	//Адрес порта открытого(ых) коллектора(ов), который(е) устанавливаем в состояние на порту выхода OCD(VAR_OUT)
	uint8_t VAR_OUT;		//Значение которое устанавливается если температура в диапазоне значений
	uint16_t OCD_Pin;		//Адрес разряда порта(D_OUT) на который устанавливаем значение(VAR_OUT), которое например переключает реле
	bool clrFlag;			//Флаг использования переменной
}TSiDo[AMT_TEMP_SENS];

//Voltage Analog Input PWM Output(Open Drain)
//struct ScaningVAIN_UpdatePWM
//{
//	uint8_t A_IN;			//Номер канала аналогового входа который считываем
//	double RANGE_LOW;		//Нижнее значение на входе A_IN удовлетворяющее условию
//	double RANGE_HIGH;		//Верхнее значение на входе A_IN удовлетворяющее условию
//	GPIO_TypeDef *P_OUT;	//Адрес порта генерации Ш�?М
//	uint16_t PULSE_WIDTH;	//Значение длительности импульса
//	uint16_t PULSE_PERIOD;	//Значение периода импульсоа
//	bool clrFlag;			//Флаг использования переменной
//}VAiPo[4];
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
#define IN7_EXTI_IRQn EXTI9_5_IRQn
#define IN6_Pin GPIO_PIN_9
#define IN6_GPIO_Port GPIOD
#define IN6_EXTI_IRQn EXTI9_5_IRQn
#define IN5_Pin GPIO_PIN_10
#define IN5_GPIO_Port GPIOD
#define IN5_EXTI_IRQn EXTI15_10_IRQn
#define IN4_Pin GPIO_PIN_11
#define IN4_GPIO_Port GPIOD
#define IN4_EXTI_IRQn EXTI15_10_IRQn
#define IN3_Pin GPIO_PIN_12
#define IN3_GPIO_Port GPIOD
#define IN3_EXTI_IRQn EXTI15_10_IRQn
#define IN2_Pin GPIO_PIN_13
#define IN2_GPIO_Port GPIOD
#define IN2_EXTI_IRQn EXTI15_10_IRQn
#define IN1_Pin GPIO_PIN_14
#define IN1_GPIO_Port GPIOD
#define IN1_EXTI_IRQn EXTI15_10_IRQn
#define IN0_Pin GPIO_PIN_15
#define IN0_GPIO_Port GPIOD
#define IN0_EXTI_IRQn EXTI15_10_IRQn
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
//---------------------------RS-485---------------------------
#define RS485_Tx	GPIOD->ODR |= GPIO_ODR_ODR_4 | GPIO_ODR_ODR_7		//конфа на передачу DE = 1,RE# = 1;
#define RS485_Rx	GPIOD->ODR &= ~(GPIO_ODR_ODR_4 | GPIO_ODR_ODR_7)	//конфа на прием DE = 0,RE# = 0;
//------------------------------------------------------------
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
