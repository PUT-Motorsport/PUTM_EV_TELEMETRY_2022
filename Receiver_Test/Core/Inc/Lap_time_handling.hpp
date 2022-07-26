/*
 * Lap_time_handling.hpp
 *
 *  Created on: Jul 25, 2022
 *      Author: wasyl
 */

#ifndef INC_LAP_TIME_HANDLING_HPP_
#define INC_LAP_TIME_HANDLING_HPP_

#include "main.h"
#include <cstdlib>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format-inl.h>

class Time{
	private:
		uint8_t  minute : 3;
		uint8_t  seconds : 5;
		uint16_t miliseconds : 10;
	public:
		Time()
		{
			minute = 0;
			seconds = 0;
			miliseconds = 0;
		}
		void Check_best(Time best, Time last)
		{


		}
		void Override_last(uint8_t minutes, uint8_t seconds, uint16_t miliseconds)
		{


		}

};
void Handle_new_time(uint8_t RxData[]);

#endif /* INC_LAP_TIME_HANDLING_HPP_ */
