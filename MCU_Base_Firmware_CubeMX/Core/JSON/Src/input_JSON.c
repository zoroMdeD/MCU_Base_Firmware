/*
 * input_JSON.c
 *
 *  Created on: 24 нояб. 2020 г.
 *      Author: MMorozov
 */
#include "main.h"
#include <stdlib.h>
#include "../Inc/cJSON.h"
#include "../Inc/input_JSON.h"

extern uint8_t Status_DIN[8];	//Статус цифровых входов
extern uint8_t Status_AIN[8];	//Статус аналоговых входов
extern uint8_t Status_OCD[8];	//Статус выходов открытый коллектор
extern uint8_t Status_1WR[8];	//Статус выходов интерфейса 1-Wire
extern uint8_t Status_PWM[8];	//Статус выходов ШИМ

char *INSTRUCTION;		//Инструкция
char *TYPE;				//Комманда

//Переменные уловных выражений
char *D_IN;					//Цифровой вход
char *D_OUT;				//Цифровой выход
char *VAR_IN;				//Переменная уровня цифрового входа
char *VAR_OUT;				//Переменная уровня цифрового выхода
char *A_IN;					//Аналоговый вход
char *A_OUT;				//Аналоговый выход
char *RANGE_LOW;			//Переменная нижней границы аналогового входа
char *RANGE_HIGH;			//Переменная верхней границы аналогового выхода
char *PWM_OUT;				//Выход ШИМ
char *D_CYCLE;				//Коэффициент заполнения ШИМ
char *D_CONVERSION_STEP;	//Шаг преобразования ШИМ
char *BREATHING_EFFECT;		//Эффект "дыхания"
char *DYNAMIC_PWM;			//Динамический ШИМ?
char *ROM_RAW;				//Уникальный идентификатор датчика температуры
char *RANGE_TEMP_LOW;		//Нижняя границы температуры
char *RANGE_TEMP_HIGH;		//Верхняя граница температуры

char *NAME_FW;
char *VERSION_FW;
char *SIZE_FW;

//Массивы параметров
char *DigitalParamMass;					//Массив для значений цифровых выходов (8 параметров)
char *AnalogParamMass;					//Массив для значений аналоговых выходов (8 параметров)
char *OpenCollectorDrainParamMass;		//Массив для значений релейных выходов (8 параметров)
char *OneWireParamMass;					//Массив для значений 1-Wire выходов (2 параметра)
char *PWMParamMass;						//Массив для значений ШИМ выходов (4 параметра)

char *TIME;				//Переменная JSON системного времени сервера, для сравнения актуальности данных (если время с сервера меньше чем время у клиента, то данные не актуальны и игнорируем их) Timestamp
int Time_Server = 0;	//Переменная системного времени сервера
int Time_Client = 0;	//Переменная системного времени клиента

