#include "../Inc/temperature_sensors.h"
//--------------------------------------------------
uint8_t LastDeviceFlag;
uint8_t LastDiscrepancy;
uint8_t LastFamilyDiscrepancy;
uint8_t ROM_NO[8];
uint8_t Dev_ID[AMT_TEMP_SENS][8];
uint8_t Dev_Cnt;
char Device_RAW_ROM[AMT_TEMP_SENS][20];

#define SET_LOW				GPIOD->ODR &= ~GPIO_ODR_ODR_0
#define SET_HIGH			GPIOD->ODR |= GPIO_ODR_ODR_0
#define GET_STATUS_PIN		GPIOD->IDR & GPIO_IDR_IDR_0

extern bool OneWire_Test_Flag_Init;
extern bool OneWire_Test_Flag_Read;

float temper;

void port_init(void)
{
	HAL_GPIO_DeInit(GPIOD, WR0_Pin);
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = WR0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
//--------------------------------------------------
uint8_t sensors_Reset(void)
{
	uint16_t status;

	SET_LOW;					//низкий уровень
	delay_micros(480);			//задержка как минимум на 480 микросекунд
	SET_HIGH;					//высокий уровень
	delay_micros(60);			//задержка как минимум на 60 микросекунд
	status = GET_STATUS_PIN;	//провер¤ем уровень
	delay_micros(480);			//задержка как минимум на 480 микросекунд
  	  	  	  	  	  	  		//(на всякий случай подождЄм побольше, так как могут быть неточности в задержке)
	return (status ? 1 : 0);	//вернЄм результат
}
//----------------------------------------------------------
uint8_t sensors_ReadBit(void)
{
	uint8_t bit = 0;

	SET_LOW;							//низкий уровень
	delay_micros(1);
	SET_HIGH;							//высокий уровень
	delay_micros(14);
	bit = (GET_STATUS_PIN ? 1 : 0);		//провер¤ем уровень
	delay_micros(45);

	return bit;
}
//-----------------------------------------------
uint8_t sensors_ReadByte(void)
{
	uint8_t data = 0;

	for (uint8_t i = 0; i <= 7; i++)
		data += sensors_ReadBit() << i;

	return data;
}
//-----------------------------------------------
void sensors_WriteBit(uint8_t bit)
{
	SET_LOW;
	delay_micros(bit ? 1 : 60);
	SET_HIGH;
	delay_micros(bit ? 60 : 1);
}
//-----------------------------------------------
void sensors_WriteByte(uint8_t dt)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		sensors_WriteBit(dt >> i & 1);
		//Delay Protection
		delay_micros(5);
	}
}
//-----------------------------------------------
uint8_t sensors_SearchRom(uint8_t *Addr)
{
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	uint8_t rom_byte_mask, search_direction;

	//проинициализируем переменные
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;
	if (!LastDeviceFlag)
	{
		sensors_Reset();
		sensors_WriteByte(0xF0);
	}
	do
	{
		id_bit = sensors_ReadBit();
		cmp_id_bit = sensors_ReadBit();
		if ((id_bit == 1) && (cmp_id_bit == 1))
			break;
		else
		{
			if (id_bit != cmp_id_bit)
				search_direction = id_bit; // bit write value for search
			else
			{
				if (id_bit_number < LastDiscrepancy)
					search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
				else
					search_direction = (id_bit_number == LastDiscrepancy);
				if (search_direction == 0)
				{
					last_zero = id_bit_number;
					if (last_zero < 9)
					LastFamilyDiscrepancy = last_zero;
				}
			}
			if (search_direction == 1)
				ROM_NO[rom_byte_number] |= rom_byte_mask;
			else
				ROM_NO[rom_byte_number] &= ~rom_byte_mask;
			sensors_WriteBit(search_direction);
			id_bit_number++;
			rom_byte_mask <<= 1;
			if (rom_byte_mask == 0)
			{
				rom_byte_number++;
				rom_byte_mask = 1;
			}
		}
	} while(rom_byte_number < 8);		//считываем байты с 0 до 7 в цикле

	if (!(id_bit_number < 65))
	{
		// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
		LastDiscrepancy = last_zero;
		// check for last device
		if (LastDiscrepancy == 0)
			LastDeviceFlag = 1;
		search_result = 1;	
	}
	if (!search_result || !ROM_NO[0])
	{
		LastDiscrepancy = 0;
		LastDeviceFlag = 0;
		LastFamilyDiscrepancy = 0;
		search_result = 0;
	}
	else
	{
		for (int i = 0; i < 8; i++)
			Addr[i] = ROM_NO[i];
	}
	return search_result;
}
//-----------------------------------------------
uint8_t sensors_init(uint8_t mode)
{
	int i = 0, j=0;
	uint8_t dt[8];
	if(mode == SKIP_ROM)
	{
		if(sensors_Reset())
			return 1;
		//SKIP ROM
		sensors_WriteByte(0xCC);
		//WRITE SCRATCHPAD
		sensors_WriteByte(0x4E);
		//TH REGISTER 100 градусов
		sensors_WriteByte(0x64);
		//TL REGISTER - 30 градусов
		sensors_WriteByte(0x9E);
		//Resolution 12 bit
		sensors_WriteByte(RESOLUTION_12BIT);
	}
	else
	{
		for(i = 1; i <= 8; i++)
		{
			if(sensors_SearchRom(dt))
			{
				Dev_Cnt++;
				memcpy(Dev_ID[Dev_Cnt-1], dt, sizeof(dt));
			}
			else break;
		}
		for(i = 1; i <= Dev_Cnt; i++)
		{
			if(sensors_Reset())
				return 1;
			//Match Rom
			sensors_WriteByte(0x55);
			for(j = 0; j <= 7; j++)
			{
				sensors_WriteByte(Dev_ID[i-1][j]);
			}
			//WRITE SCRATCHPAD
			sensors_WriteByte(0x4E);
			//TH REGISTER 100 градусов
			sensors_WriteByte(0x64);
			//TL REGISTER - 30 градусов
			sensors_WriteByte(0x9E);
			//Resolution 12 bit
			sensors_WriteByte(RESOLUTION_12BIT);
		}
	}
	return 0;
}
//----------------------------------------------------------
void sensors_MeasureTemperCmd(uint8_t mode, uint8_t DevNum)
{
	int i = 0;
	sensors_Reset();
	if(mode == SKIP_ROM)
	{
    //SKIP ROM
		sensors_WriteByte(0xCC);
	}
	else
	{
		//Match Rom
		sensors_WriteByte(0x55);
		for(i = 0; i <= 7; i++)
		{
			sensors_WriteByte(Dev_ID[DevNum-1][i]);
		}
	}
	//CONVERT T
	sensors_WriteByte(0x44);
}
//----------------------------------------------------------
void sensors_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum)
{
	uint8_t i;
	sensors_Reset();
	if(mode == SKIP_ROM)
	{
		//SKIP ROM
		sensors_WriteByte(0xCC);
	}
	else
	{
		//Match Rom
		sensors_WriteByte(0x55);
		for(i = 0; i <= 7; i++)
		{
			sensors_WriteByte(Dev_ID[DevNum-1][i]);
		}
	}
	//READ SCRATCHPAD
	sensors_WriteByte(0xBE);
	for(i = 0; i < 8; i++)
	{
		Data[i] = sensors_ReadByte();
	}
}
//----------------------------------------------------------
uint8_t sensors_GetSign(uint16_t dt)
{
	//ѕроверим 11-й бит
	if (dt&(1<<11))
		return 1;
	else
		return 0;
}
//----------------------------------------------------------
float sensors_Convert(uint16_t dt)
{
	float t;

	t = (float)((dt&0x07FF)>>4);		//отборосим знаковые и дробные биты
	t += (float)(dt&0x000F) / 16.0f;	//ѕрибавим дробную часть

	return t;
}
//----------------------------------------------------------
void UpdateTempSens(void)
{
	sensors_init(NO_SKIP_ROM);

	for(uint8_t i = 0; i < Dev_Cnt; i++)
	{
		sprintf(Device_RAW_ROM[i],"%02X%02X%02X%02X%02X%02X%02X%02X", Dev_ID[i][0], Dev_ID[i][1], Dev_ID[i][2], Dev_ID[i][3], Dev_ID[i][4], Dev_ID[i][5], Dev_ID[i][6], Dev_ID[i][7]);
		//----------------------from debug---------------------------
		if(strcmp(Device_RAW_ROM[i], "28790E950C000069") == 0)
		{
			SEND_str(Device_RAW_ROM[i]);
			SEND_str(" - YES\n");
		}
		if(strcmp(Device_RAW_ROM[i], "28BF1E930C000031") == 0)
		{
			SEND_str(Device_RAW_ROM[i]);
			SEND_str(" - YES\n");
		}
		//-----------------------------------------------------------
	}

	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_SR_UIF); // очищаем флаг
	HAL_TIM_Base_Start_IT(&htim6);
}
