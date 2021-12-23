/*
 * input_JSON.c
 *
 *  Created on: 24 нояб. 2020 г.
 *      Author: MMorozov
 */

#include "main.h"
#include <stdlib.h>
#include "cJSON.h"
#include "input_JSON.h"

extern uint8_t Status_DIN[8];	//Статус цифровых входов
extern uint8_t Status_AIN[8];	//Статус аналоговых входов
extern uint8_t Status_OCD[8];	//Статус выходов открытый коллектор
extern uint8_t Status_1WR[8];	//Статус выходов интерфейса 1-Wire
extern uint8_t Status_PWM[8];	//Статус выходов ШИМ

char *INSTRUCTION;		//Инструкция
char *TYPE;				//Комманда

//Переменные уловных выражений
char *D_IN;				//Цифровой вход
char *D_OUT;			//Цифровой выход
char *VAR_IN;			//Переменная уровня цифрового входа
char *VAR_OUT;			//Переменная уровня цифрового выхода
char *A_IN;				//Аналоговый вход
char *A_OUT;			//Аналоговый выход
char *RANGE_LOW;		//Переменная нижней границы аналогового входа
char *RANGE_HIGH;		//Переменная верхней границы аналогового выхода

//Массивы параметров
char *DigitalParamMass;					//Массив для значений цифровых выходов (8 параметров)
char *AnalogParamMass;					//Массив для значений аналоговых выходов (8 параметров)
char *OpenCollectorDrainParamMass;		//Массив для значений релейных выходов (8 параметров)
char *OneWireParamMass;					//Массив для значений 1-Wire выходов (2 параметра)
char *PWMParamMass;						//Массив для значений ШИМ выходов (4 параметра)

char *TIME;				//Переменная JSON системного времени сервера, для сравнения актуальности данных (если время с сервера меньше чем время у клиента, то данных не актуальны и игнорируем их) Timestamp
int Time_Server = 0;	//Переменная системного времени сервера
int Time_Client = 0;	//Переменная системного времени клиента

char paramValue[8];
char *test;


char a[32];
char b[32];
char c[32];
char d[32];

