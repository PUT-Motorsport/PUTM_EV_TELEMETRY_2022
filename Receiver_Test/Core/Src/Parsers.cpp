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

void Message_65(uint8_t RxData[])
{
	//Message ID
	ms65.msgid = RxData[0];
	//APPS Data:
	ms65.Pedal_Position = RxData[1] << 8 | RxData[2];
	ms65.Pedal_diff = (int)(RxData[3]);
	//LV Battery Data:
	ms65.LV_Voltage = RxData[4] << 8 | RxData[5];
	ms65.LV_Soc = RxData[6];
	ms65.LV_Temps = RxData[7];
	ms65.LV_Current = RxData[8];
	//HV Battery Data:
	ms65.HV_Voltage = RxData[9] << 8 | RxData[10];
	ms65.HV_Current = RxData[11] << 8 | RxData[12];
	ms65.HV_SoC = RxData[13] << 8 | RxData[14];
	ms65.HV_Temp_max = RxData[15];
	ms65.HV_Temps = RxData[16];
	//FIXME use fmt.
	/*
	auto display65 = fmt::memory_buffer();

	uint8_t Message_ID = RxData[0];
	format_to(std::back_inserter(display65),"{}/{}/{}/{}/{}/{}/{}/{}/{}/{}/{}/{}",
			ms65.msgid,ms65.Pedal_Position,ms65.Pedal_diff,ms65.LV_Voltage,ms65.LV_Soc,ms65.LV_Temps,ms65.LV_Current,ms65.HV_Voltage,
			ms65.HV_SoC,ms65.HV_Temp_max,ms65.HV_Temps,ms65.HV_Current);

	auto data = out1.data(); // pointer to the formatted data
	auto size = out1.size(); // size of the formatted data
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1000);

/*
	uint16_t APPS_Position = RxData[1] << 8 | RxData[2];
	//uint16_t APPS_Position = rand()% 500;
	itoa(APPS_Position, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t APPS_Diff = 150;
	itoa(APPS_Diff, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t LV_Voltage = RxData[4] << 8 | RxData[5];
	itoa(LV_Voltage, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t LV_Soc = RxData[6];
	itoa(LV_Soc, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t LV_Temps = RxData[7];
	itoa(LV_Temps, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t LV_Current = RxData[8];
	itoa(LV_Current, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t HV_Voltage_Sum = RxData[9] << 8 | RxData[10];
	itoa(HV_Voltage_Sum, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Soc = RxData[11] << 8 | RxData[12];
	itoa(HV_Soc, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temp_Max = RxData[13];
	itoa(HV_Temp_Max, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temps = RxData[14];
	itoa(HV_Temps, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t HV_Current = RxData[15] << 8 | RxData[16];
	itoa(HV_Current, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SW_Angle = RxData[17] << 8 | RxData[18];
	itoa(SW_Angle, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t SF_Combined_States = RxData[19];
	itoa(SF_Combined_States, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SF_Sum_Of_Currents = RxData[20] << 8 | RxData[21];
	itoa(SF_Sum_Of_Currents, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	//uint8_t Flag_Buffer = RxData[22];
	uint8_t Flag_Buffer = 255;
	itoa(Flag_Buffer, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
	*/
}
bool Message_66(uint8_t RxData[])
{
	ms66.susp_right_front = RxData[1] << 8 | RxData[2];
	ms66.susp_left_front = RxData[3] << 8 | RxData[4];
	ms66.brake_pressure_front = RxData[5] << 8 | RxData[6];
	ms66.brake_pressure_rear = RxData[7] << 8 | RxData[8];
	ms66.Acceleration = RxData[17] << 8 | RxData[18];
	ms66.Safety_front = RxData[21];

	uint8_t mask = 0x01;

	for(int i = 0; i<8; i++)
	{
		if(ms66.Safety_front & mask == 0)
		{
			return true;
		}
		else
		{
		    return false;
		}
		mask = mask << 1;
	}
	/*
	uint8_t Message_ID = RxData[0];
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t susp_right = RxData[1] << 8 | RxData[2];
	itoa(susp_right, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t susp_left = RxData[3] << 8 | RxData[4];
	itoa(susp_left, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t brake_pressure_front = RxData[5] << 8 | RxData[6];
	itoa(brake_pressure_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t brake_pressure_back = RxData[7]  << 8 | RxData[8];
	itoa(brake_pressure_back, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_x = RxData[9] << 8 | RxData[10];
	itoa(speed_x, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_y = RxData[11] << 8 | RxData[12];
	itoa(speed_y, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_z = RxData[13] << 8 | RxData[14];
	itoa(speed_z, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_x = RxData[15] << 8 | RxData[16];
	itoa(acc_x, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_y = RxData[17] << 8 | RxData[18];
	itoa(acc_y, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_z = RxData[19] << 8 | RxData[20];
	itoa(acc_z, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
	*/
}
void Message_67(uint8_t RxData[])
{
	ms67.vehicle_speed = RxData[1] << 8 | RxData[2];
	ms67.motor_current = RxData[3];
	ms67.engine_speed = RxData[4] << 8 | RxData[5];
	ms67.susp_left_rear = RxData[8] << 8 | RxData[9];
	ms67.susp_right_rear = RxData[10] | RxData[11];
	ms67.wheel_left_front = RxData[12] | RxData[13];
	ms67.wheel_right_front = RxData[14] | RxData[15];
	ms67.wheel_left_rear = RxData[16] | RxData[17];
	ms67.wheel_right_rear = RxData[18] | RxData[19];

	//FIXME use fmt.

	/*
	uint8_t Message_ID = RxData[0];
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t vehicle_speed = RxData[1] << 8 | RxData[2];
	itoa(vehicle_speed, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t motor_current = RxData[3];
	itoa(motor_current, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t engine_speed = RxData[4] << 8 | RxData[5];
	itoa(engine_speed, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t traction_control_enable = RxData[6];
	itoa(traction_control_enable, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t traction_control_intensivity = RxData[6];
	itoa(traction_control_intensivity, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t susp_left = RxData[8] << 8 | RxData[9];
	itoa(susp_left, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t susp_right = RxData[10] << 8 | RxData[11];
	itoa(susp_right, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t wheel_left_front = RxData[12] << 8 | RxData[13];
	itoa(wheel_left_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t wheel_right_front = RxData[14] << 8 | RxData[15];
	itoa(wheel_right_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t wheel_left_rear = RxData[16] << 8 | RxData[17];
	itoa(wheel_left_rear, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t wheel_right_rear = RxData[18] << 8 | RxData[19];
	itoa(wheel_right_rear, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
	*/
}

