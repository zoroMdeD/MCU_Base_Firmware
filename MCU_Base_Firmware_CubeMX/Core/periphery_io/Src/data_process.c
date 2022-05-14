/*
 * input_data.c
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */
#include "../Inc/data_process.h"

#include "stdio.h"

extern uint8_t Status_AIN[];	//Статус аналоговых входов
extern uint8_t Status_DIN[];	//Статус цифровых входов
extern uint8_t Status_PWM[];	//Статус выходов ШИМ
extern uint8_t Status_OCD[];	//Статус выходов открытый коллектор
extern uint8_t Status_1WR[];	//Статус выходов интерфейса 1-Wire

extern double adcValue[];

extern bool OneWire_Test_Flag_Init;
extern bool OneWire_Test_Flag_Read;
extern uint8_t Dev_Cnt;
uint16_t raw_temper;
uint8_t dt[8];
char c;
extern char Device_RAW_ROM[AMT_TEMP_SENS][20];
extern float temper;
char sign_temper[16];

GPIO_TypeDef *pVHOD[8] = {VHOD1, VHOD2, VHOD3, VHOD4, VHOD5, VHOD6, VHOD7, VHOD8};				//Входы дискретных сигналов
GPIO_TypeDef *pVIHOD[8] = {VIHOD1, VIHOD2, VIHOD3, VIHOD4, VIHOD5, VIHOD6, VIHOD7, VIHOD8};		//Выходы открытый коллектор
uint16_t OCD_Pin[8] = {O0_Pin, O1_Pin, O2_Pin, O3_Pin, O4_Pin, O5_Pin, O6_Pin, O7_Pin};

