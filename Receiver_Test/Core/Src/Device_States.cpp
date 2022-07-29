/*
 * Device_States.cpp
 *
 *  Created on: Jul 28, 2022
 *      Author: wasyl
 */


#include "Device_States.hpp"
#include "Parsers.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>

std::string Return_apps_state(Apps_states state)
{
	switch(state)
	{
	case Apps_states::Power_up: return "Power up";
	case Apps_states::Normal_operation: return "Normal_operation";
	case Apps_states::Sensor_Implausiblity: return "Sensor Implausiblity";
	case Apps_states::Left_sensor_out_of_range_lower_bound: return "Left_sensor_out_of_range_lower_bound";
	case Apps_states::Left_sensor_out_of_range_upper_bound: return "Left_sensor_out_of_range_upper_bound";
	case Apps_states::Right_sensor_out_of_range_lower_bound: return "Right_sensor_out_of_range_lower_bound";
	case Apps_states::Right_sensor_out_of_range_upper_bound: return "Right_sensor_out_of_range_upper_bound";
	default: return "error";
	}
	return 0;
}
std::string Return_lv_state(BMS_LV_states state)
{
	switch(state)
	{
	case BMS_LV_states::Normal: return "Normal";
	}
	return 0;
}
