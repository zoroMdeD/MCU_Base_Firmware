/*
 * delay.h
 *
 *  Created on: 20 апр. 2022 г.
 *      Author: mmorozov
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

//#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
//#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
//#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

void DWT_Init(void);
void delay_micros(uint32_t us);

#endif /* INC_DELAY_H_ */
