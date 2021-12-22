/*
 * usart_ring.c
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */


#include "usart_ring.h"

//----------------------------------------------------GSM USART----------------------------------------------------
volatile gsm_rx_buffer_index_t gsm_rx_buffer_head = 0;
volatile gsm_rx_buffer_index_t gsm_rx_buffer_tail = 0;
uint8_t gsm_rx_buffer[GSM_RX_BUFFER_SIZE] = {0,};

int16_t gsm_available(void)
{
	return ((uint16_t)(GSM_RX_BUFFER_SIZE + gsm_rx_buffer_head - gsm_rx_buffer_tail)) % GSM_RX_BUFFER_SIZE;
}

int16_t gsm_read(void)
{
	if(gsm_rx_buffer_head == gsm_rx_buffer_tail)
	{
		return -1;
	}
	else
	{
		unsigned char c = gsm_rx_buffer[gsm_rx_buffer_tail];
		gsm_rx_buffer_tail = (gsm_rx_buffer_index_t)(gsm_rx_buffer_tail + 1) % GSM_RX_BUFFER_SIZE;
		return c;
	}
}
//-----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------DEBUG USART---------------------------------------------------
volatile dbg_rx_buffer_index_t dbg_rx_buffer_head = 0;
volatile dbg_rx_buffer_index_t dbg_rx_buffer_tail = 0;
uint8_t dbg_rx_buffer[DBG_RX_BUFFER_SIZE] = {0,};

int16_t dbg_available(void)
{
	return ((uint16_t)(DBG_RX_BUFFER_SIZE + dbg_rx_buffer_head - dbg_rx_buffer_tail)) % DBG_RX_BUFFER_SIZE;
}

int16_t dbg_read(void)
{
	if(dbg_rx_buffer_head == dbg_rx_buffer_tail)
	{
		return -1;
	}
	else
	{
		uint8_t c = dbg_rx_buffer[dbg_rx_buffer_tail];
		dbg_rx_buffer_tail = (dbg_rx_buffer_index_t)(dbg_rx_buffer_tail + 1) % DBG_RX_BUFFER_SIZE;
		return c;
	}
}
//-----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------RS485 USART---------------------------------------------------
volatile rs485_rx_buffer_index_t rs485_rx_buffer_head = 0;
volatile rs485_rx_buffer_index_t rs485_rx_buffer_tail = 0;
uint8_t rs485_rx_buffer[RS485_RX_BUFFER_SIZE] = {0,};

int16_t rs485_available(void)
{
	return ((uint16_t)(RS485_RX_BUFFER_SIZE + rs485_rx_buffer_head - rs485_rx_buffer_tail)) % RS485_RX_BUFFER_SIZE;
}

int16_t rs485_read(void)
{
	if(rs485_rx_buffer_head == rs485_rx_buffer_tail)
	{
		return -1;
	}
	else
	{
		uint8_t c = rs485_rx_buffer[rs485_rx_buffer_tail];
		rs485_rx_buffer_tail = (rs485_rx_buffer_index_t)(rs485_rx_buffer_tail + 1) % RS485_RX_BUFFER_SIZE;
		return c;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void EN_Interrupt(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

	//__HAL_SPI_ENABLE_IT(&hspi2, SPI_IT_RXNE);
}