//Функция разбора подстроки значений параметра
//Принимает указатель на массив символов со значениями параметра
//Ворзвращает указатель на разобранный массив символов со значениями параметра
char *parseValue(char *value)
{
	char *tmp;
	for(int i = 1, j = 0; i < strlen(value); i++)
	{
		if(i % 2)
		{
			paramValue[j] = value[i];
			j++;
		}
	}
	tmp = paramValue;

	return tmp;
}
//Функция разбора полученной строки
void json_input(char *text)
{
	cJSON *json = cJSON_Parse(text);

	cJSON *stime = cJSON_GetObjectItem(json, "TIME");
	TIME = stime->valuestring;
	Time_Server = atoi(TIME);
	if(Time_Server > Time_Client)
	{
		cJSON *sInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
		INSTRUCTION = sInstruction->valuestring;

		if(strcmp(INSTRUCTION, "SET_PROGRAMM") == 0)
		{
			cJSON *sType = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "TYPE");
			TYPE = sType->valuestring;
			if(strcmp(TYPE, "SET_DIDO") == 0)	//Включить/выключить цифровой выход если цифровой вход = значение(уровень)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "D_IN");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VAR_IN");
				cJSON *s3 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "D_OUT");
				cJSON *s4 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VAR_OUT");

				D_IN = s1->valuestring;
				VAR_IN = s2->valuestring;
				D_OUT = s3->valuestring;
				VAR_OUT = s4->valuestring;

				//parseValue(a, D_IN);
				for(int i = 1, j = 0; i < strlen(D_IN); i++)
				{
					if(i % 2)
					{
						a[j] = D_IN[i];
						j++;
					}
				}
				//parseValue(b, VAR_IN);
				for(int i = 1, j = 0; i < strlen(VAR_IN); i++)
				{
					if(i % 2)
					{
						b[j] = VAR_IN[i];
						j++;
					}
				}
				//parseValue(c, D_OUT);
				for(int i = 1, j = 0; i < strlen(D_OUT); i++)
				{
					if(i % 2)
					{
						c[j] = D_OUT[i];
						j++;
					}
				}
				//parseValue(d, VAR_OUT);
				for(int i = 1, j = 0; i < strlen(VAR_OUT); i++)
				{
					if(i % 2)
					{
						d[j] = VAR_OUT[i];
						j++;
					}
				}

				set_dido(D_IN, (VAR_IN - 0x30), D_OUT, (VAR_OUT - 0x30));

				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(D_IN);
				SEND_str("\n");
				SEND_str(VAR_IN);
				SEND_str("\n");
				SEND_str(D_OUT);
				SEND_str("\n");
				SEND_str(VAR_OUT);
				SEND_str("\n");

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
				free(s3);
				free(s4);
			}
			else if(strcmp(TYPE, "SET_AIDO") == 0)	//Включить/выключить один цифровой выход если аналоговый вход в интервале значений
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "A_IN");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_LOW");
				cJSON *s3 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_HIGH");
				cJSON *s4 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "D_OUT");
				cJSON *s5 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VAR_OUT");
				A_IN = s1->valuestring;
				RANGE_LOW = s2->valuestring;
				RANGE_HIGH = s3->valuestring;
				D_OUT = s4->valuestring;
				VAR_OUT = s5->valuestring;

				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(A_IN);
				SEND_str("\n");
				SEND_str(RANGE_LOW);
				SEND_str("\n");
				SEND_str(RANGE_HIGH);
				SEND_str("\n");
				SEND_str(D_OUT);
				SEND_str("\n");
				SEND_str(VAR_OUT);
				SEND_str("\n");

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
				free(s3);
				free(s4);
				free(s5);
			}
			else if(strcmp(TYPE, "SET_AIAO") == 0)	//Установить значение в аналоговый выход если аналоговый вход = значение
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "A_IN");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_LOW");
				cJSON *s3 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_HIGH");
				cJSON *s4 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "A_OUT");
				cJSON *s5 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VAR_OUT");
				A_IN = s1->valuestring;
				RANGE_LOW = s2->valuestring;
				RANGE_HIGH = s3->valuestring;
				A_OUT = s4->valuestring;
				VAR_OUT = s5->valuestring;

				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(A_IN);
				SEND_str("\n");
				SEND_str(RANGE_LOW);
				SEND_str("\n");
				SEND_str(RANGE_HIGH);
				SEND_str("\n");
				SEND_str(A_OUT);
				SEND_str("\n");
				SEND_str(VAR_OUT);
				SEND_str("\n");

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
				free(s3);
				free(s4);
				free(s5);
			}
			else
			{
				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
			}
		}
		else if(strcmp(INSTRUCTION, "SET_PERIPHERALS") == 0)
		{
			cJSON *sType = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "TYPE");
			TYPE = sType->valuestring;
			if(strcmp(TYPE, "DIGITAL") == 0)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "SET");
				DigitalParamMass = s1->valuestring;

				test = parseValue(DigitalParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_DIN[i] = (test[i] - 0x30);
				}

				//-------------------------For testing-------------------------
				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				//-------------------------------------------------------------

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
			}
			else if(strcmp(TYPE, "ANALOG") == 0)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "SET");
				AnalogParamMass = s1->valuestring;

				test = parseValue(AnalogParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_AIN[i] = (test[i] - 0x30);
				}

				//-------------------------For testing-------------------------
				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				//-------------------------------------------------------------

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
			}
			else if(strcmp(TYPE, "OCD") == 0)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "SET");

				OpenCollectorDrainParamMass = s1->valuestring;

				test = parseValue(OpenCollectorDrainParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_OCD[i] = (test[i] - 0x30);
				}

				//-------------------------For testing-------------------------
				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				//-------------------------------------------------------------

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
			}
//			else if(strcmp(TYPE, "OneWire") == 0)
//			{
//				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "SET");
//
//				OpenCollectorDrainParamMass = s1->valuestring;
//
//				test = parseValue(OpenCollectorDrainParamMass);
//
//				for(int i = 0; i < 8; i++)
//				{
//					Status_OCD[i] = (test[i] - 0x30);
//				}
//
//				//-------------------------For testing-------------------------
//				SEND_str("\n");
//				SEND_str(TYPE);
//				SEND_str("\n");
//				//-------------------------------------------------------------
//
//				cJSON_Delete(json);
//				free(stime);
//				free(sInstruction);
//				free(sType);
//				free(s1);
//			}
//			else
//			{
//				cJSON_Delete(json);
//				free(stime);
//				free(sInstruction);
//				free(sType);
//			}
		}
		else
		{

			free(stime);
			free(sInstruction);
		}

	}
	else
	{
		cJSON_Delete(json);
		free(stime);
	}
}
