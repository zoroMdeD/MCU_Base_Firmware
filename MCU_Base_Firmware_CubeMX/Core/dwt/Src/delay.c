/*
 * delay.c
 *
 *  Created on: 20 апр. 2022 г.
 *      Author: mmorozov
 */
#include "main.h"

//Функция инициализации DWT таймера
void DWT_Init(void)
{
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	//Разрешаем использовать счётчик
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;		//Запускаем счётчик
}
//Функция временной задержки в мкс
//Принимает "us" - значение в микросекундах
void delay_micros(uint32_t us)
{
    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000);	//Получаем кол-во тактов за 1 мкс и умножаем на наше значение
    DWT->CYCCNT = 0U; 											//Обнуляем счётчик
    while(DWT->CYCCNT < us_count_tic);
}
