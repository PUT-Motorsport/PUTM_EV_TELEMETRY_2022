/*
 * Lap_time_handling.cpp
 *
 *  Created on: Jul 25, 2022
 *      Author: wasyl
 */

#include "Lap_time_handling.hpp"
#include "Printer.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern uint8_t Cycle_screens;

Time lap_time;
Time Best;
Time Last;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM2)
	{
		lap_time.Update_Time();
		if(Cycle_screens == 2)
		{
			Print_time(lap_time, Best, Last);
		}
	}
}
void Time::Update_Time()
{
	lap_time.miliseconds++;
	if(lap_time.miliseconds == 1000)
	{
		lap_time.seconds++;
		lap_time.miliseconds = 0;
	}
	if(lap_time.seconds == 60)
	{
		lap_time.minute++;
		lap_time.seconds = 0;
	}
}
void End_Lap()
{
	lap_time.Check_best(Best, Last);
	Last = lap_time;
}
