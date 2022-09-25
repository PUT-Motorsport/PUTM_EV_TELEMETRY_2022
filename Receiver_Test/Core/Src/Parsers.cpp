/*
 * Parsers.c
 *
 *  Created on: 11 May 2022
 *      Author: Adam Wasilewski
 */
#include <Parsers.hpp>
#include <cstdlib>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format-inl.h>

extern UART_HandleTypeDef hlpuart1;

uint8_t UART_Buffer[10];
uint8_t next_line2[] = "\n\r";
uint8_t space2[] = "/";

static msg65 ms65;
static msg66 ms66;
static msg67 ms67;
static msg68 ms68;
static msg70 ms70;


uint8_t buffer[35]{0};


void Message_65(uint8_t RxData[])
{
	memcpy(buffer, RxData, 32);

	buffer[33] = '\r';
	buffer[34] = '\n';

	HAL_UART_Transmit(&hlpuart1, buffer , 34, 10);
}
bool Message_66(uint8_t RxData[])
{
	memcpy(buffer, RxData, 32);

	buffer[33] = '\r';
	buffer[34] = '\n';

	HAL_UART_Transmit(&hlpuart1, buffer , 34, 10);

}
void Message_67(uint8_t RxData[])
{

}

void Message_68(uint8_t RxData[])
{


}
void Message_69(uint8_t RxData[])
{


}
bool Message_70(uint8_t RxData[])
{

}
void Update_Terminal_No_Safety_front()
{

}

void Update_Terminal_No_Safety_rear()
{



}

void Update_Terminal()//With twitching
{

}

void Update_Terminal2()
{
	auto out = fmt::memory_buffer();
	format_to(std::back_inserter(out),"APPS:{} LVV:{} LV SoC{} HVV:{} HV SoC:{} HV T:{} HV Curr:{} BP Front:{} Speed:{} Motor curr:{} Engine V:{} Inverter T:{}\n\r",
			ms65.Pedal_Position,ms65.LV_Voltage,ms65.LV_Soc,ms65.HV_Voltage, ms65.HV_SoC, ms65.HV_Temps, ms65.HV_Current,ms66.brake_pressure_front,ms67.vehicle_speed,ms67.motor_current, ms67.engine_speed, ms68.inverter_temp);

	auto data = out.data(); // pointer to the formatted data
	auto size = out.size(); // size of the formatted data

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1000);

	HAL_Delay(10);
}