void Message_68(uint8_t RxData[])
{

	ms68.engine_temp = RxData[1];
	ms68.inverter_temp = RxData[2];

	//FIXME use fmt.

	/*
	uint8_t Message_ID = RxData[0];
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t engine_temp = RxData[1];
	itoa(engine_temp, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t inverter_temp = RxData[2];
	itoa(inverter_temp, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t water_p_in = RxData[3];
	itoa(water_p_in, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t water_p_out = RxData[4];
	itoa(water_p_out, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t water_tmp_in = RxData[5];
	itoa(water_tmp_in, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t water_tmp_out = RxData[6];
	itoa(water_tmp_out, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
	*/
}
void Message_69(uint8_t RxData[])
{

	/*
	uint8_t Message_ID = RxData[0];
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);

	for(int i = 1; i <= 7; i++)
	{
		HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);
		itoa(RxData[i], UART_Buffer, 10);
		HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	}
	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
	*/
}
bool Message_70(uint8_t RxData[])
{
	//check safety.
	ms70.Safety_rear = RxData[1];
	uint8_t mask = 0x01;

	for(int i = 0; i<8; i++)
	{
		if(ms70.Safety_rear & mask == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	mask = mask << 1;
	}
}
void Update_Terminal_No_Safety_front()
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

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1000);
}

void Update_Terminal_No_Safety_rear()
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

	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1000);

}

void Update_Terminal()//With twitching
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
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)(data), size, 1000);

	HAL_Delay(100);

	HAL_UART_Transmit(&hlpuart1, clear, sizeof(clear), 1000);
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
