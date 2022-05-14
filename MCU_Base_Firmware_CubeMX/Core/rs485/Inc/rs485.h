/*
 * rs485.h
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */

#ifndef RS485_INC_RS485_H_
#define RS485_INC_RS485_H_

void USART_RS485(unsigned char Data);
void SEND_RS485_str(char * string);

void RS485_main(void);

#endif /* RS485_INC_RS485_H_ */
