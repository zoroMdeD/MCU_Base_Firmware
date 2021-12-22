/*
 * analog.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "main.h"

float Conv_ADC1(void)
{
	float Value = 0;
	float ADC_value = 0;
	const float Resolution = 0.0008056640625;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	ADC_value = HAL_ADC_GetValue(&hadc1);

	Value = (ADC_value * Resolution);
	//Value = (Value * 3.2323232323232);	//Напряжение
	Value = (Value / 60);		//Ток

	return Value;
}
uint16_t Get_ADC1(void)
{
	uint16_t ADC_value = 0;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	ADC_value = HAL_ADC_GetValue(&hadc1);

	return ADC_value;
}
