/*
 * input_data.c
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */
#include "input_data.h"
#include "stdio.h"

extern uint8_t Status_AIN[];	//Статус аналоговых входов
extern uint8_t Status_DIN[];	//Статус цифровых входов
extern uint8_t Status_PWM[];	//Статус выходов ШИМ
extern uint8_t Status_OCD[];	//Статус выходов открытый коллектор
extern uint8_t Status_1WR[];	//Статус выходов интерфейса 1-Wire

GPIO_TypeDef *varPortIN = 0;
GPIO_TypeDef *varPortOUT = 0;
uint16_t varPinIN = 0;
uint16_t varPinOUT = 0;

uint8_t NumOut = 0;

struct ScaningDIN_UpdateOCD
{
	GPIO_TypeDef *D_IN;
	uint8_t VAR_IN;
	uint16_t DIN_Pin;
	GPIO_TypeDef *D_OUT;
	uint8_t VAR_OUT;
	uint16_t OCD_Pin;
}DiDo[8];

void CheckReWrite()
{
	for(int i = 0; i < 8; i++)
	{
		if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) != DiDo[i].VAR_IN)	//(Если Вход1 == 0 то Выход3 = 1) Инверсная логика на входах оптопар
		{
			//Status_OCD[j] = DiDo[i].VAR_OUT;									//нужно записать в массив актуальные данные!!!
			HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, DiDo[i].VAR_OUT);
			//SEND_str("success...\n");
		}
		else if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) == DiDo[i].VAR_IN)
		{
			//Status_OCD[j] = !DiDo[i].VAR_OUT;
			HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, !DiDo[i].VAR_OUT);
			//SEND_str("miss...\n");
		}
	}
}

//Включить/выключить цифровой выход если цифровой вход = значение(уровень)
//Принимает "D_IN" - строку с номером цифрового входа
//Принимает "VAR_IN" - переменная состояния входа
//Принимает "D_OUT" - строку с номером цифрового выхода
//Принимает "VAR_OUT" - переменная состояния выхода
void set_dido(char *D_IN, uint8_t VAR_IN, char *D_OUT, uint8_t VAR_OUT)
{
	char VHOD[8][10] = {"VHOD1", "VHOD2", "VHOD3", "VHOD4", "VHOD5", "VHOD6", "VHOD7", "VHOD8"};
	char VIHOD[8][10] = {"VIHOD1", "VIHOD2", "VIHOD3", "VIHOD4", "VIHOD5", "VIHOD6", "VIHOD7", "VIHOD8"};

	uint16_t DIN_Pin[8] = {IN0_Pin, IN1_Pin, IN2_Pin, IN3_Pin, IN4_Pin, IN5_Pin, IN6_Pin, IN7_Pin};
	uint16_t OCD_Pin[8] = {O0_Pin, O1_Pin, O2_Pin, O3_Pin, O4_Pin, O5_Pin, O6_Pin, O7_Pin};

	GPIO_TypeDef *pVHOD[8] = {VHOD1, VHOD2, VHOD3, VHOD4, VHOD5, VHOD6, VHOD7, VHOD8};
	GPIO_TypeDef *pVIHOD[8] = {VIHOD1, VIHOD2, VIHOD3, VIHOD4, VIHOD5, VIHOD6, VIHOD7, VIHOD8};

	for(int i = 0; i < 8; i++)
	{
		if(strcmp(D_IN, VHOD[i]) == 0)
		{
			DiDo[i].D_IN = pVHOD[i];
			DiDo[i].VAR_IN = VAR_IN;
			DiDo[i].DIN_Pin = DIN_Pin[i];
			for(int j = 0; j < 8; j++)
			{
				if(strcmp(D_OUT, VIHOD[j]) == 0)
				{
					DiDo[i].D_OUT = pVIHOD[j];
					DiDo[i].VAR_OUT = VAR_OUT;
					DiDo[i].OCD_Pin = OCD_Pin[j];

					if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) != VAR_IN)	//(Если Вход1 == 0 то Выход3 = 1) Инверсная логика на входах оптопар
					{
						Status_OCD[j] = VAR_OUT;
						HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, VAR_OUT);
						SEND_str("success...\n");
					}
					else if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) == VAR_IN)
					{
						Status_OCD[j] = !VAR_OUT;
						HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, !VAR_OUT);
						SEND_str("miss...\n");
					}

					//----------------------------------For debuging----------------------------------
					if(DiDo[i].D_IN == VHOD1)
						SEND_str("1 - success...\n");
					if(DiDo[i].VAR_IN == VAR_IN)
						SEND_str("2 - success...\n");
					if(DiDo[i].D_OUT == VIHOD3)
						SEND_str("3 - success...\n");
					if(DiDo[i].VAR_OUT == VAR_OUT)
						SEND_str("4 - success...\n");
					//--------------------------------------------------------------------------------

					break;
				}
			}
			break;
		}
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
//Принимает "D_IN" - строку с номером цифрового входа
//Принимает "VAR_IN" - значение цифрового входа
//Принимает "A_OUT" - строку с номером аналогового выхода
//Принимает "VAR_OUT" - значение аналогового выхода
void set_diao(char *D_IN, uint8_t VAR_IN, char *A_OUT, uint8_t VAR_OUT)
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