//Мониторинг дискретных входов и изменение выходов открытый коллектор
//	*(Если Вход1 == 0 то Выход3 = 1) Инверсная логика на входах оптопар
void CheckReWriteDiDo(void)
{
	for(int i = 0; i < 8; i++)
	{
		if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) != DiDo[i].VAR_IN)
		{
			Status_OCD[i] = DiDo[i].VAR_OUT;
			HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, DiDo[i].VAR_OUT);
		}
		else if(HAL_GPIO_ReadPin(DiDo[i].D_IN, DiDo[i].DIN_Pin) == DiDo[i].VAR_IN)
		{
			Status_OCD[i] = !DiDo[i].VAR_OUT;
			HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, !DiDo[i].VAR_OUT);
		}
	}
}
//Мониторинг аналоговых входов и изменение выходов открытый коллектор
void CheckReWriteAiDo(void)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(AiDo[i].OCD_Pin == OCD_Pin[j])
			{
				if((adcValue[i] >= AiDo[i].RANGE_LOW) && (AiDo[i].RANGE_HIGH >= adcValue[i]))
				{
					Status_OCD[j] = AiDo[i].VAR_OUT;
					HAL_GPIO_WritePin(AiDo[i].D_OUT, AiDo[i].OCD_Pin, AiDo[i].VAR_OUT);
				}
				else
				{
					Status_OCD[j] = AiDo[i].VAR_OUT;
					HAL_GPIO_WritePin(AiDo[i].D_OUT, AiDo[i].OCD_Pin, RESET);	//!AiDo[i].VAR_OUT - вариант инверсии выхода по отношении ко входу
				}
			}
		}
	}
}
//Мониторинг датчиков температуры и изменение выходов открытый коллектор
void CheckReWriteTSiDo(void)
{
	if(OneWire_Test_Flag_Init)
	{
		OneWire_Test_Flag_Init = false;
    	for(uint8_t i = 1; i <= Dev_Cnt; i++)
    	{
    		sensors_MeasureTemperCmd(NO_SKIP_ROM, i);
    	}
	}
	if(OneWire_Test_Flag_Read)
	{
		OneWire_Test_Flag_Read = false;
    	for(uint8_t i = 1; i <= Dev_Cnt; i++)
    	{
    		for(uint8_t j = 0; j < 8; j++)
    		{
    			if(TSiDo[i-1].OCD_Pin == OCD_Pin[j])
    			{
    				sensors_ReadStratcpad(NO_SKIP_ROM, dt, i);
    				raw_temper = ((uint16_t)dt[1]<<8)|dt[0];
    				if(sensors_GetSign(raw_temper))
    					c='-';
    				else
    					c='+';
    				temper = sensors_Convert(raw_temper);
    				sprintf(sign_temper, "%c%.2f", c, temper);
    				temper = atof(sign_temper);
    				//--------------------from debug--------------------------
    				HAL_UART_Transmit(&huart3, (uint8_t*)sign_temper, strlen(sign_temper), 0x1000);
    				//--------------------------------------------------------
					if((temper >= TSiDo[i-1].RANGE_TEMP_LOW) && (TSiDo[i-1].RANGE_TEMP_HIGH >= temper))
					{
						Status_OCD[j] = TSiDo[i-1].VAR_OUT;
						HAL_GPIO_WritePin(TSiDo[i-1].D_OUT, TSiDo[i-1].OCD_Pin, TSiDo[i-1].VAR_OUT);
					}
					else
					{
						Status_OCD[j] =  TSiDo[i-1].VAR_OUT;
						HAL_GPIO_WritePin(TSiDo[i-1].D_OUT, TSiDo[i-1].OCD_Pin, RESET);
					}
    			}
    		}
    	}
	}
}
double Conversion_ADC1(uint16_t ADC_value, uint8_t item)
{
	double Value = 0;
	const double Resolution = 0.0008056640625;

	Value = (ADC_value * Resolution);

	if(AiDo[item].inputFlag)
		Value = (Value * 3.2323232323232);	//Напряжение
	else
		Value = (Value / 60);				//Ток (60 - коэффициент усиления ОУ)

	return Value;
}
//Включить/выключить цифровой выход(Открытый коллектор) если цифровой вход = значение(уровень)
//Принимает "D_IN" - строку с номером цифрового входа
//Принимает "VAR_IN" - переменная состояния входа
//Принимает "D_OUT" - строку с номером цифрового выхода
//Принимает "VAR_OUT" - переменная состояния выхода
void set_dido(char *D_IN, uint8_t VAR_IN, char *D_OUT, uint8_t VAR_OUT)
{
	char VHOD[8][10] = {"VHOD1", "VHOD2", "VHOD3", "VHOD4", "VHOD5", "VHOD6", "VHOD7", "VHOD8"};
	char VIHOD[8][10] = {"VIHOD1", "VIHOD2", "VIHOD3", "VIHOD4", "VIHOD5", "VIHOD6", "VIHOD7", "VIHOD8"};

	uint16_t DIN_Pin[8] = {IN0_Pin, IN1_Pin, IN2_Pin, IN3_Pin, IN4_Pin, IN5_Pin, IN6_Pin, IN7_Pin};

	for(int i = 0; i < 8; i++)
	{
		if(strcmp(D_IN, VHOD[i]) == 0)
		{
			if(DiDo[i].clrFlag != false)
				HAL_GPIO_WritePin(DiDo[i].D_OUT, DiDo[i].OCD_Pin, RESET);

			DiDo[i].clrFlag = true;
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

					//For Debug:
					//------------------------------------------------
					if(DiDo[i].D_IN == VHOD1)
						SEND_str("1 - success...\n");
					if(DiDo[i].VAR_IN == VAR_IN)
						SEND_str("2 - success...\n");
					if(DiDo[i].D_OUT == VIHOD3)
						SEND_str("3 - success...\n");
					if(DiDo[i].VAR_OUT == VAR_OUT)
						SEND_str("4 - success...\n");
					//------------------------------------------------

					break;
				}
			}
			break;
		}
	}
}
//Включить/выключить один цифровой выход(открытый коллектор) если аналоговый вход в интервале значений
//Принимает "A_IN" - строку с номером аналогового входа
//Принимает "RANGE_LOW" - нижний предел значений
//Принимает "RANGE_HIGH" - верхний предел значений
//Принимает "D_OUT" - строку с номером цифрового выхода
//Принимает "VAR_OUT" - переменная состояния выхода
void set_aido(char *A_IN, double RANGE_LOW, double RANGE_HIGH, char *D_OUT, uint8_t VAR_OUT)
{
	char VHOD[4][10] = {"VHOD1", "VHOD2", "VHOD3", "VHOD4"};
	char VIHOD[8][10] = {"VIHOD1", "VIHOD2", "VIHOD3", "VIHOD4", "VIHOD5", "VIHOD6", "VIHOD7", "VIHOD8"};

	uint8_t pVHOD[4] = {ADC1_IN3, ADC1_IN4, ADC1_IN5, ADC1_IN6};

	for(int i = 0; i < 4; i++)
	{
		if(strcmp(A_IN, VHOD[i]) == 0)
		{
			if(AiDo[i].clrFlag != false)
				HAL_GPIO_WritePin(AiDo[i].D_OUT, AiDo[i].OCD_Pin, RESET);

			AiDo[i].clrFlag = true;
			AiDo[i].A_IN = pVHOD[i];
			AiDo[i].RANGE_LOW = RANGE_LOW;
			AiDo[i].RANGE_HIGH = RANGE_HIGH;
			//DiDo[i].DIN_Pin = DIN_Pin[i];
			for(int j = 0; j < 8; j++)
			{
				if(strcmp(D_OUT, VIHOD[j]) == 0)
				{
					AiDo[i].D_OUT = pVIHOD[j];
					AiDo[i].VAR_OUT = VAR_OUT;
					AiDo[i].OCD_Pin = OCD_Pin[j];

					//For Debug:
					//------------------------------------------------
					char Buff[32];
					SEND_str("SET VALUE: ");
					sprintf(Buff, "%.3f", adcValue[i]);
					SEND_str(Buff);
					SEND_str("\nRANGE_LOW: ");
					sprintf(Buff, "%.3f", AiDo[i].RANGE_LOW);
					SEND_str(Buff);
					SEND_str("\nRANGE_HIGH: ");
					sprintf(Buff, "%.3f", AiDo[i].RANGE_HIGH);
					SEND_str(Buff);
					SEND_str("\n");
					//------------------------------------------------

					break;
				}
			}
			break;
		}
	}
}
//Задать сигнал ШИМ на одном выходе
//Принимает "PWM_OUT" - строку с номером ШИМ выхода
//Принимает "D_CYCLE" - коэффициент заполнения ШИМ
void set_pwm(char *PWM_OUT, uint32_t D_CYCLE)
{
	char PVIHOD[4][5] = {"PWM1", "PWM2", "PWM3", "PWM4"};
	uint32_t PWM_Channel[4] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

	for(int i = 0; i < 4; i++)
	{
		if(strcmp(PWM_OUT, PVIHOD[i]) == 0)
		{
			if(PWM[i].clrFlag != false)
				HAL_TIM_PWM_Start_DMA(&htim3, PWM_Channel[i], 0, 1);

			PWM[i].clrFlag = true;
			PWM[i].PWM_Channel = PWM_Channel[i];
			PWM[i].D_CYCLE[0] = (D_CYCLE * 100);
			HAL_TIM_PWM_Start_DMA(&htim3, PWM_Channel[i], (uint32_t*)PWM[i].D_CYCLE, 1);

			//For Debug:
			//------------------------------------------------
			char Buff[32];
			SEND_str("PWM_OUT: ");
			SEND_str(PVIHOD[i]);
			SEND_str("\n");
			SEND_str("DUTY_CYCLE: ");
			sprintf(Buff, "%d", D_CYCLE);
			SEND_str(Buff);
			SEND_str("%");
			SEND_str("\n");
			//------------------------------------------------

			break;
		}
	}
}
//Включить/выключить один цифровой выход(открытый коллектор) если температура датчика в интервале значений
//Принимает "ROM_RAW" - строку с уникальным идкнтификатором температурного датчика
//Принимает "RANGE_TEMP_LOW" - нижний предел значения температуры
//Принимает "RANGE_TEMP_HIGH" - верхний предел значения температуры
//Принимает "D_OUT" - строку с номером цифрового выхода
//Принимает "VAR_OUT" - переменная состояния выхода
void set_temperature(char *ROM_RAW, double RANGE_TEMP_LOW, double RANGE_TEMP_HIGH, char *D_OUT, uint8_t VAR_OUT)
{
	char VIHOD[8][10] = {"VIHOD1", "VIHOD2", "VIHOD3", "VIHOD4", "VIHOD5", "VIHOD6", "VIHOD7", "VIHOD8"};

	for(int i = 0; i < AMT_TEMP_SENS; i++)
	{
		if(strcmp(ROM_RAW, Device_RAW_ROM[i]) == 0)
		{
			if(TSiDo[i].clrFlag != false)
				HAL_GPIO_WritePin(TSiDo[i].D_OUT, TSiDo[i].OCD_Pin, RESET);

			TSiDo[i].clrFlag = true;
			TSiDo[i].DEVICE_NUMBER = i+1;
			for(int j = 0; j < 16; j++)
				TSiDo[i].ROM_RAW[j] = Device_RAW_ROM[i][j];
			TSiDo[i].RANGE_TEMP_LOW = RANGE_TEMP_LOW;
			TSiDo[i].RANGE_TEMP_HIGH = RANGE_TEMP_HIGH;
			for(int j = 0; j < 8; j++)
			{
				if(strcmp(D_OUT, VIHOD[j]) == 0)
				{
					TSiDo[i].D_OUT = pVIHOD[j];
					TSiDo[i].VAR_OUT = VAR_OUT;
					TSiDo[i].OCD_Pin = OCD_Pin[j];

					//For Debug:
					//------------------------------------------------
					char Buff[32];
					SEND_str("ROM_RAW: ");
					SEND_str(ROM_RAW);
					SEND_str("\n");
					SEND_str("SET VALUE: ");
					sprintf(Buff, "%.2f", atof(sign_temper));
					SEND_str(Buff);
					SEND_str("\nRANGE_TEMP_LOW: ");
					sprintf(Buff, "%.2f", TSiDo[i].RANGE_TEMP_LOW);
					SEND_str(Buff);
					SEND_str("\nRANGE_TEMP_HIGH: ");
					sprintf(Buff, "%.2f", TSiDo[i].RANGE_TEMP_HIGH);
					SEND_str(Buff);
					SEND_str("\n");
					//------------------------------------------------

					break;
				}
			}
			break;
		}
	}
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
