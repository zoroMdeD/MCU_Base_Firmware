/*
 * digital.h
 *
 *  Created on: 1 нояб. 2020 г.
 *      Author: moroz
 */

#ifndef INC_DIGITAL_H_
#define INC_DIGITAL_H_

#include "main.h"
//Структура цифровых входов
//Индикаторы срабатываний режимов прерываний;
typedef struct DINn
{
	uint8_t DIN0_interrupt;		//Сработало прерывание по DIN0
	uint8_t DIN1_interrupt;		//Сработало прерывание по DIN1
	uint8_t DIN2_interrupt;		//Сработало прерывание по DIN2
	uint8_t DIN3_interrupt;		//Сработало прерывание по DIN3
	uint8_t DIN4_interrupt;		//Сработало прерывание по DIN4
	uint8_t DIN5_interrupt;		//Сработало прерывание по DIN5
	uint8_t DIN6_interrupt;		//Сработало прерывание по DIN6
	uint8_t DIN7_interrupt;		//Сработало прерывание по DIN7
}
DINn_ptr;

void DINn_ini(void);
void DIGIAL_ini(void);
void ReINIT_DIGITAL(uint8_t flag_it, uint8_t flag_setup);
void DINn_Callback(uint16_t GPIO_Pin);

#endif /* INC_DIGITAL_H_ */
