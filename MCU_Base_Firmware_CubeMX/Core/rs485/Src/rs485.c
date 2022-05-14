/*
 * rs485.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */
#include "../rs485/Inc/rs485.h"

#include "main.h"
#include "usart_ring.h"
#include <stdlib.h>

#include "../JSON/Inc/cJSON.h"
//#define RS485 &huart2

//extern UART_HandleTypeDef huart2;

char rs485_buf[RS485_RX_BUFFER_SIZE] = {0,};
char rs485_str[RS485_RX_BUFFER_SIZE] = {0,};

//Функция передачи байта по USART2
void USART_RS485(unsigned char Data)
{
	while(!(USART2->SR & USART_SR_TC));
	USART2->DR = Data;
}
//Функция отправки сткроки
//Принимает строку для отправки
void SEND_RS485_str(char * string)
{
	uint8_t i = 0;
	while(string[i])
	{
		USART_RS485(string[i]);
		i++;
	}
}
void RS485_main(void)
{
	if(rs485_available())
	{
		uint16_t i = 0;
		uint8_t fdbg_rs485 = 1;
		memset(rs485_buf, 0, RS485_RX_BUFFER_SIZE);
		HAL_Delay(50);

		while(rs485_available())
		{
			rs485_buf[i++] = rs485_read();
			if(i > RS485_RX_BUFFER_SIZE - 1)
				break;
			HAL_Delay(1);
		}
		clear_string(rs485_buf);
		if(strstr(rs485_buf, "test") != NULL)
		{
			SEND_str("respone\n");
			// что-то делаем
				//HAL_UART_Transmit(&huart3, (uint8_t*)"My number\n", strlen("My number\n"), 1000);
				//incoming_call(); // можно принять звонок
				//disable_connection(); // сброс соединения
		}
//		clear_string(rs485_buf);
//		snprintf(rs485_str, RS485_RX_BUFFER_SIZE, "%s\r\n", rs485_buf);
//		c = strchr(rs485_str, '.') - rs485_str;
//		HAL_UART_Transmit(&huart2, (uint8_t*)rs485_str, c, 1000);	//strlen(rs485_str)

		if(fdbg_rs485)
		{
			snprintf(rs485_str, RS485_RX_BUFFER_SIZE, "%s\n", rs485_buf);
			HAL_UART_Transmit(&huart3, (uint8_t*)rs485_str, strlen(rs485_str), 1000);
		}
	}
}
