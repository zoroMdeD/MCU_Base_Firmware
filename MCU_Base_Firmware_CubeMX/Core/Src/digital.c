/*
 * digital_ini.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "digital.h"

extern GPIO_InitTypeDef GPIO_InitStruct;

uint8_t flag_it = 0;
uint8_t flag_setup = 0;

//Инициализация переменной структуры
DINn_ptr DINn;

void DINn_ini(void)
{
	DINn.DIN0_interrupt = 0;
	DINn.DIN1_interrupt = 0;
	DINn.DIN2_interrupt = 0;
	DINn.DIN3_interrupt = 0;
	DINn.DIN4_interrupt = 0;
	DINn.DIN5_interrupt = 0;
	DINn.DIN6_interrupt = 0;
	DINn.DIN7_interrupt = 0;
}
//Фунуция перенастройки режима прерывания входов
//Принимает флаг изменения вида прерываний: flag_it
//Принимает флаг изменения настройки конкретного входа прерываний: flag_setup
void ReINIT_DIGITAL(uint8_t flag_it, uint8_t flag_setup)
{
	if(flag_setup = 1)
	{
		GPIO_InitStruct.Pin = IN0_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 2)
	{
		GPIO_InitStruct.Pin = IN1_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 3)
	{
		GPIO_InitStruct.Pin = IN2_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 4)
	{
		GPIO_InitStruct.Pin = IN3_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 5)
	{
		GPIO_InitStruct.Pin = IN4_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 6)
	{
		GPIO_InitStruct.Pin = IN5_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 7)
	{
		GPIO_InitStruct.Pin = IN6_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(flag_setup = 8)
	{
		GPIO_InitStruct.Pin = IN7_Pin;
		if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(flag_it)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}
//Функция обработки прерываний						!!!!!Нужно где-то сбросить эти флаги
void DINn_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_8)		//DIN7
	{
		DINn.DIN0_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_9)	//DIN6
	{
		DINn.DIN1_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_10)	//DIN5
	{
		DINn.DIN2_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_11)	//DIN4
	{
		DINn.DIN3_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_12)	//DIN3
	{
		DINn.DIN4_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_13)	//DIN2
	{
		DINn.DIN5_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_14)	//DIN1
	{
		DINn.DIN6_interrupt = 1;
	}
	else if (GPIO_Pin == GPIO_PIN_15)	//DIN0
	{
		DINn.DIN7_interrupt = 1;
	}
	else
	{
		__NOP();
	}
}











