/*
 * input_data.h
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */

#ifndef INC_DIGITAL_SEND_H_
#define INC_DIGITAL_SEND_H_

#include "main.h"

void CheckReWrite();
void set_dido(char *D_IN, uint8_t VAR_IN, char *D_OUT, uint8_t VAR_OUT);

#define VHOD1	GPIOD
#define VHOD2	GPIOD
#define VHOD3	GPIOD
#define VHOD4	GPIOD
#define VHOD5	GPIOD
#define VHOD6	GPIOD
#define VHOD7	GPIOD
#define VHOD8	GPIOD

#define VIHOD1	GPIOA
#define VIHOD2	GPIOB
#define VIHOD3	GPIOB
#define VIHOD4	GPIOE
#define VIHOD5	GPIOE
#define VIHOD6	GPIOE
#define VIHOD7	GPIOE
#define VIHOD8	GPIOE

#endif /* INC_DIGITAL_SEND_H_ */
