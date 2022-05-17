/*
 * Timers.c
 *
 *  Created on: May 10, 2022
 *      Author: Adam Wasilewski
 */

#include "stm32l4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef time1;
RTC_DateTypeDef dt1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Send state and time frame
	if(htim->Instance == TIM2)
	{
		HAL_RTC_GetDate(&hrtc, &dt1, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&hrtc, &time1, RTC_FORMAT_BIN);

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
