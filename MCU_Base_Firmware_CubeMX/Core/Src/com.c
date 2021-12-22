/*
 * com.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "main.h"
#include "rs485.h"
#include "usart_ring.h"
#include <stdlib.h>
#include "cJSON.h"
#include "input_JSON.h"

char DBG_buf[DBG_RX_BUFFER_SIZE] = {0,};
char DBG_str[DBG_RX_BUFFER_SIZE] = {0,};

//Функция передачи байта по USART3
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
void DEBUG_main(void)
{
	if(dbg_available()) //если послали в терминал какую-то команду, то она перенаправиться в модем
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

			//HAL_UART_Transmit(&huart3, INSTRUCTION, strlen(INSTRUCTION), 0x1000);

			// что-то делаем
				//HAL_UART_Transmit(&huart3, (uint8_t*)"My number\n", strlen("My number\n"), 1000);
				//incoming_call(); // можно принять звонок
				//disable_connection(); // сброс соединения
		}
//		clear_string(DBG_buf);
//		snprintf(DBG_str, DBG_RX_BUFFER_SIZE, "%s\r\n", DBG_buf);
//		c = strchr(DBG_str, '.') - DBG_str;
//		HAL_UART_Transmit(&huart2, (uint8_t*)DBG_str, c, 1000);	//strlen(DBG_str)

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
			json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"OCD\",\"SET\":\"[0,1,0,1,0,1,1,1]\"},\"TIME\":\"1122334455\"}");
		}

		if(fdbg)
		{
			snprintf(DBG_str, DBG_RX_BUFFER_SIZE, "%s\n", DBG_buf);

			//json_input("{\"INSTRUCTION\":\"SET_PERIPHERALS\",\"COMMAND\":{\"TYPE\":\"DIGITAL\",\"SET\":\"[0,1,1,0,0,0,0,1]\"},\"TIME\":\"1122334455\"}");
		}
	}
}
