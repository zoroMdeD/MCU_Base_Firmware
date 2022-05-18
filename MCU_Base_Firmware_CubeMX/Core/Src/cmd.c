/*
 * cmd.c
 *
 *  Created on: 15 нояб. 2020 г.
 *      Author: moroz
 */
#include "main.h"

//Функция обновления прошивки по USB
//void UPD_USB(char *Bytes )
//{
//	if(strcmp(Bytes,"BeginUPD") == 0)
//	{
////		json_input("{\"INSTRUCTION\":\"UPDATE_FIRMWARE\",\"COMMAND\":{\"TYPE\":\"SETTING_FIRMWARE\",\"NAME\":\"test_firmware\",\"VERSION\":\"v.0.0.1\",\"SIZE\":\"1272\"},\"TIME\":\"1122334455\"}");
////		check_UPD_FW = true;
////		SEND_str("start\n");
//	}
//	else if(strcmp(Bytes,"EndUPD") == 0)
//	{
//		check_UPD_FW = false;
//		fl_close();
//		SEND_str("end\n");
//		HAL_NVIC_SystemReset();		//Перезапускаем контроллер
//	}
//	else if(check_UPD_FW)
//	{
//		//Запись посылки но 1024 байт, прикрепляем к концу посылки еще 4 байта контрольной суммы, итого 1028 байт в посылки каждый раз.
//		//Буфер можно попробовать увеличить
//		//Следующую посылку посылать с сервера только после получения обратного сообщения что контрольная сумма сошлась
//		//Если посылка последнего пакета байт получается не кратная 4 то ее необходимо дополнить системными единицами памяти(FFh), до 1024 байт + 4 байта CRC = 1028 байт
//		char tmp_crc32[9];
//		sprintf(tmp_crc32, "%02X%02X%02X%02X", Bytes[1024], Bytes[1025], Bytes[1026] ,Bytes[1027]);	//Вытаскиваем последние 4 байта(CRC16)
//		char *pEnd;
//		uint32_t crc32 = (uint32_t)(strtol(tmp_crc32, &pEnd, 16));
//		SEND_str(my_write_file_firmware(SetFW.NAME, Bytes, crc32));		//atoi(tmp_crc16)
//	}
//}
////Функция обновления прошивки по Ethernet
//void UPD_ETH(void)
//{
//
//}
