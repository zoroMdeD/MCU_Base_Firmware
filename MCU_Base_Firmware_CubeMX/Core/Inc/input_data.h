/*
 * input_data.h
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */

#ifndef INC_DIGITAL_SEND_H_
#define INC_DIGITAL_SEND_H_

#include "main.h"

void CheckReWriteDiDo(void);
void CheckReWriteVAiDo(void);
void CheckReWriteTSiDo(void);
void set_dido(char *D_IN, uint8_t VAR_IN, char *D_OUT, uint8_t VAR_OUT);
void set_vaido(char *A_IN, double RANGE_LOW, double RANGE_HIGH, char *D_OUT, uint8_t VAR_OUT);
void set_pwm(char *PWM_OUT, uint32_t DUTY_CYCLE);
void set_temperature(char *ROM_RAW, double RANGE_TEMP_LOW, double RANGE_TEMP_HIGH, char *D_OUT, uint8_t VAR_OUT);

#define VHOD1	GPIOD
#define VHOD2	GPIOD
#define VHOD3	GPIOD
#define VHOD4	GPIOD
#define VHOD5	GPIOD
#define VHOD6	GPIOD
#define VHOD7	GPIOD
#define VHOD8	GPIOD

#define VIHOD1	GPIOA
#define VIHOD2	GPIOB
#define VIHOD3	GPIOB
#define VIHOD4	GPIOE
#define VIHOD5	GPIOE
#define VIHOD6	GPIOE
#define VIHOD7	GPIOE
#define VIHOD8	GPIOE

#define ADC1_IN3	1
#define ADC1_IN4	2
#define ADC1_IN5	3
#define ADC1_IN6	4
/*Таблица истонности переключения аналогового комутатора
 * S1_Pin = 0 - Включено измерение тока на 3-ем канале измерения ADC1_IN3 || S1_Pin = 1 - Включено измерение напряжения на 3-ем канале измерения ADC1_IN3
 * S2_Pin = 0 - Включено измерение тока на 4-ом канале измерения ADC1_IN4 || S2_Pin = 1 - Включено измерение напряжения на 4-ом канале измерения ADC1_IN4
 * S3_Pin = 0 - Включено измерение тока на 5-ом канале измерения ADC1_IN5 || S3_Pin = 1 - Включено измерение напряжения на 5-ом канале измерения ADC1_IN5
 * S4_Pin = 0 - Включено измерение тока на 6-ом канале измерения ADC1_IN6 || S4_Pin = 1 - Включено измерение напряжения на 6-ом канале измерения ADC1_IN6
 */
#define SelectChannelOne(x)		HAL_GPIO_WritePin(GPIOE, S1_Pin, x)
#define SelectChannelTwo(x)		HAL_GPIO_WritePin(GPIOE, S2_Pin, x)
#define SelectChannelThree(x)	HAL_GPIO_WritePin(GPIOE, S3_Pin, x)
#define SelectChannelFour(x)	HAL_GPIO_WritePin(GPIOE, S4_Pin, x)

#endif /* INC_DIGITAL_SEND_H_ */
