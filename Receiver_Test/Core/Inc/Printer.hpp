/*
 * Printer.hpp
 *
 *  Created on: Jul 25, 2022
 *      Author: wasyl
 */

#ifndef INC_PRINTER_HPP_
#define INC_PRINTER_HPP_

#include "Lap_time_handling.hpp"
#include <cstdlib>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include "main.h"

void Print_time(Time best, Time last, Time latest);
void Print_Data();
void Print_Data_to_application();
void Print_States();
void Select_Screen(uint8_t screen);

#endif /* INC_PRINTER_HPP_ */
