/*
 * sd_cmd.c
 *
 *  Created on: 6 мая 2022 г.
 *      Author: MMorozov
 */

#include "../../Core/fatfs/Inc/sd_cmd.h"

FATFS FATFS_Obj;	//Инициализация структуры описывающей инициализацию файловой системы

FRESULT result;		//Инициализация структуры описывающей статусы работы карты памяти
FIL test;			//Инициализация структуры описывающей выбранный файл

uint8_t readBuffer[512];	//Буфер для хранения прочитанных с карты данных
uint32_t BytesToRead = 0;	//Буфер для хранения размера файла
uint32_t BytesCounter = 0;	//Счетчик кол-ва прочитанных данных итерируемый пачками readBuffer[512]
UINT readBytes = 0;			//Счетчик кол-ва прочитанных данных
UINT WriteBytes = 0;		//Счетчик кол-ва записанных данных

//Функция инициализации карты памяти
void my_init_card(void)
{
//	SD_PowerOn();
	sd_ini();
}
//Функция чтения файла с карты памяти
void my_read_file(void)
{
	if (f_mount(0, &FATFS_Obj) == FR_OK)	//Монтируем модуль FatFs
	{
		SEND_str("f_mount -> success\n");

		uint8_t path[10]="test.json";
		path[9] = '\0';

		result = f_open(&test, (char*)path, FA_READ);

		if(result == FR_OK)
		{
			SEND_str("f_open -> success\n");

			BytesToRead = test.fsize;

			char str1[60];
			sprintf(str1, "file_Size: %d Byte\n", BytesToRead);
			SEND_str(str1);

			BytesCounter = 0;
			while ((BytesToRead - BytesCounter) >= 512)
		    {
		       	f_read(&test, readBuffer, 512, &readBytes);
		       	BytesCounter += 512;

		       	HAL_UART_Transmit(&huart3, (uint8_t*)readBuffer, strlen(readBuffer), 0x1000);
		    }
		    if (BytesToRead != BytesCounter)
		    {
	        	f_read(&test, readBuffer, (BytesToRead - BytesCounter), &readBytes);

	        	HAL_UART_Transmit(&huart3, (uint8_t*)readBuffer, BytesToRead - BytesCounter, 0x1000);

	        	BytesCounter = BytesToRead;
	        }
//			result = f_read(&test, readBuffer, sizeof(readBuffer), &readBytes);
//			if(result == FR_OK)
//			{
//				SEND_str("f_read -> success\n");
//				SEND_str(readBuffer);
//				SEND_str("\n");
//				sprintf(str1,"BytesToRead: %d\n",readBytes);
//				SEND_str(str1);
//			}
		    f_close(&test);
//		    f_unlink((char*)path);
		}
	}
}
//Функция записи файла на карту памяти
//Принимает "path" - указатель на имя файла
//Принимает "text" - указатель на строку JSON, которую нужно сохранить
void my_write_file_json(char *path, char *text)
{
	if (f_mount(0, &FATFS_Obj) == FR_OK)
	{
		SEND_str("f_mount -> success\n");

		result = f_open(&test, path + '\0', FA_CREATE_ALWAYS|FA_WRITE);

		if(result == FR_OK)
		{
			SEND_str("f_open -> success\n");

			result = f_write(&test, text, strlen(text), &WriteBytes);
			if(result == FR_OK)
			{
				SEND_str("f_write -> success\n");

				char str1[60];
				sprintf(str1, "write_bytes: %d Byte\n", WriteBytes);
				SEND_str(str1);
			}
		    f_close(&test);
		}
	}
}
//Функция сохраниения конфигурационных данных (Включить/выключить цифровой выход(Открытый коллектор) если цифровой вход = значение(уровень))
//Принимает "D_IN" - строку с номером цифрового входа
//Принимает "text" - указатель на строку JSON
void save_dido(char *D_IN, char *text)
{
	char name_FIL[32];

	SEND_str(text);
	sprintf(name_FIL,"%s%s.json", D_IN, "(DiDo)");
	SEND_str(name_FIL);
	my_write_file_json(name_FIL, text);
}
//Функция сохраниения конфигурационных данных (Включить/выключить один цифровой выход(открытый коллектор) если аналоговый вход в интервале значений)
//Принимает "A_IN" - строку с номером аналогового входа
//Принимает "text" - указатель на строку JSON
void save_aido(char *A_IN, char *text)
{
	char name_FIL[32];

	SEND_str(text);
	sprintf(name_FIL,"%s%s.json", A_IN, "(AiDo)");
	SEND_str(name_FIL);
	my_write_file_json(name_FIL, text);
}
//Функция сохраниения конфигурационных данных (Задать сигнал ШИМ на одном выходе)
//Принимает "PWM_OUT" - строку с номером ШИМ выхода
//Принимает "text" - указатель на строку JSON
void save_pwm(char *PWM_OUT, char *text)
{
	char name_FIL[32];

	SEND_str(text);
	sprintf(name_FIL,"%s.json", PWM_OUT);
	SEND_str(name_FIL);
	my_write_file_json(name_FIL, text);
}
//Функция сохраниения конфигурационных данных (Включить/выключить один цифровой выход(открытый коллектор) если температура датчика в интервале значений)
//Принимает "ROM_RAW" - строку с уникальным идентификатором температурного датчика
//Принимает "text" - указатель на строку JSON
void save_tsido(char *ROM_RAW, char *text)
{
	char name_FIL[32];

	SEND_str(text);
	sprintf(name_FIL,"%s%s.json", ROM_RAW, "(TSiDo)");
	SEND_str(name_FIL);
	my_write_file_json(name_FIL, text);
}
