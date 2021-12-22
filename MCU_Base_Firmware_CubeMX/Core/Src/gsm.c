/*
 * gsm.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "main.h"
#include "gsm.h"
#include "usart_ring.h"
#include <stdlib.h>
#include "cJSON.h"
//#include "input_JSON.h"

#define SEND_STR_SIZE 64

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

char buf[GSM_RX_BUFFER_SIZE] = {0,};
char str[GSM_RX_BUFFER_SIZE] = {0,};

uint8_t toggle_pin_test = 0;

//Функция для замены смволов \r и \n на пробелы
void replac_string(char *src)
{
	if(!src) return;

	for(;*src; src++)
	{
		if(*src == '\n' || *src == '\r') *src = ' ';
	}
}

//Функция для отправки настроечных команд, в цикле лучше не использовать
void set_comand(char *buff)
{
	char str[SEND_STR_SIZE] = {0,};
	snprintf(str, SEND_STR_SIZE, "%s\r\n", buff);
	HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 100);
	HAL_Delay(200);
	GSM_RxCpltCallback();
}
//Функция проверки и установки скорости 19200
void chek_speed(void)
{
  for(uint8_t i = 0; i < 7; i++)
  {
	  uint32_t sp = 0;

	  if(i == 0) sp = 2400;
	  else if(i == 1) sp = 4800;
	  else if(i == 2) sp = 9600;
	  else if(i == 3) sp = 19200;
	  else if(i == 4) sp = 38400;
	  else if(i == 5) sp = 57600;
	  else if(i == 6) sp = 115200;

	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = sp;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
		  Error_Handler();
	  }

	  char str[16] = {0,};
	  HAL_UART_Transmit(&huart1, (uint8_t*)"AT\r\n", strlen("AT\r\n"), 1000);
	  HAL_Delay(300);

	  if(gsm_available()) //если модуль что-то прислал
	  {
		  uint16_t i = 0;

		  while(gsm_available())
		  {
			  str[i++] = gsm_read();
			  if(i > 15) break;
			  HAL_Delay(1);
		  }

		  if(strstr(str, "OK") != NULL)
		  {
			  char buf[64] = {0,};
			  snprintf(buf, 64, "Uart modem was %lu, switched to 57600\n", huart1.Init.BaudRate);
			  HAL_UART_Transmit(&huart3, (uint8_t*)buf, strlen(buf), 100);
			  HAL_UART_Transmit(&huart1, (uint8_t*)"AT+IPR=57600\r\n", strlen("AT+IPR=57600\r\n"), 1000);
			  HAL_Delay(250);
			  MX_USART1_UART_Init();
			  break;
		  }
	  }
  }
}
//Функция удаления символов \r и \n из строки
void clear_string(char *src)
{
	char *dst = NULL;
	if(!src) return;
	uint8_t i = 0;

	for(dst = src; *src; src++)
	{
		if(i < 2 && (*src == '\n' || *src == '\r'))
		{
			i++;
			continue;
		}
		else if(*src == '\n' || *src == '\r') *src = ' ';

		*dst++ = *src;
	}

	*dst = 0;
}
void INIT_GSM(void)
{
//	chek_speed();
//----------------------------------------------НАСТРОЙКА МОДЕМА----------------------------------------------
//	set_comand(ATCPAS);		//Проверка статуса модема
//	set_comand(ATCREG);  	//Проверка регистрации в сети - должен вернуть  +CREG: 0,1
////	set_comand(ATCLIP1); 	//Включить АОН
////	set_comand(ATE);     	//Отключить «эхо»
////	set_comand(ATS);     	//Поднимать трубку только "вручную"
////	set_comand(ATDDET);  	//Включить DTMF
////	set_comand(ATCCLKK); 	//Установить дату/время
////-----------------------------------------------НАСТРОЙКА СМС------------------------------------------------
//	set_comand(ATCMGF);    	//Устанавливает текстовый режим смс-сообщения
//	set_comand(ATCPBS);    	//Открывает доступ к данным телефонной книги SIM-карты
//	set_comand(ATCSCS);    	//Кодировка текста - GSM
//	set_comand(ATCNMI);    	//Настройка вывода смс в консоль
////----------------------------------------------------ИНФО----------------------------------------------------
//	set_comand(ATIPR);      //Скорость usart'a модема
//	set_comand(ATI);        //Название и версия модуля
//	set_comand(ATCGSN);     //Считывание IMEI из EEPROM
//	set_comand(ATCSPN);     //Оператор сети
//--------------------------------------------НАСТРОЙКА BLUETOOTH---------------------------------------------
	//set_comand("AT+BTHOST=DevElectronics");
	//set_comand("AT+BTHOST?");
	set_comand(ATBTPOWER);	//Вкл. питания блютуз
}
////Функция передачи байта по USART1
void USART_GSM(unsigned char Data)
{
	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = Data;
}
//Функция отправки сткроки
//Принимает строку для отправки
void SEND_GSM_str(char * string)
{
	uint8_t i = 0;
	while(string[i])
	{
		USART_GSM(string[i]);
		i++;
	}
}
//Функция разбора строки
void string_parse_gsm(char* buf_str)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)buf_str, strlen(buf_str), 10);
}
void GSM_RxCpltCallback(void)
{
	if(gsm_available()) //если модуль что-то прислал
	{
		uint16_t i = 0;
		uint8_t fdbg = 1;
		memset(buf, 0, GSM_RX_BUFFER_SIZE);
		HAL_Delay(50);

		while(gsm_available())
		{
			buf[i++] = gsm_read();
			if(i > GSM_RX_BUFFER_SIZE - 1) break;
			HAL_Delay(1);
		}
		clear_string(buf); // очищаем строку от символов \r и \n

		/////////////////// НАЧИНАЕМ РАСПОЗНАВАТЬ ЧТО ПРИСЛАЛ МОДУЛЬ /////////////////////
		if(strstr(buf, "RING") != NULL) // ЕСЛИ ЭТО ЗВОНОК
		{
			if(strstr(buf, "9834567891") != NULL) // если звонит нужный номер
			{
				// что-то делаем
				HAL_UART_Transmit(&huart3, (uint8_t*)"My number\n", strlen("My number\n"), 1000);
				//incoming_call(); // можно принять звонок
				//disable_connection(); // сброс соединения
			}
			else
			{
				HAL_UART_Transmit(&huart3, (uint8_t*)"Unknow number\n", strlen("Unknow number\n"), 1000);
				//disable_connection(); // сброс соединения
			}
		}
		else if(strstr(buf, "+CMT:") != NULL) // ЕСЛИ ЭТО SMS
		{
			if(strstr(buf, "9815555596") != NULL) // проверяем от кого смс
			{
				HAL_UART_Transmit(&huart3, (uint8_t*)"Sms my number\n", strlen("Sms my number\n"), 1000);

				// что-то делаем или ищем какую-то строку, которую мы послали в смс, например слово "Hello"
				if(strstr(buf, "Hello") != NULL)
				{
					HAL_UART_Transmit(&huart3, (uint8_t*)"Reciv Hello\n", strlen("Reciv Hello\n"), 1000);
					// что-то делаем
				}
				else if(strstr(buf, "Call") != NULL) // если прилетело слово "Call" то звоним
				{
					//call(); // номер указать в файле gsm.c
				}
				else if(strstr(buf, "Money") != NULL) // если отпрвить sms со словом "Money", то в ответ придёт смс с балансом (деньги) на модеме
				{
					//balance(); // посылаем команду узнать баланс (мтс)
				}
			}
			else
			{
				HAL_UART_Transmit(&huart3, (uint8_t*)"Unknow number sms\n", strlen("Unknow number sms\n"), 1000);
			}
		}
		else if(strstr(buf, "BTCONNECT:") != NULL)		//Сообщение о подключении по bluetooth
		{
			HAL_UART_Transmit(&huart3, (uint8_t*)"Bluetooth connection message\n", 29, 1000);
		}
		else if(strstr(buf, "BTCONNECTING:") != NULL)	//Запрос устройства на подключение по bluetooth
		{
			HAL_UART_Transmit(&huart3, (uint8_t*)"Device request for bluetooth connection\n", 40, 1000);
			//set_comand(ATBTACPT);
			SEND_GSM_str("AT+BTACPT=1\r\n");
		}
		else if(strstr(buf, "BTDISCONN:") != NULL)		//Соединение по bluetooth  отключено
		{
			HAL_UART_Transmit(&huart3, (uint8_t*)"Bluetooth disconnected\n", 23, 1000);
		}
		else if(strstr(buf, "t:PIN1;") != NULL)		//Соединение по bluetooth  отключено
		{
			if(!toggle_pin_test)
			{
				toggle_pin_test = 1;

				SEND_GSM_str("AT+BTSPPSEND=8\r\n");
				HAL_Delay(100);
				SEND_GSM_str("PIN1:SET\r\n");

				HAL_UART_Transmit(&huart3, (uint8_t*)"PIN1:SET\n", 9, 1000);
			}
			else if(toggle_pin_test)
			{
				toggle_pin_test = 0;

				SEND_GSM_str("AT+BTSPPSEND=10\r\n");
				HAL_Delay(100);
				SEND_GSM_str("PIN1:RESET\r\n");

				HAL_UART_Transmit(&huart3, (uint8_t*)"PIN1:RESET\n", 11, 1000);
			}
//			HAL_UART_Transmit(&huart3, (uint8_t*)"Bluetooth disconnected\n", 23, 1000);
		}
//		else if(strstr(buf, "+CUSD") != NULL)  // ЕСЛИ ЭТО СТРОКА С БАЛАНСОМ
//		{
//			char *p = NULL;
//
//			if((p = strstr(buf, "Balance")) != NULL) // ищем слово "Balance"
//			{
//				// отправляем смс с балансом на указанный телефон, укажите нужный номер и раскомментируйте этот блок
//				/*snprintf(str, GSM_RX_BUFFER_SIZE, "AT+CMGS=\"+79839655557\"\r\n"); // номер
//				HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
//				HAL_Delay(100);
//				snprintf(str, GSM_RX_BUFFER_SIZE, "%s", p); // текст смс
//				HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
//				p = 0;
//				HAL_Delay(100);
//				snprintf(str, GSM_RX_BUFFER_SIZE, "%c", (char)26); // символ ctrl-z
//				HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);*/
//				// блок закомментирован чтоб модуль не слал смски пока тестируете
//			}
//		}
//		else if(strstr(buf, "+DTMF") != NULL)  //ЕСЛИ ЭТО DTMF СИГНАЛ
//		{
//			if(strstr(buf, "0") != NULL) // если пришёл сигнал кнопки 0
//			{
//				// что-то делаем
//				HAL_UART_Transmit(&huart3, (uint8_t*)"DTMF Button 0\n", strlen("DTMF Button 0\n"), 1000);
//			}
//			else if(strstr(buf, "1") != NULL) // если пришёл сигнал кнопки 1
//			{
//				// что-то делаем
//				HAL_UART_Transmit(&huart3, (uint8_t*)"DTMF Button 1\n", strlen("DTMF Button 1\n"), 1000);
//			}
//			// и т.д.
//			disable_connection(); // разрываем соединение, или не разрываем (в зависимости от того, что вам нужно)
//		}
//		else if(strstr(buf, "+CCLK") != NULL)  // ЕСЛИ ЭТО ДАТА/ВРЕМЯ
//		{
//			replac_string(buf);
//			char res[32] = {0,};
//
//			for(uint8_t i = 0; i < GSM_RX_BUFFER_SIZE; i++)
//			{
//				if(buf[i] == '"')
//				{
//					i++;
//					for(uint8_t j = 0; j < 20; i++, j++)
//					{
//						if(buf[i] == '+')
//						{
//							buf[i] = 0;
//							break;
//						}
//						if(buf[i] == ',') buf[i] = ' ';
//							res[j] = buf[i];
//					}
//					break;
//				}
//			}
//			snprintf(str, GSM_RX_BUFFER_SIZE, "DateTime %s\n", res);
//			HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);
//			fdbg = 0;
//		}
		if(fdbg)
		{
			snprintf(str, GSM_RX_BUFFER_SIZE, "%s\n", buf);
			HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);
		}
	}
}

