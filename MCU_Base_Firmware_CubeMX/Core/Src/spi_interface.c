/*
 * spi_interface.c
 *
 *  Created on: 25 нояб. 2020 г.
 *      Author: MMorozov
 */

#include "main.h"
#include "spi_interface.h"

uint8_t Status_AIN[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//Статус аналоговых входов
uint8_t Status_DIN[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//Статус цифровых входов
uint8_t Status_PWM[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//Статус выходов ШИМ
uint8_t Status_OCD[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//Статус выходов открытый коллектор
uint8_t Status_1WR[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//Статус выходов интерфейса 1-Wire

uint8_t SPI_rx_buf[1] = {0};
uint8_t SPI_tx_buf[1] = {0};
uint8_t flag_iput_spi2 = 0;

uint8_t i = 0;
uint8_t j = 0;
uint8_t tp = 0;
uint8_t tp1 = 1;
uint8_t tp2 = 0;
uint8_t tp3 = 1;

//extern struct ScaningDIN_UpdateOCD
//{
//	GPIO_TypeDef *D_IN;		//Адрес порта оптопары, который читаем для изменения состояния на порту выхода OCD
//	uint8_t VAR_IN;			//Значение которое должно быть "флагом" для выполнения условия по выходу OCD(установка значения, например переключение реле)
//	uint16_t DIN_Pin;		//Адрес разряда порта(D_IN) с которого читаем значение(VAR_IN), которое должно удовлетворять условию для выполнения условия
//	GPIO_TypeDef *D_OUT;	//Адрес порта открытого(ых) коллектора(ов), который(е) устанавливаем в состояние на порту выхода OCD(VAR_OUT)
//	uint8_t VAR_OUT;		//Значение которое устанавливается если выполняется условие, которое установлено на вход(VAR_IN)
//	uint16_t OCD_Pin;		//Адрес разряда порта(D_OUT) на который устанавливаем значение(VAR_OUT), которое например переключает реле
//}DiDo[8];

//RTC
//RTC_TimeTypeDef sTime = {0};
//RTC_DateTypeDef DateToUpdate = {0};
//
//char trans_str[64] = {0,};


//Функция обновления значений в массиве данных коллекторных выходов
void ReWriteOCD(void)
{
	if(Status_OCD[0])
		HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, SET);
	else
		HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, RESET);
	if(Status_OCD[1])
		HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, SET);
	else
		HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, RESET);
	if(Status_OCD[2])
		HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, SET);
	else
		HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, RESET);
	if(Status_OCD[3])
		HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, SET);
	else
		HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, RESET);
	if(Status_OCD[4])
		HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, SET);
	else
		HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, RESET);
	if(Status_OCD[5])
		HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, SET);
	else
		HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, RESET);
	if(Status_OCD[6])
		HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, SET);
	else
		HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, RESET);
	if(Status_OCD[7])
		HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, SET);
	else
		HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, RESET);
}
//Функция обновления значений в массиве данных цифровых входов
void ReWriteDIN(void)
{
	if(Status_DIN[0])
		HAL_GPIO_WritePin(IN0_GPIO_Port, IN0_Pin, SET);
	else
		HAL_GPIO_WritePin(IN0_GPIO_Port, IN0_Pin, RESET);
	if(Status_DIN[1])
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, SET);
	else
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, RESET);
	if(Status_DIN[2])
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, SET);
	else
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, RESET);
	if(Status_DIN[3])
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, SET);
	else
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, RESET);
	if(Status_DIN[4])
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, SET);
	else
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, RESET);
	if(Status_DIN[5])
		HAL_GPIO_WritePin(IN5_GPIO_Port, IN5_Pin, SET);
	else
		HAL_GPIO_WritePin(IN5_GPIO_Port, IN5_Pin, RESET);
	if(Status_DIN[6])
		HAL_GPIO_WritePin(IN6_GPIO_Port, IN6_Pin, SET);
	else
		HAL_GPIO_WritePin(IN6_GPIO_Port, IN6_Pin, RESET);
	if(Status_DIN[7])
		HAL_GPIO_WritePin(IN7_GPIO_Port, IN7_Pin, SET);
	else
		HAL_GPIO_WritePin(IN7_GPIO_Port, IN7_Pin, RESET);
}
//Функция обновления значений в массиве данных аналоговых входов
void ReWriteAIN(void)
{
	if(Status_AIN[0])
		HAL_GPIO_WritePin(AIN0_GPIO_Port, AIN0_Pin, SET);
	else
		HAL_GPIO_WritePin(AIN0_GPIO_Port, AIN0_Pin, RESET);
	if(Status_AIN[1])
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, SET);
	else
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, RESET);
	if(Status_AIN[2])
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, SET);
	else
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, RESET);
	if(Status_AIN[3])
		HAL_GPIO_WritePin(AIN3_GPIO_Port, AIN3_Pin, SET);
	else
		HAL_GPIO_WritePin(AIN3_GPIO_Port, AIN3_Pin, RESET);
}
//Функция обновления значений в массиве данных выходных каналов широтноимпульсной модуляции
void ReWritePWM(void)
{
	if(Status_PWM[0])
		HAL_GPIO_WritePin(PWM0_GPIO_Port, PWM0_Pin, SET);
	else
		HAL_GPIO_WritePin(PWM0_GPIO_Port, PWM0_Pin, RESET);
	if(Status_PWM[1])
		HAL_GPIO_WritePin(PWM1_GPIO_Port, PWM1_Pin, SET);
	else
		HAL_GPIO_WritePin(PWM1_GPIO_Port, PWM1_Pin, RESET);
	if(Status_PWM[2])
		HAL_GPIO_WritePin(PWM2_GPIO_Port, PWM2_Pin, SET);
	else
		HAL_GPIO_WritePin(PWM2_GPIO_Port, PWM2_Pin, RESET);
	if(Status_PWM[3])
		HAL_GPIO_WritePin(PWM3_GPIO_Port, PWM3_Pin, SET);
	else
		HAL_GPIO_WritePin(PWM3_GPIO_Port, PWM3_Pin, RESET);
}
//Функция обновления значений в массиве данных однопроводного интерфейса
void ReWrite1Wire(void)
{
	if(Status_1WR[0])
		HAL_GPIO_WritePin(WR0_GPIO_Port, WR0_Pin, SET);
	else
		HAL_GPIO_WritePin(WR0_GPIO_Port, WR0_Pin, RESET);
	if(Status_1WR[1])
		HAL_GPIO_WritePin(WR1_GPIO_Port, WR1_Pin, SET);
	else
		HAL_GPIO_WritePin(WR1_GPIO_Port, WR1_Pin, RESET);
}
//Функция обработки посылок от интрефейсного МК
//Обновляет значения в массивах данных входов/выходов
void SPI_available(void)
{
	if(flag_iput_spi2)
	{
		switch(SPI_rx_buf[0])	//Избаиться от постоянной проверки кейс условий!
		{
				//------------------------Открытый коллектор------------------------
			case OC0_ON:
				Status_OCD[0] = 1;
				HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, SET);
				break;
			case OC0_OFF:
				Status_OCD[0] = 0;
				HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, RESET);
				break;
			case OC1_ON:
				Status_OCD[1] = 1;
				HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, SET);
				break;
			case OC1_OFF:
				Status_OCD[1] = 0;
				HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, RESET);
				break;
			case OC2_ON:
				Status_OCD[2] = 1;
				HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, SET);
				break;
			case OC2_OFF:
				Status_OCD[2] = 0;
				HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, RESET);
				break;
			case OC3_ON:
				Status_OCD[3] = 1;
				HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, SET);
				break;
			case OC3_OFF:
				Status_OCD[3] = 0;
				HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, RESET);
				break;
			case OC4_ON:
				Status_OCD[4] = 1;
				HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, SET);
				break;
			case OC4_OFF:
				Status_OCD[4] = 0;
				HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, RESET);
				break;
			case OC5_ON:
				Status_OCD[5] = 1;
				HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, SET);
				break;
			case OC5_OFF:
				Status_OCD[5] = 0;
				HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, RESET);
				break;
			case OC6_ON:
				Status_OCD[6] = 1;
				HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, SET);
				break;
			case OC6_OFF:
				Status_OCD[6] = 0;
				HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, RESET);
				break;
			case OC7_ON:
				Status_OCD[7] = 1;
				HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, SET);
				break;
			case OC7_OFF:
				Status_OCD[7] = 0;
				HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, RESET);
				break;
				//------------------------------------------------------------------
				//--------------------------Цифровые входы--------------------------
			case DIN0_R:
				Status_DIN[0] = 1;
				HAL_GPIO_WritePin(IN0_GPIO_Port, IN0_Pin, SET);
				break;
			case DIN0_F:
				Status_DIN[0] = 0;
				HAL_GPIO_WritePin(IN0_GPIO_Port, IN0_Pin, RESET);
				break;
			case DIN1_R:
				Status_DIN[1] = 1;
				HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, SET);
				break;
			case DIN1_F:
				Status_DIN[1] = 0;
				HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, RESET);
				break;
			case DIN2_R:
				Status_DIN[2] = 1;
				HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, SET);
				break;
			case DIN2_F:
				Status_DIN[2] = 0;
				HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, RESET);
				break;
			case DIN3_R:
				Status_DIN[3] = 1;
				HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, SET);
				break;
			case DIN3_F:
				Status_DIN[3] = 0;
				HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, RESET);
				break;
			case DIN4_R:
				Status_DIN[4] = 1;
				HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, SET);
				break;
			case DIN4_F:
				Status_DIN[4] = 0;
				HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, RESET);
				break;
			case DIN5_R:
				Status_DIN[5] = 1;
				HAL_GPIO_WritePin(IN5_GPIO_Port, IN5_Pin, SET);
				break;
			case DIN5_F:
				Status_DIN[5] = 0;
				HAL_GPIO_WritePin(IN5_GPIO_Port, IN5_Pin, RESET);
				break;
			case DIN6_R:
				Status_DIN[6] = 1;
				HAL_GPIO_WritePin(IN6_GPIO_Port, IN6_Pin, SET);
				break;
			case DIN6_F:
				Status_DIN[6] = 0;
				HAL_GPIO_WritePin(IN6_GPIO_Port, IN6_Pin, RESET);
				break;
			case DIN7_R:
				Status_DIN[7] = 1;
				HAL_GPIO_WritePin(IN7_GPIO_Port, IN7_Pin, SET);
				break;
			case DIN7_F:
				Status_DIN[7] = 0;
				HAL_GPIO_WritePin(IN7_GPIO_Port, IN7_Pin, RESET);
				break;
				//------------------------------------------------------------------
				//-------------------------Аналоговые входы-------------------------
			case AIN0_ON:
				Status_AIN[0] = 1;
				HAL_GPIO_WritePin(AIN0_GPIO_Port, AIN0_Pin, SET);
				break;
			case AIN0_OFF:
				Status_AIN[0] = 0;
				HAL_GPIO_WritePin(AIN0_GPIO_Port, AIN0_Pin, RESET);
				break;
			case AIN1_ON:
				Status_AIN[1] = 1;
				HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, SET);
				break;
			case AIN1_OFF:
				Status_AIN[1] = 0;
				HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, RESET);
				break;
			case AIN2_ON:
				Status_AIN[2] = 1;
				HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, SET);
				break;
			case AIN2_OFF:
				Status_AIN[2] = 0;
				HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, RESET);
				break;
			case AIN3_ON:
				Status_AIN[3] = 1;
				HAL_GPIO_WritePin(AIN3_GPIO_Port, AIN3_Pin, SET);
				break;
			case AIN3_OFF:
				Status_AIN[3] = 0;
				HAL_GPIO_WritePin(AIN3_GPIO_Port, AIN3_Pin, RESET);
				break;
				//------------------------------------------------------------------
				//----------------------------Выходы ШИМ----------------------------
			case PWM0_ON:
				Status_PWM[0] = 1;
				HAL_GPIO_WritePin(PWM0_GPIO_Port, PWM0_Pin, SET);
				break;
			case PWM0_OFF:
				Status_PWM[0] = 0;
				HAL_GPIO_WritePin(PWM0_GPIO_Port, PWM0_Pin, RESET);
				break;
			case PWM1_ON:
				Status_PWM[1] = 1;
				HAL_GPIO_WritePin(PWM1_GPIO_Port, PWM1_Pin, SET);
				break;
			case PWM1_OFF:
				Status_PWM[1] = 0;
				HAL_GPIO_WritePin(PWM1_GPIO_Port, PWM1_Pin, RESET);
				break;
			case PWM2_ON:
				Status_PWM[2] = 1;
				HAL_GPIO_WritePin(PWM2_GPIO_Port, PWM2_Pin, SET);
				break;
			case PWM2_OFF:
				Status_PWM[2] = 0;
				HAL_GPIO_WritePin(PWM2_GPIO_Port, PWM2_Pin, RESET);
				break;
			case PWM3_ON:
				Status_PWM[3] = 1;
				HAL_GPIO_WritePin(PWM3_GPIO_Port, PWM3_Pin, SET);
				break;
			case PWM3_OFF:
				Status_PWM[3] = 0;
				HAL_GPIO_WritePin(PWM3_GPIO_Port, PWM3_Pin, RESET);
				break;
				//------------------------------------------------------------------
				//--------------------------Выходы 1-Wire---------------------------
			case WR0_ON:
				Status_1WR[0] = 1;
				HAL_GPIO_WritePin(WR0_GPIO_Port, WR0_Pin, SET);
				break;
			case WR0_OFF:
				Status_1WR[0] = 0;
				HAL_GPIO_WritePin(WR0_GPIO_Port, WR0_Pin, RESET);
				break;
			case WR1_ON:
				Status_1WR[1] = 1;
				HAL_GPIO_WritePin(WR1_GPIO_Port, WR1_Pin, SET);
				break;
			case WR1_OFF:
				Status_1WR[1] = 0;
				HAL_GPIO_WritePin(WR1_GPIO_Port, WR1_Pin, RESET);
				break;
				//----------------------------GSM Modem-----------------------------
			case GSM_ON:
				break;
			case GSM_OFF:
				break;
				//------------------------------------------------------------------
				//------------------Проверка подключения модулей--------------------
			case RELAY_TEST:
				break;
			case DIMMING_TEST:
				break;
			case DIGITAL_TEST:
				break;
			case INTERFACE_TEST:
				break;
				//------------------------------------------------------------------
		}
		if(SPI_rx_buf[0] == MCU_OUTPUT)
		{
			tp = 1;
			tp1 = 1;
			SEND_str("start\n");
			SEND_str("status_OCD ");
			USART_Tx(Status_OCD[0]);
			USART_Tx(Status_OCD[1]);
			USART_Tx(Status_OCD[2]);
			USART_Tx(Status_OCD[3]);
			USART_Tx(Status_OCD[4]);
			USART_Tx(Status_OCD[5]);
			USART_Tx(Status_OCD[6]);
			USART_Tx(Status_OCD[7]);
			SEND_str(" END_status_OCD\n");

			//----------------------For testing----------------------
//			if(Status_OCD[0])
//				HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O0_GPIO_Port, O0_Pin, RESET);
//			if(Status_OCD[1])
//				HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, RESET);
//			if(Status_OCD[2])
//				HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, RESET);
//			if(Status_OCD[3])
//				HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, RESET);
//			if(Status_OCD[4])
//				HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, RESET);
//			if(Status_OCD[5])
//				HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, RESET);
//			if(Status_OCD[6])
//				HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, RESET);
//			if(Status_OCD[7])
//				HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, SET);
//			else
//				HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, RESET);
			//-------------------------------------------------------

		}
		if(tp)
		{
			if(i == 0)
			{
				SPI_tx_buf[0] = Status_AIN[j];
				j++;
				if(j > 7)
				{
					j = 0;
					i++;
				}
				flag_iput_spi2 = 0;
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
			}
			else if(i == 1)
			{
				SPI_tx_buf[0] = Status_DIN[j];
				j++;
				if(j > 7)
				{
					j = 0;
					i++;
				}
				flag_iput_spi2 = 0;
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
			}
			else if(i == 2)
			{
				SPI_tx_buf[0] = Status_PWM[j];
				j++;
				if(j > 7)
				{
					j = 0;
					i++;
				}
				flag_iput_spi2 = 0;
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
			}
			else if(i == 3)
			{
				SPI_tx_buf[0] = Status_OCD[j];
				j++;
				if(j > 7)
				{
					j = 0;
					i++;
				}
				flag_iput_spi2 = 0;
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
			}
			else if(i == 4)
			{
				SPI_tx_buf[0] = Status_1WR[j];
				j++;
				if(j > 7)
				{
					j = 0;
					i++;
				}
//				else
//				{
					flag_iput_spi2 = 0;
					HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
					while(!flag_iput_spi2) {;}
//				}
			}
			else if(i == 5)
			{
				i = 0;
				j = 0;
				tp = 0;
				SEND_str("the_end\n");
				flag_iput_spi2 = 0;
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
			}
		}
		else if(SPI_rx_buf[0] == MCU_RELAY_CNT)
		{
			//По этой комманде надо прислать КОЛ-ВО подключенных блоков расширения РЕЛЕ (от 0 до 10)
		}
		else if(SPI_rx_buf[0] == MCU_DIGITAL_CNT)
		{
			//По этой комманде надо прислать КОЛ-ВО подключенных блоков расширения ЦИФРОВЫХ ВХОДОВ (от 0 до 10)
		}
		else if(SPI_rx_buf[0] == MCU_DIMMING_CNT)
		{
			//По этой комманде надо прислать КОЛ-ВО подключенных блоков расширения ДИММИРОВАНИЯ (от 0 до 10)
		}
		else if(SPI_rx_buf[0] == MCU_INTERFACE_CNT)
		{
			//По этой комманде надо прислать КОЛ-ВО подключенных блоков расширения ИНТЕРФЕЙСОВ (от 0 до 10)
		}
		else if(SPI_rx_buf[0] == MCU_RELAY_ADR)
		{
			//По этой комманде надо прислать АДРЕСА подключенных блоков расширения РЕЛЕ (от 2 до 11)
		}
		else if(SPI_rx_buf[0] == MCU_DIGITAL_ADR)
		{
			//По этой комманде надо прислать АДРЕСА подключенных блоков расширения ЦИФРОВЫХ ВХОДОВ (от 12 до 21)
		}
		else if(SPI_rx_buf[0] == MCU_DIMMING_ADR)
		{
			//По этой комманде надо прислать АДРЕСА подключенных блоков расширения ДИММИРОВАНИЯ (от 22 до 31)
		}
		else if(SPI_rx_buf[0] == MCU_INTERFACE_ADR)
		{
			//По этой комманде надо прислать АДРЕСА подключенных блоков расширения ИНТЕРФЕЙСОВ (от 32 до 41)
		}
		if(!tp)
		{
//			if(tp1)
//			{
//				tp1 = 0;
//			}
//			else
//			{
				flag_iput_spi2 = 1;		//Fix bag! so so
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
//			}
		}

		//		if(SPI_rx_buf[0] == MCU_TIME)
		//		{
		//			tp2 = 1;
		//			tp3 = 1;
		//			SEND_str("start_time_send\n");
		//
		//	        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
		//	        snprintf(trans_str, 63, "%.02d:%.02d", sTime.Hours, sTime.Minutes);
		//	        //HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 1000);
		//	        //SEND_str(trans_str);
		//
		////			SEND_str("status_AIN ");
		////			USART_Tx(Status_AIN[0]);
		////			USART_Tx(Status_AIN[1]);
		////			USART_Tx(Status_AIN[2]);
		////			USART_Tx(Status_AIN[3]);
		////			USART_Tx(Status_AIN[4]);
		////			USART_Tx(Status_AIN[5]);
		////			USART_Tx(Status_AIN[6]);
		////			USART_Tx(Status_AIN[7]);
		////			SEND_str(" END_status_AIN\n");
		//		}
		//		if(tp2)
		//		{
		//
		//			if(i <= 63)
		//			{
		//				SPI_tx_buf[0] = trans_str[i];
		//				i++;
		//				flag_iput_spi2 = 0;
		//				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
		//				while(!flag_iput_spi2) {;}
		//			}
		//			else if(i == 64)
		//			{
		//				i = 0;
		//				tp2 = 0;
		//				SEND_str("the_end_time_send\n");
		//				flag_iput_spi2 = 0;
		//				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
		//			}
		//		}
		//		if(!tp2)
		//		{
		//			if(tp3)
		//			{
		//				tp3 = 0;
		//			}
		//			else
		//			{
		//				flag_iput_spi2 = 0;
		//				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
		//				while(!flag_iput_spi2) {;}
		//			}
		//		}
	}
}
