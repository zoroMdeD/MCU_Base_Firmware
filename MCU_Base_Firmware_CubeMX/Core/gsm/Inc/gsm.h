/*
 * gsm.h
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */

#ifndef GSM_INC_GSM_H_
#define GSM_INC_GSM_H_

#include "string.h"
#include "stdio.h"

//------------------------------------------------ИНФОРМАЦИОННЫЕ ДЕФАЙНЫ------------------------------------------------
#define ATCPAS		"AT+CPAS" 		//Проверка статуса модема, ответ — (+CPAS: 0), (0 — готов к работе, 2 — неизвестно, 3 — входящий звонок, 4 — в режиме соединения)
#define ATCREG		"AT+CREG?" 		//Регистрация в сети, интересен второй параметр в ответе - (+CREG: 0,1), 0 - не зарегистрирован в сети, 1 - зерегистрированно, 2 - не зарегистрированно идет поиск сети, 3 - регистрация отклонена, 4 - неизвестно, 5 - зарегистрированно роуминг
#define ATI			"ATI" 			//Название и версия модуля
#define ATCGSN 		"AT+CGSN" 		//Считывание IMEI из EEPROM
#define ATCSPN 		"AT+CSPN?" 		//Оператор
#define ATIPR 		"AT+IPR?" 		//Узнать скорость порта
#define ATCSQ 		"AT+CSQ" 		//Уровень сигнала. Может быть в диапазоне 0..31, чем больше тем лучше. 99 значит отсутствие сигнала.

//---------------------------------------------------ДЕФАЙНЫ НАСТРОЙКИ--------------------------------------------------
#define ATCLIP1 	"AT+CLIP=1" 							//Включить АОН, 0 - отключить
#define ATE 		"ATE0" 									//Отключить «эхо», 1 - включить
#define ATCCLKK 	"AT+CCLK=\"19/08/21,14:03:00+00\"" 		//Установить дату/время - yy/mm/dd,hh:mm:ss+zz
#define ATIPRS 		"AT+IPR=57600" 							//Установить скорость порта
#define ATS 		"ATS0=0" 								//Поднимать трубку только "вручную", ATS0=2 - поднимать трубку после двух гудков, ATS0=3 - псле трёх и т.д.
#define ATGSMBUSY 	"AT+GSMBUSY=1" 							//Запрет всех входящих звонков. 0 — разрешены.
#define ATDDET 		"AT+DDET=1" 							//Включить DTMF, 0 - отключить.
#define ATCMGF 		"AT+CMGF=1" 							//Устанавливает текстовый режим смс-сообщения
#define ATCPBS 		"AT+CPBS=\"SM\"" 						//Открывает доступ к данным телефонной книги SIM-карты
#define ATCSCS 		"AT+CSCS=\"GSM\"" 						//Кодировка текста - GSM
#define ATCNMI 		"AT+CNMI=1,2,2,1,0" 					//Настройка вывода смс в консоль

#define ATBTPOWER 	"AT+BTPOWER=1"							//Вкл. питания блютуз
#define ATBTACPT 	"AT+BTACPT=1"							//Согласие на подключение удаленного устройства
#define ATBTSPPSEND "AT+BTSPPSEND=" 						//Кол-во отправляемых байт в сообщении

void EN_Interrupt(void);
void replac_string(char *src);
void set_comand(char *buff);
void chek_speed(void);
void clear_string(char *src);
void INIT_GSM(void);

void GSM_RxCpltCallback(void);

typedef struct USART_gsm{
  uint8_t usart_buf[26];
  uint8_t usart_cnt;
  uint8_t is_tcp_connect;	//статус попытки создать соединение TCP с сервером
  uint8_t is_text;			//статус попытки передать текст серверу
} USART_gsm_ptr;





//void USART_GSM(unsigned char Data);
//void SEND_GSM_str(char * string);

#endif /* GSM_INC_GSM_H_ */
