/*
 * usart_ring.h
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */

#ifndef INC_USART_RING_H_
#define INC_USART_RING_H_

#include "main.h"

//----------------------------------------------------GSM USART----------------------------------------------------
#define GSM_RX_BUFFER_SIZE 64

#if (GSM_RX_BUFFER_SIZE > 256)
	typedef uint16_t gsm_rx_buffer_index_t;
#else
	typedef uint8_t gsm_rx_buffer_index_t;
#endif

int16_t gsm_available(void);
int16_t gsm_read(void);

//-----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------DEBUG USART---------------------------------------------------
#define DBG_RX_BUFFER_SIZE 256

#if (DBG_RX_BUFFER_SIZE > 256)
	typedef uint16_t dbg_rx_buffer_index_t;
#else
	typedef uint8_t dbg_rx_buffer_index_t;
#endif

int16_t dbg_available(void);
int16_t dbg_read(void);
//-----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------RS485 USART---------------------------------------------------
#define RS485_RX_BUFFER_SIZE 64

#if (RS485_RX_BUFFER_SIZE > 256)
	typedef uint16_t rs485_rx_buffer_index_t;
#else
	typedef uint8_t rs485_rx_buffer_index_t;
#endif

int16_t rs485_available(void);
int16_t rs485_read(void);

//-----------------------------------------------------------------------------------------------------------------

#endif /* INC_USART_RING_H_ */
