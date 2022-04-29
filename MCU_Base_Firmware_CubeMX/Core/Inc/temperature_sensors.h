#ifndef temperature_sensors_H_
#define temperature_sensors_H_
//--------------------------------------------------
#include "main.h"
//--------------------------------------------------
#define SKIP_ROM 		0
#define NO_SKIP_ROM 	1
//--------------------------------------------------
#define RESOLUTION_9BIT		0x1F
#define RESOLUTION_10BIT 	0x3F
#define RESOLUTION_11BIT 	0x5F
#define RESOLUTION_12BIT 	0x7F
//--------------------------------------------------
void port_init(void);
uint8_t sensors_init(uint8_t mode);
void sensors_MeasureTemperCmd(uint8_t mode, uint8_t DevNum);
void sensors_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum);
uint8_t sensors_GetSign(uint16_t dt);
float sensors_Convert(uint16_t dt);
void UpdateTempSens(void);
//void TempSensMain(void);
//--------------------------------------------------
#endif /* temperature_sensors_H_ */
