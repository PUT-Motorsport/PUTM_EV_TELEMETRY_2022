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
	for(int i = 1; i < 32; i++)
	{
		itoa(RxData[i], UART_Buffer, 10);
		HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
		HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);
	}
	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
}
void Message_68(uint8_t RxData[])
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
void Message_65(uint8_t RxData[])
{

	uint8_t Message_ID = RxData[0];
	itoa(Message_ID, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t APPS_Position = RxData[1] << 8 | RxData[2];
	itoa(Message_ID, UART_Buffer, 10);
	itoa(APPS_Position, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t APPS_Diff = RxData[3];
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

	uint8_t AQ_Susp_Right_front = RxData[9];
	itoa(AQ_Susp_Right_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t AQ_Susp_Left_front = RxData[10];
	itoa(AQ_Susp_Left_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t AQ_Break_pressure_front = RxData[11];
	itoa(AQ_Break_pressure_front, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t AQ_Break_pressure_rear = RxData[12];
	itoa(AQ_Break_pressure_rear, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t AQ_Air_Flow_Velocity = RxData[13];
	itoa(AQ_Air_Flow_Velocity, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t HV_Voltage_Sum = RxData[14] << 8 | RxData[15];
	itoa(HV_Voltage_Sum, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Soc = RxData[16];
	itoa(HV_Soc, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temp_Max = RxData[17];
	itoa(HV_Temp_Max, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Temps = RxData[18];
	itoa(HV_Temps, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t HV_Current = RxData[19];
	itoa(HV_Current, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t SF_Combined_States = RxData[20];
	itoa(SF_Combined_States, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 3, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SF_Sum_Of_Currents = RxData[21] << 8 | RxData[22];
	itoa(SF_Sum_Of_Currents, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint16_t SW_angle = RxData[23] << 8 | RxData[24];
	itoa(SW_angle, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 4, 1000);
	HAL_UART_Transmit(&hlpuart1, space2, sizeof(space2), 1000);

	uint8_t Flag_Buffer = RxData[27];
	itoa(Flag_Buffer, UART_Buffer, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer, 2, 1000);
	HAL_UART_Transmit(&hlpuart1, next_line2, sizeof(next_line2), 1000);
}
