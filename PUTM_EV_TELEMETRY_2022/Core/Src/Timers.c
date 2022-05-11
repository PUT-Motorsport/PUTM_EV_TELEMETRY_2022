/*
 * Timers.c
 *
 *  Created on: May 10, 2022
 *      Author: Adam Wasilewski
 */

#include "stm32l4xx_hal.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Send state and time frame
	if(htim->Instance == TIM2)
	{

	}
	//DataBuffer1 timer overflow
	if(htim->Instance == TIM3)
	{

	}
	//DataBuffer2 timer overflow
	if(htim->Instance == TIM4)
	{

	}
	//DataBuffer3 timer overflow
	if(htim->Instance == TIM5)
	{

	}


}
