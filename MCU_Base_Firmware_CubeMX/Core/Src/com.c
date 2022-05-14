/*
 * com.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "main.h"
#include "usart_ring.h"
#include <stdlib.h>

#include "../JSON/Inc/cJSON.h"
#include "../JSON/Inc/input_JSON.h"
#include "../rs485/Inc/rs485.h"

char DBG_buf[DBG_RX_BUFFER_SIZE] = {0,};
char DBG_str[DBG_RX_BUFFER_SIZE] = {0,};

//Функция передачи байта по USART3
//Принимает байт
void USART_Tx(unsigned char Data)
{
	while(!(USART3->SR & USART_SR_TC));
	USART3->DR = Data;
}
//Функция отправки сткроки
//Принимает строку для отправки
void SEND_str(char * string)
{
	uint8_t i = 0;
	while(string[i])
	{
		USART_Tx(string[i]);
		i++;
	}
}
//Функция для отладки через COM порт
void DEBUG_main(void)
{
	if(dbg_available())
	{
		uint16_t i = 0;
		uint8_t fdbg = 1;
		memset(DBG_buf, 0, DBG_RX_BUFFER_SIZE);
		HAL_Delay(50);

		while(dbg_available())
		{
			DBG_buf[i++] = dbg_read();
			if(i > DBG_RX_BUFFER_SIZE - 1)
				break;
			HAL_Delay(1);
		}
		clear_string(DBG_buf);
		if(strstr(DBG_buf, "test") != NULL)
		{
			SEND_str("respone\n");
		}
		else if(strstr(DBG_buf, "AIN") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"ANALOG\",\"SET\":\"[1,1,0,0,1,1,0,0]\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "DIN") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"DIGITAL\",\"SET\":\"[0,0,1,1,0,0,1,1]\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "OCD") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"OCD\",\"SET\":\"[0,1,1,0,0,1,1,0]\"},\"TIME\":\"1122334455\"}");
		}
		//------------------------------------------------Digital-----------------------------------------------
		else if(strstr(DBG_buf, "SP1") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_DIDO\",\"D_IN\":\"VHOD1\",\"VAR_IN\":\"0\",\"D_OUT\":\"VIHOD2\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SP2") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_DIDO\",\"D_IN\":\"VHOD2\",\"VAR_IN\":\"0\",\"D_OUT\":\"VIHOD8\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		//----------------------------------------------End_Digital---------------------------------------------
		//------------------------------------------------Analog------------------------------------------------
		else if(strstr(DBG_buf, "SP3") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_VAIDO\",\"A_IN\":\"VHOD1\",\"RANGE_LOW\":\"2.5\",\"RANGE_HIGH\":\"3.5\",\"D_OUT\":\"VIHOD3\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SPVol") != NULL)		//Voltage
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_VAIDO\",\"A_IN\":\"VHOD1\",\"RANGE_LOW\":\"1.5\",\"RANGE_HIGH\":\"2\",\"D_OUT\":\"VIHOD4\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SPCur") != NULL)	//Current
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_CAIDO\",\"A_IN\":\"VHOD1\",\"RANGE_LOW\":\"0.006\",\"RANGE_HIGH\":\"0.008\",\"D_OUT\":\"VIHOD4\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		//----------------------------------------------End_Analog----------------------------------------------
		//--------------------------------------------------PWM-------------------------------------------------
		else if(strstr(DBG_buf, "SP5") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_PWM\",\"PWM_OUT\":\"PWM1\",\"D_CYCLE\":\"10\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SP6") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_PWM\",\"PWM_OUT\":\"PWM2\",\"D_CYCLE\":\"25\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SP7") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_PWM\",\"PWM_OUT\":\"PWM3\",\"D_CYCLE\":\"50\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SP8") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_PWM\",\"PWM_OUT\":\"PWM4\",\"D_CYCLE\":\"90\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "SP9") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_PWM\",\"PWM_OUT\":\"PWM1\",\"D_CYCLE\":\"50\"},\"TIME\":\"1122334455\"}");
		}
		//------------------------------------------------End_PWM-----------------------------------------------
		//----------------------------------------------TEMPERATURE---------------------------------------------
		else if(strstr(DBG_buf, "TP1") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_TEMP_PROFILE\",\"ROM_RAW\":\"28BF1E930C000031\",\"RANGE_TEMP_LOW\":\"-128\",\"RANGE_TEMP_HIGH\":\"-126\",\"D_OUT\":\"VIHOD3\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "TP2") != NULL)
		{
			json_input("{\"INSTRUCTION\":\"SET_PROGRAMM\",\"COMMAND\":{\"TYPE\":\"SET_TEMP_PROFILE\",\"ROM_RAW\":\"28790E950C000069\",\"RANGE_TEMP_LOW\":\"+28\",\"RANGE_TEMP_HIGH\":\"+50\",\"D_OUT\":\"VIHOD4\",\"VAR_OUT\":\"1\"},\"TIME\":\"1122334455\"}");
		}
		else if(strstr(DBG_buf, "READ_SD") != NULL)
		{
			my_read_file();
		}
		else if(strstr(DBG_buf, "WRITE_SD") != NULL)
		{
//			save_periphery_data();
		}
//		else	//тест для посылки строки через терминал
//		{
//			snprintf(DBG_str, DBG_RX_BUFFER_SIZE, "%s", DBG_buf);
//			SEND_str(DBG_str);
//			json_input(DBG_str);
//		}
		//--------------------------------------------End_TEMPERATURE-------------------------------------------
		if(fdbg)
		{
			snprintf(DBG_str, DBG_RX_BUFFER_SIZE, "%s\n", DBG_buf);

			//json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"DIGITAL\",\"SET\":\"[0,1,1,0,0,0,0,1]\"},\"TIME\":\"1122334455\"}");
		}
	}
}
