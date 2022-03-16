/*
 * input_data.h
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */

#ifndef INC_DIGITAL_SEND_H_
#define INC_DIGITAL_SEND_H_

#include "main.h"

void CheckReWrite();
void set_dido(char *D_IN, uint8_t VAR_IN, char *D_OUT, uint8_t VAR_OUT);
void set_vaido(char *A_IN, double RANGE_LOW, double RANGE_HIGH, char *D_OUT, uint8_t VAR_OUT);

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

#define SelectChannelOne	HAL_GPIO_WritePin(GPIOE, S1_Pin, SET)
#define SelectChannelTwo	HAL_GPIO_WritePin(GPIOE, S2_Pin, SET)
#define SelectChannelThree	HAL_GPIO_WritePin(GPIOE, S3_Pin, SET)
#define SelectChannelFour	HAL_GPIO_WritePin(GPIOE, S4_Pin, SET)

#endif /* INC_DIGITAL_SEND_H_ */
