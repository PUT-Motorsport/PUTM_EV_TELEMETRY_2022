/*
 * Lap_time_handling.cpp
 *
 *  Created on: Jul 25, 2022
 *      Author: Adam Wasilewski
 */
#include "Lap_time_handling.hpp"
#include "Printer.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern uint8_t Cycle_screens;
extern uint8_t Laps_tracker;

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
			Print_time(lap_time, Best, Last, Laps_tracker);
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
	if(lap_time.minute > INVALID_LAP_TIME)
	{
		lap_time.Timeout();
	}
}
void Time::Check_best(Time Best, Time Last)
{
	if(Last.minute < Best.minute)
	{
		Best = Last;
	}
	else if(Last.minute == Best.minute)
	{
		if(Last.seconds < Best.seconds)
		{
			Best = Last;
		}
		else if(Last.seconds == Best.seconds)
		{
			if(Last.miliseconds < Best.miliseconds)
			{
				Best = Last;
			}
		}
	}
}
void Time::Reset_time()
{
	this->minute = 0;
	this->seconds = 0;
	this->miliseconds = 0;
}
void Time::Timeout()
{
	lap_time.Reset_time();
	HAL_TIM_Base_Stop_IT(&htim2);
}
void End_Lap()
{
	Last = lap_time;
	lap_time.Check_best(Best, Last);
	lap_time.Reset_time();
}
