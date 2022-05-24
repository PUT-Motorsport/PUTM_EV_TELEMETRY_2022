/*
 * LEDs.h
 *
 *  Created on: May 21, 2022
 *      Author: wasyl
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

//#include "stm32l4xx_hal.h"
#include "main.h"


void Set_Error();
void Set_OK();
void Set_Radio_Warning();
void Clear_Radio_Warning();
void Set_CAN_Warning();
void Clear_CAN_Warning();
void Clear_All();
void Set_All();
void Start();

#endif /* INC_LEDS_H_ */
