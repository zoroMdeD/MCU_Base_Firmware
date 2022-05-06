/*
 * sd_cmd.c
 *
 *  Created on: 6 мая 2022 г.
 *      Author: MMorozov
 */

#include "../../Core/fatfs/Inc/sd_cmd.h"

FATFS FATFS_Obj;

FRESULT result;
FIL test;

uint8_t readBuffer[512];
uint32_t BytesToRead = 0;
uint32_t BytesCounter = 0;
//	uint32_t currentAddress = 0;
UINT readBytes = 0;
UINT WriteBytes = 0;

void my_init_card(void)
{
//	SD_PowerOn();
	sd_ini();
}

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
void my_write_file(void)
{
	if (f_mount(0, &FATFS_Obj) == FR_OK)	//Монтируем модуль FatFs
	{
		SEND_str("f_mount -> success\n");

		uint8_t path[12]="test_wr.txt";
		path[11] = '\0';

		result = f_open(&test, (char*)path, FA_CREATE_ALWAYS|FA_WRITE);

		if(result == FR_OK)
		{
			SEND_str("f_open -> success\n");

			result = f_write(&test, "\nTesting write on SD card", 25, &WriteBytes);
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
