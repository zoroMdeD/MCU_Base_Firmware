//  ******************************************************************************
//  * @file    sd.h 
//  * @author  CIG
//  * @version V1.0.0
//  * @date    
//  * @brief   
//  ******************************************************************************

#ifndef SD_H_
#define SD_H_

#include <diskio.h>
#include <ff.h>
#include <ffconf.h>
#include "main.h"

/* Port Controls */
#define CS_HIGH()	HAL_GPIO_WritePin(GPIOB, CS1__Pin, SET)		//GPIOB->GPIO_BSRR_BS_6		/* MMC CS = H */
#define CS_LOW()	HAL_GPIO_WritePin(GPIOB, CS1__Pin, RESET)	//GPIOB->GPIO_BSRR_BR_6		/* MMC CS = L */

void disk_timerproc(void);

#endif /* SD_H_ */
