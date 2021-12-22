/*
 * input_data.c
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: MMorozov
 */
#include "input_data.h"

//Включить/выключить цифровой выход если цифровой вход = значение(уровень)
//Принимает "d_vhod" - строку с номером цифрового входа
//Принимает "data_a" - переменная состояния входа
//Принимает "d_vihod" - строку с номером цифрового выхода
//Принимает "data_b" - переменная состояния выхода
void set_dido(char *d_vhod, uint8_t data_a, char *d_vihod, uint8_t data_b)
{

}
//Включить/выключить один цифровой выход если аналоговый вход в интервале значений
//Принимает "a_vhod" - строку с номером аналогового входа
//Принимает "data_low" - нижний предел значений
//Принимает "data_high" - верхний предел значений
//Принимает "d_vihod" - строку с номером цифрового выхода
//Принимает "data" - переменная состояния выхода
void set_aido(char *a_vhod, uint8_t data_low, uint8_t data_high, char *d_vihod, uint8_t data)
{

}
//Установить значение в аналоговый выход если цифровой вход = значение(уровень)
//Принимает "d_vihod" - строку с номером цифрового входа
//Принимает "d_data" - значение цифрового входа
//Принимает "a_vihod" - строку с номером аналогового выхода
//Принимает "a_data" - значение аналогового выхода
void set_diao(char *d_vhod, uint8_t d_data, char *a_vihod, uint8_t a_data)
{

}
//Установить значение в аналоговый выход если аналоговый вход = значение
//Принимает "a_vhod" - строку с номером аналогового входа
//Принимает "data_low" - нижний предел значений
//Принимает "data_high" - верхний предел значений
//Принимает "a_vihod" - строку с номером аналогового выхода
//Принимает "data" - значение аналогового выхода
void set_aiao(char *a_vhod, uint8_t data_low, uint8_t data_high, char *a_vihod, uint8_t data)
{

}


void set_data(void)
{

}
