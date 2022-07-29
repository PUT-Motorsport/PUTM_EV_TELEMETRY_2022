/*
 * Lap_time_handling.hpp
 *
 *  Created on: Jul 25, 2022
 *      Author: Adam Wasilewski
 */

#ifndef INC_LAP_TIME_HANDLING_HPP_
#define INC_LAP_TIME_HANDLING_HPP_

#include "main.h"
#include <cstdlib>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format-inl.h>

#define INVALID_LAP_TIME  2
class Time{
	private:
		uint8_t  minute : 3;
		uint8_t  seconds;
		uint16_t miliseconds : 10;
	public:
		Time()
		{
			minute = 0;
			seconds = 0;
			miliseconds = 0;
		}
		void Update_Time();
		void Check_best(Time Best, Time Last);
		void Override_last(Time lap_time, Time Last);
		void Reset_time();
		void Timeout();
		uint8_t return_minutes(){return minute;}
		uint8_t return_seconds(){return seconds;}
		uint16_t return_miliseconds(){return miliseconds;}

};
void End_Lap();

#endif /* INC_LAP_TIME_HANDLING_HPP_ */
