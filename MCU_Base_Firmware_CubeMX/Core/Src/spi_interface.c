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

//RTC
//RTC_TimeTypeDef sTime = {0};
//RTC_DateTypeDef DateToUpdate = {0};
//
//char trans_str[64] = {0,};

void SPI_available(void)
{
	if(flag_iput_spi2)
	{
		switch(SPI_rx_buf[0])
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
				break;
			case DIN0_F:
				Status_DIN[0] = 0;
				break;
			case DIN1_R:
				Status_DIN[1] = 1;
				break;
			case DIN1_F:
				Status_DIN[1] = 0;
				break;
			case DIN2_R:
				Status_DIN[2] = 1;
				break;
			case DIN2_F:
				Status_DIN[2] = 0;
				break;
			case DIN3_R:
				Status_DIN[3] = 1;
				break;
			case DIN3_F:
				Status_DIN[3] = 0;
				break;
			case DIN4_R:
				Status_DIN[4] = 1;
				break;
			case DIN4_F:
				Status_DIN[4] = 0;
				break;
			case DIN5_R:
				Status_DIN[5] = 1;
				break;
			case DIN5_F:
				Status_DIN[5] = 0;
				break;
			case DIN6_R:
				Status_DIN[6] = 1;
				break;
			case DIN6_F:
				Status_DIN[6] = 0;
				break;
			case DIN7_R:
				Status_DIN[7] = 1;
				break;
			case DIN7_F:
				Status_DIN[7] = 0;
				break;
				//------------------------------------------------------------------
				//-------------------------Аналоговые входы-------------------------
			case AIN0_ON:
				Status_AIN[0] = 1;
				break;
			case AIN0_OFF:
				Status_AIN[0] = 0;
				break;
			case AIN1_ON:
				Status_AIN[1] = 1;
				break;
			case AIN1_OFF:
				Status_AIN[1] = 0;
				break;
			case AIN2_ON:
				Status_AIN[2] = 1;
				break;
			case AIN2_OFF:
				Status_AIN[2] = 0;
				break;
			case AIN3_ON:
				Status_AIN[3] = 1;
				break;
			case AIN3_OFF:
				Status_AIN[3] = 0;
				break;
			case AIN4_ON:
				Status_AIN[4] = 1;
				break;
			case AIN4_OFF:
				Status_AIN[4] = 0;
				break;
			case AIN5_ON:
				Status_AIN[5] = 1;;
				break;
			case AIN5_OFF:
				Status_AIN[5] = 0;
				break;
			case AIN6_ON:
				Status_AIN[6] = 1;
				break;
			case AIN6_OFF:
				Status_AIN[6] = 0;
				break;
			case AIN7_ON:
				Status_AIN[7] = 1;
				break;
			case AIN7_OFF:
				Status_AIN[7] = 0;
				break;
				//------------------------------------------------------------------
				//----------------------------Выходы ШИМ----------------------------
			case PWM0_ON:
				Status_PWM[0] = 1;
				break;
			case PWM0_OFF:
				Status_PWM[0] = 0;
				break;
			case PWM1_ON:
				Status_PWM[1] = 1;
				break;
			case PWM1_OFF:
				Status_PWM[1] = 0;
				break;
			case PWM2_ON:
				Status_PWM[2] = 1;
				break;
			case PWM2_OFF:
				Status_PWM[2] = 0;
				break;
			case PWM3_ON:
				Status_PWM[3] = 1;
				break;
			case PWM3_OFF:
				Status_PWM[3] = 0;
				break;
				//------------------------------------------------------------------
				//--------------------------Выходы 1-Wire---------------------------
			case WR0_ON:
				Status_1WR[0] = 1;
				break;
			case WR0_OFF:
				Status_1WR[0] = 0;
				break;
			case WR1_ON:
				Status_1WR[1] = 1;
				break;
			case WR1_OFF:
				Status_1WR[1] = 0;
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
			SEND_str("status_AIN ");
			USART_Tx(Status_AIN[0]);
			USART_Tx(Status_AIN[1]);
			USART_Tx(Status_AIN[2]);
			USART_Tx(Status_AIN[3]);
			USART_Tx(Status_AIN[4]);
			USART_Tx(Status_AIN[5]);
			USART_Tx(Status_AIN[6]);
			USART_Tx(Status_AIN[7]);
			SEND_str(" END_status_AIN\n");
		}

		if(tp)
		{
			if(i == 0)
			{
				SPI_tx_buf[0] = Status_AIN[j];
				j++;
				if(j == 8)
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
				if(j == 8)
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
				if(j == 8)
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
				if(j == 8)
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
				if(j == 8)
				{
					j = 0;
					i++;
				}
				else
				{
					flag_iput_spi2 = 0;
					HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
					while(!flag_iput_spi2) {;}
				}
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
			if(tp1)
			{
				tp1 = 0;
			}
			else
			{
				flag_iput_spi2 = 1;		//Fix bag! so so
				HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);
				while(!flag_iput_spi2) {;}
			}
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
