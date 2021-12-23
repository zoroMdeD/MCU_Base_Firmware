/*
 * input_data.c
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */
#include "input_data.h"

//Включить/выключить цифровой выход если цифровой вход = значение(уровень)
//Принимает "d_vhod" - строку с номером цифрового входа
//Принимает "data_a" - переменная состояния входа
//Принимает "d_vihod" - строку с номером цифрового выхода
//Принимает "data_b" - переменная состояния выхода
void set_dido(char *d_vhod, uint8_t data_a, char *d_vihod, uint8_t data_b)
{
	GPIO_TypeDef *varPortIN = 0;
	GPIO_TypeDef *varPortOUT = 0;
	uint16_t varPinIN = 0;
	uint16_t varPinOUT = 0;

	if(strcmp(d_vhod, "VHOD1") == 0){varPortIN = VHOD1; varPinIN = IN0_Pin;}
	else if(strcmp(d_vhod, "VHOD2") == 0){varPortIN = VHOD2; varPinIN = IN1_Pin;}
	else if(strcmp(d_vhod, "VHOD3") == 0){varPortIN = VHOD3; varPinIN = IN2_Pin;}
	else if(strcmp(d_vhod, "VHOD4") == 0){varPortIN = VHOD4; varPinIN = IN3_Pin;}
	else if(strcmp(d_vhod, "VHOD5") == 0){varPortIN = VHOD5; varPinIN = IN4_Pin;}
	else if(strcmp(d_vhod, "VHOD6") == 0){varPortIN = VHOD6; varPinIN = IN5_Pin;}
	else if(strcmp(d_vhod, "VHOD7") == 0){varPortIN = VHOD7; varPinIN = IN6_Pin;}
	else if(strcmp(d_vhod, "VHOD8") == 0){varPortIN = VHOD8; varPinIN = IN7_Pin;}

	if(strcmp(d_vihod, "VIHOD1") == 0){varPortOUT = VIHOD1; varPinOUT = O0_Pin;}
	else if(strcmp(d_vihod, "VIHOD2") == 0){varPortOUT = VIHOD2; varPinOUT = O1_Pin;}
	else if(strcmp(d_vihod, "VIHOD3") == 0){varPortOUT = VIHOD3; varPinOUT = O2_Pin;}
	else if(strcmp(d_vihod, "VIHOD4") == 0){varPortOUT = VIHOD4; varPinOUT = O3_Pin;}
	else if(strcmp(d_vihod, "VIHOD5") == 0){varPortOUT = VIHOD5; varPinOUT = O4_Pin;}
	else if(strcmp(d_vihod, "VIHOD6") == 0){varPortOUT = VIHOD6; varPinOUT = O5_Pin;}
	else if(strcmp(d_vihod, "VIHOD7") == 0){varPortOUT = VIHOD7; varPinOUT = O6_Pin;}
	else if(strcmp(d_vihod, "VIHOD8") == 0){varPortOUT = VIHOD8; varPinOUT = O7_Pin;}



	if(HAL_GPIO_ReadPin(varPortIN, varPinIN) == 0)
	{
		HAL_GPIO_WritePin(varPortOUT, varPinOUT, data_b);
		SEND_str("if != 1");
		SEND_str("\n");
	}
	else
	{
		HAL_GPIO_WritePin(varPortOUT, varPinOUT, RESET);
		SEND_str("nothing");
		SEND_str("\n");
	}
}
//Включить/выключить один цифровой выход если аналоговый вход в интервале значений
//Принимает "a_vhod" - строку с номером аналогового входа
//Принимает "data_low" - нижний предел значений
//Принимает "data_high" - верхний предел значений
//Принимает "d_vihod" - строку с номером цифрового выхода
//Принимает "data" - переменная состояния выхода
void set_aido(char *a_vhod, uint8_t data_low, uint8_t data_high, char *d_vihod, uint8_t data)
{

}
//Установить значение в аналоговый выход если цифровой вход = значение(уровень)
//Принимает "d_vihod" - строку с номером цифрового входа
//Принимает "d_data" - значение цифрового входа
//Принимает "a_vihod" - строку с номером аналогового выхода
//Принимает "a_data" - значение аналогового выхода
void set_diao(char *d_vhod, uint8_t d_data, char *a_vihod, uint8_t a_data)
{

}
//Установить значение в аналоговый выход если аналоговый вход = значение
//Принимает "a_vhod" - строку с номером аналогового входа
//Принимает "data_low" - нижний предел значений
//Принимает "data_high" - верхний предел значений
//Принимает "a_vihod" - строку с номером аналогового выхода
//Принимает "data" - значение аналогового выхода
void set_aiao(char *a_vhod, uint8_t data_low, uint8_t data_high, char *a_vihod, uint8_t data)
{

}


void set_data(void)
{

}
