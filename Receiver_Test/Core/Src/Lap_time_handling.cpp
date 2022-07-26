/*
 * Lap_time_handling.cpp
 *
 *  Created on: Jul 25, 2022
 *      Author: wasyl
 */

#include "Lap_time_handling.hpp"

static Time best;
static Time last;

void Handle_new_time(uint8_t RxData[])
{
	uint8_t minutes = RxData[1];
	uint8_t seconds = RxData[2];
	uint16_t miliseconds = RxData[3] << 8 | RxData[4];
	last.Override_last(minutes, seconds, miliseconds);
	last.Check_best(best, last);
}