char paramValue[8];
char *MyFile;

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
//Функция разбора полученной строки JSON
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

				save_dido(D_IN, text);

				set_dido(D_IN, (uint8_t)(atoi(VAR_IN)), D_OUT, (uint8_t)(atoi(VAR_OUT)));

				//---------------------------------QA---------------------------------
				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(D_IN);
				SEND_str(": ");
				USART_Tx(D_IN[4]);
				SEND_str("\n");
				SEND_str(VAR_IN);
				SEND_str("\n");
				SEND_str(D_OUT);
				SEND_str(": ");
				USART_Tx(D_OUT[5]);
				SEND_str("\n");
				SEND_str(VAR_OUT);
				SEND_str("\n");
				//------------------------------------------------------------------

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
				free(s3);
				free(s4);
			}
			else if((strcmp(TYPE, "SET_VAIDO") == 0) || strcmp(TYPE, "SET_CAIDO") == 0)	//Включить/выключить один цифровой выход если аналоговый вход(измерение напряжения || измерение тока) в интервале значений
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

				if(strcmp(A_IN, "VHOD1") == 0)
				{
					if(strcmp(TYPE, "SET_VAIDO") == 0)
					{
						AiDo[0].inputFlag = true;
						SelectChannelOne(SET);
					}
					else
					{
						AiDo[0].inputFlag = false;
						SelectChannelOne(RESET);
					}
				}
				else if(strcmp(A_IN, "VHOD2") == 0)
				{
					if(strcmp(TYPE, "SET_VAIDO") == 0)
					{
						AiDo[1].inputFlag = true;
						SelectChannelTwo(SET);
					}
					else
					{
						AiDo[1].inputFlag = false;
						SelectChannelTwo(RESET);
					}
				}
				else if(strcmp(A_IN, "VHOD3") == 0)
				{
					if(strcmp(TYPE, "SET_VAIDO") == 0)
					{
						AiDo[2].inputFlag = true;
						SelectChannelThree(SET);
					}
					else
					{
						AiDo[2].inputFlag = false;
						SelectChannelThree(RESET);
					}
				}
				else if(strcmp(A_IN, "VHOD4") == 0)
				{
					if(strcmp(TYPE, "SET_VAIDO") == 0)
					{
						AiDo[3].inputFlag = true;
						SelectChannelFour(SET);
					}
					else
					{
						AiDo[3].inputFlag = false;
						SelectChannelFour(RESET);
					}
				}

				save_aido(A_IN, text);

				set_aido(A_IN, atof(RANGE_LOW), atof(RANGE_HIGH), D_OUT, (uint8_t)(atoi(VAR_OUT)));
				//---------------------------------QA---------------------------------
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
				//------------------------------------------------------------------
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
			else if(strcmp(TYPE, "SET_PWM") == 0)	//Выставить коэффициент заполнения на ШИМ
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "PWM_OUT");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "D_CYCLE");
				PWM_OUT = s1->valuestring;
				D_CYCLE = s2->valuestring;

				save_pwm(PWM_OUT, text);

				set_pwm(PWM_OUT, (uint32_t)(atoi(D_CYCLE)));
				//---------------------------------QA---------------------------------
				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(PWM_OUT);
				SEND_str("\n");
				SEND_str(D_CYCLE);
				SEND_str("\n");
				//------------------------------------------------------------------
				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
			}
			else if(strcmp(TYPE, "SET_TEMP_PROFILE") == 0)	//Установить профиль температуры: Если температура в диапазоне значений то установить цыфровой выход(OCD)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "ROM_RAW");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_TEMP_LOW");
				cJSON *s3 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "RANGE_TEMP_HIGH");
				cJSON *s4 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "D_OUT");
				cJSON *s5 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VAR_OUT");

				ROM_RAW = s1->valuestring;
				RANGE_TEMP_LOW = s2->valuestring;
				RANGE_TEMP_HIGH = s3->valuestring;
				D_OUT = s4->valuestring;
				VAR_OUT = s5->valuestring;

				save_tsido(ROM_RAW, text);

				set_temperature(ROM_RAW, atof(RANGE_TEMP_LOW), atof(RANGE_TEMP_HIGH), D_OUT, (uint8_t)(atoi(VAR_OUT)));

				SEND_str("\n");
				SEND_str(TYPE);
				SEND_str("\n");
				SEND_str(ROM_RAW);
				SEND_str("\n");
				SEND_str(RANGE_TEMP_LOW);
				SEND_str("\n");
				SEND_str(RANGE_TEMP_HIGH);
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
		else if(strcmp(INSTRUCTION, "UPDATE_FIRMWARE") == 0)
		{
			cJSON *sType = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "TYPE");
			TYPE = sType->valuestring;

			if(strcmp(TYPE, "FILE_DOWNLOAD") == 0)
			{
				cJSON *s1 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "NAME");
				cJSON *s2 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "VERSION");
				cJSON *s3 = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "COMMAND"), "SIZE");
				NAME_FW = s1->valuestring;
				VERSION_FW = s2->valuestring;
				SIZE_FW = s3->valuestring;

				firmware.NAME = NAME_FW;
				firmware.VERSION = VERSION_FW;
				firmware.SIZE = atoi(SIZE_FW);
				firmware.check_UPD = true;

				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);
				free(s1);
				free(s2);
				free(s3);
			}
			else if(strcmp(TYPE, "RESET_MCU") == 0)
			{
				//Нужно сохранить все данные перед перезагрузкой!!!
				cJSON_Delete(json);
				free(stime);
				free(sInstruction);
				free(sType);

				HAL_NVIC_SystemReset();		//Перезапускаем контроллер
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

				MyFile = parseValue(DigitalParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_DIN[i] = (MyFile[i] - 0x30);
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

				MyFile = parseValue(AnalogParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_AIN[i] = (MyFile[i] - 0x30);
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

				MyFile = parseValue(OpenCollectorDrainParamMass);

				for(int i = 0; i < 8; i++)
				{
					Status_OCD[i] = (MyFile[i] - 0x30);
				}

				ReWriteOCD();

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
