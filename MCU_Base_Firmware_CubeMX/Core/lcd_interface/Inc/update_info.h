/*
 * spi_interface.h
 *
 *  Created on: 25 нояб. 2020 г.
 *      Author: MMorozov
 */

#ifndef LCD_INTERFACE_INC_UPDATE_INFO_H_
#define LCD_INTERFACE_INC_UPDATE_INFO_H_

#define MCU_OUTPUT			0xC0	//Комманда чтения статусов входов/выходов
#define NOP_MCU				0x00	//Пустая посылка
#define MCU_RELAY_CNT		0xA1	//Кол-во реле блоков
#define MCU_DIGITAL_CNT		0xA2	//Кол-во цифровых блоков
#define MCU_DIMMING_CNT		0xA3	//Кол-во диммирования блоков
#define MCU_INTERFACE_CNT	0xA4	//Кол-во интерфейсных блоков
#define MCU_RELAY_ADR		0xB1	//Адреса реле блоков
#define MCU_DIGITAL_ADR		0xB2	//Адреса цифровых блоков
#define MCU_DIMMING_ADR		0xB3	//Адреса диммирования блоков
#define MCU_INTERFACE_ADR	0xB4	//Адреса интерфейсных блоков

//------------------------Открытый коллектор------------------------
#define OC0_ON		0x01
#define OC0_OFF		0x02
#define OC1_ON		0x03
#define OC1_OFF		0x04
#define OC2_ON		0x05
#define OC2_OFF		0x06
#define OC3_ON		0x07
#define OC3_OFF		0x08
#define OC4_ON		0x09
#define OC4_OFF		0x10
#define OC5_ON		0x11
#define OC5_OFF		0x12
#define OC6_ON		0x13
#define OC6_OFF		0x14
#define OC7_ON		0x15
#define OC7_OFF		0x16
//------------------------------------------------------------------
//--------------------------Цифровые входы--------------------------
#define DIN0_R		0x17
#define DIN0_F		0x18
#define DIN1_R		0x19
#define DIN1_F		0x20
#define DIN2_R		0x21
#define DIN2_F		0x22
#define DIN3_R		0x23
#define DIN3_F		0x24
#define DIN4_R		0x25
#define DIN4_F		0x26
#define DIN5_R		0x27
#define DIN5_F		0x28
#define DIN6_R		0x29
#define DIN6_F		0x30
#define DIN7_R		0x31
#define DIN7_F		0x32
//------------------------------------------------------------------
//-------------------------Аналоговые входы-------------------------
#define AIN0_ON		0x33
#define AIN0_OFF	0x34
#define AIN1_ON		0x35
#define AIN1_OFF	0x36
#define AIN2_ON		0x37
#define AIN2_OFF	0x38
#define AIN3_ON		0x39
#define AIN3_OFF	0x40
#define AIN4_ON		0x41
#define AIN4_OFF	0x42
#define AIN5_ON		0x43
#define AIN5_OFF	0x44
#define AIN6_ON		0x45
#define AIN6_OFF	0x46
#define AIN7_ON		0x47
#define AIN7_OFF	0x48
//------------------------------------------------------------------
//----------------------------Выходы ШИМ----------------------------
#define PWM0_ON		0x49
#define PWM0_OFF	0x50
#define PWM1_ON		0x51
#define PWM1_OFF	0x52
#define PWM2_ON		0x53
#define PWM2_OFF	0x54
#define PWM3_ON		0x55
#define PWM3_OFF	0x56
//------------------------------------------------------------------
//--------------------------Выходы 1-Wire---------------------------
#define WR0_ON		0x57
#define WR0_OFF		0x58
#define WR1_ON		0x59
#define WR1_OFF		0x60
//------------------------------------------------------------------
//----------------------------GSM Modem-----------------------------
#define GSM_ON		0x61
#define GSM_OFF		0x62
//------------------------------------------------------------------
//------------------Проверка подключения модулей--------------------
#define RELAY_TEST		0x63
#define DIMMING_TEST	0x64
#define DIGITAL_TEST	0x65
#define INTERFACE_TEST	0x66
//------------------------------------------------------------------
//----------------------Запрос даты и времени-----------------------
#define MCU_TIME	0x67
//------------------------------------------------------------------
void ReWriteOCD(void);
void ReWriteDIN(void);
void ReWriteAIN(void);
void ReWritePWM(void);
void ReWrite1Wire(void);
void SPI_available(void);

#endif /* LCD_INTERFACE_INC_UPDATE_INFO_H_ */
