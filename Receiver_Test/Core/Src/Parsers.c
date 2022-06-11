/*
 * Parsers.c
 *
 *  Created on: 11 May 2022
 *      Author: wasyl
 */

#include <Parsers.h>

extern UART_HandleTypeDef hlpuart1;

uint8_t UART_Buffer[10];
uint8_t next_line2[] = "\n\r";
uint8_t space2[] = "/";

void Message_69(uint8_t RxData[])
{
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
}
void Message_66(uint8_t RxData[])
{
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

	uint8_t brake_pressure_front = RxData[5];
	itoa(brake_pressure_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t brake_pressure_back = RxData[6];
	itoa(brake_pressure_back, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_x = RxData[7] << 8 | RxData[8];
	itoa(speed_x, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_y = RxData[9] << 8 | RxData[10];
	itoa(speed_y, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t speed_z = RxData[11] << 8 | RxData[12];
	itoa(speed_z, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_x = RxData[13] << 8 | RxData[14];
	itoa(acc_x, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_y = RxData[15] << 8 | RxData[16];
	itoa(acc_y, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t acc_z = RxData[17] << 8 | RxData[18];
	itoa(acc_z, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
}
void Message_67(uint8_t RxData[])
{
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
}
void Message_65(uint8_t RxData[])

{

	//uint8_t Message_ID = RxData[0];
	uint8_t Message_ID = 65;
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

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

	uint8_t HV_Soc = RxData[11];
	itoa(HV_Soc, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temp_Max = RxData[12];
	itoa(HV_Temp_Max, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temps = RxData[13];
	itoa(HV_Temps, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t HV_Current = RxData[14] << 8 | RxData[15];
	itoa(HV_Current, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SW_Angle = RxData[16] << 8 | RxData[17];
	itoa(SW_Angle, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t SF_Combined_States = RxData[18];
	itoa(SF_Combined_States, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SF_Sum_Of_Currents = RxData[19] << 8 | RxData[20];
	itoa(SF_Sum_Of_Currents, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	//uint8_t Flag_Buffer = RxData[21];
	uint8_t Flag_Buffer = 255;
	itoa(Flag_Buffer, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
}
void Message_68(uint8_t RxData[])
{
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
}













