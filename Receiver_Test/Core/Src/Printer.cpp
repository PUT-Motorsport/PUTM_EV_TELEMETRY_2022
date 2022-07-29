/*
 * Printer.cpp
 *
 *  Created on: Jul 25, 2022
 *      Author: Adam Wasilewski
 */

#include "Printer.hpp"
#include "Parsers.hpp"
#include "Device_States.hpp"

extern UART_HandleTypeDef hlpuart1;

extern msg65 ms65;
extern msg66 ms66;
extern msg67 ms67;
extern msg68 ms68;
extern msg70 ms70;
extern Device_States states;

void Select_Screen(uint8_t screen)
{
	switch(screen)
	{
	case 0:
		Print_Data();
		break;
	case 1:
		Print_States();
		break;
	case 2:
		//FIXME: Handle time printing from here rather then directly form timer interrupt;
		//Print_time();
		break;
	}
}
void Print_Data()
{
	auto out = fmt::memory_buffer();

	format_to(std::back_inserter(out),"APPS:{:05} LVV:{:05} LV SoC:{:03} HVV:{:05} HV SoC:{:03} HV T:{:03} HV Curr:{:03} BP Front:{:05} Speed:{:05} Motor curr:{:05} Engine V:{:05} Inverter T:{:03}\n\r",
	   ms65.Pedal_Position,ms65.LV_Voltage,ms65.LV_Soc,ms65.HV_Voltage, ms65.HV_SoC, ms65.HV_Temps, ms65.HV_Current,ms66.brake_pressure_front,ms67.vehicle_speed,ms67.motor_current, ms67.engine_speed, ms68.inverter_temp);

	auto data = out.data(); // pointer to the formatted data
	auto size = out.size(); // size of the formatted data

	//HAL_Delay(10);

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 100);
}
void Print_time(Time lap_time, Time Best, Time Last, uint8_t Laps_tracker)
{
	auto time_screen = fmt::memory_buffer();
	format_to(std::back_inserter(time_screen), "Lap Time:{}:{:02}:{:04} Last Lap:{}:{:02}:{:04} Best Lap:{}:{:02}:{:04} Lap count:{}\n\r",
			lap_time.return_minutes(), lap_time.return_seconds(), lap_time.return_miliseconds(),
			Last.return_minutes(), Last.return_seconds(), Last.return_miliseconds(),
			Best.return_minutes(), Best.return_seconds(), Best.return_miliseconds(), Laps_tracker);
	auto data = time_screen.data(); // pointer to the formatted data
	auto size = time_screen.size(); // size of the formatted data

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1);
}

void Print_States()
{
	auto out = fmt::memory_buffer();
	format_to(std::back_inserter(out),"APPS: {}", Return_apps_state(states.apps));
	auto data = out.data(); // pointer to the formatted data
	auto size = out.size(); // size of the formatted data
	//HAL_Delay(1);
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1);
}

void Print_No_Safety_front()
{
	auto out = fmt::memory_buffer();
	format_to(std::back_inserter(out), "Missing safety front: ");

	Safety_Front sffront;

	uint8_t mask = 0x01;

	for(int i = 0; i<8; i++)
	{
		if(ms66.Safety_front & mask == 0)
		{
			sffront = Safety_Front(i);
			format_to(std::back_inserter(out), "{}" , char(sffront));
		}
		else
		{

		}
	mask = mask << 1;
	}

	format_to(std::back_inserter(out), "\n\r");

	auto data = out.data(); // pointer to the formatted data
	auto size = out.size(); // size of the formatted data

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1);
}

void Print_No_Safety_rear()
{

	auto out = fmt::memory_buffer();
	format_to(std::back_inserter(out), "Missing safety rear: ");

	Safety_Rear sfrear;

	uint8_t mask = 0x01;

	for(int i = 0; i<8; i++)
	{
		if(ms70.Safety_rear & mask == 0)
		{
			sfrear = Safety_Rear(i);
			format_to(std::back_inserter(out), "{}" , char(sfrear));
		}
		else
		{

		}
	mask = mask << 1;
	}

	format_to(std::back_inserter(out), "\n\r");

	auto data = out.data(); // pointer to the formatted data
	auto size = out.size(); // size of the formatted data

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1);

}
void Print_Data2()//With twitching
{
	uint8_t clear[] = "\033[2J";

	auto out1 = fmt::memory_buffer();

	format_to(std::back_inserter(out1), "---------------------------------------------------------\n\r");
	format_to(std::back_inserter(out1), "Pedal Position: {} difference: {}\n\r", ms65.Pedal_Position, ms65.Pedal_diff);
	format_to(std::back_inserter(out1), "HV Voltage: {} HV SoC: {} HV Temp max: {} HV Temps: {} HV Current: {}\n\r", ms65.HV_Voltage, ms65.HV_SoC, ms65.HV_Temp_max, ms65.HV_Temps, ms65.HV_Current);
	format_to(std::back_inserter(out1), "Brake pressure front: {} Brake pressure rear: {}\n\r", ms66.brake_pressure_front, ms66.brake_pressure_rear);
	format_to(std::back_inserter(out1), "Vehicle speed: {} Motor current: {} Engine speed: {} Wheel_lf: {} Wheel_rf: {} Wheel_lr: {} Wheel_rr: {}\n\r", ms67.vehicle_speed, ms67.engine_speed, ms67.motor_current, ms67.wheel_left_front, ms67.wheel_right_front, ms67.wheel_left_rear, ms67.wheel_right_rear);

	auto data = out1.data(); // pointer to the formatted data
	auto size = out1.size(); // size of the formatted data
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1);

	HAL_Delay(100);

	HAL_UART_Transmit(&hlpuart1, clear, sizeof(clear), 1);
}

