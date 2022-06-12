/*
 * Frame_by_frame_parser.h
 *
 *  Created on: 22 May 2022
 *      Author: Adam Wasilewski
 */

#include <Parsers.h>
#include "ID's.h"
//#include "lib/can_interface.hpp"

extern UART_HandleTypeDef hlpuart1;

uint8_t UART_Buffer1[10];
const uint8_t next_line1[] = "\n\r";
const uint8_t space1[] = "/";

void TransmitWithSpace(uint16_t data)
{
	itoa(data, UART_Buffer1, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer1, strlen(UART_Buffer1), 1000);
	HAL_UART_Transmit(&hlpuart1, space1, sizeof(space1), 1000);
}

void TransmitWithNewLine(uint16_t data)
{
	itoa(data, UART_Buffer1, 10);
	HAL_UART_Transmit(&hlpuart1, UART_Buffer1, strlen(UART_Buffer1), 1000);
	HAL_UART_Transmit(&hlpuart1, next_line1, sizeof(next_line1), 1000);
}

void Parse(uint8_t *RxData)
{
	uint16_t ID = RxData[0] << 8 | RxData[1];

	switch(ID)
	{
	/*
	case Apps_main:
		TransmitWithSpace(ID);

		uint16_t APPS_Position = RxData[2] << 8 | RxData[3];
		TransmitWithNewLine(APPS_Position);
		break;
		/*
	case BMS_LV_main:
		TransmitWithSpace(ID);

		uint16_t BMS_LV_VoltageSum = RxData[2] << 8 | RxData[3];
		TransmitWithSpace(BMS_LV_VoltageSum);

		uint16_t BMS_LV_Soc = RxData[4];
		TransmitWithSpace(BMS_LV_Soc);

		uint16_t BMS_LV_Temp_avg = RxData[5];
		TransmitWithSpace(BMS_LV_Temp_avg);

		uint16_t BMS_LV_Current = RxData[6];
		TransmitWithSpace(BMS_LV_Current);

		uint16_t BMS_LV_Device_state = RxData[7];
		TransmitWithNewLine(BMS_LV_Device_state);

		break;
	case Aq_card_main:
		TransmitWithSpace(ID);		
		
		uint16_t AQ_MAIN_adc_susp_right = RxData[2] << 8 | RxData[3];
		TransmitWithSpace(AQ_MAIN_adc_susp_right);

		uint16_t AQ_MAIN_adc_susp_left = RxData[4] << 8 | RxData[5];
		TransmitWithSpace(AQ_MAIN_adc_susp_left);

		uint16_t AQ_MAIN_brake_pressure_front = RxData[6];
		TransmitWithSpace(AQ_MAIN_brake_pressure_front);

		uint16_t AQ_MAIN_brake_pressure_back = RxData[7];
		TransmitWithNewLine(AQ_MAIN_brake_pressure_back);
		break;

	case BMS_HV_main:
		TransmitWithSpace(ID);

		uint16_t BMS_HV_voltage_sum = RxData[2] << 8 | RxData[3];
		TransmitWithSpace(BMS_HV_voltage_sum);

		uint16_t BMS_HV_soc = RxData[4];
		TransmitWithSpace(BMS_HV_soc);

		uint16_t BMS_HV_temp_max = RxData[5];
		TransmitWithSpace(BMS_HV_temp_max);

		uint16_t BMS_HV_temp_avg = RxData[6];
		TransmitWithSpace(BMS_HV_temp_avg);

		uint16_t BMS_HV_current = RxData[7];
		TransmitWithSpace(BMS_HV_current);

		uint16_t BMS_HV_device_state = RxData[8];
		TransmitWithNewLine(BMS_HV_device_state);
		break;
		/*
	case SF_main:
		TransmitWithSpace(ID);

		uint16_t SF_overall_state = RxData[2];
		TransmitWithSpace(SF_overall_state);

		uint16_t SF_current = RxData[3] << 8 | RxData[4];
		TransmitWithNewLine(SF_current);
		break;
	case TC_main:
		TransmitWithSpace(ID);

		uint16_t TC_vehicle_speed = RxData[2] << 8 | RxData[3];
		TransmitWithSpace(TC_vehicle_speed);

		uint16_t TC_motor_current = RxData[4];
		TransmitWithSpace(TC_motor_current);

		uint16_t TC_water_pressure = RxData[5];
		TransmitWithSpace(TC_water_pressure);

		uint16_t TC_water_temp = RxData[6];
		TransmitWithSpace(TC_water_temp);

		uint16_t TC_traction_control_intensivity = RxData[7];
		TransmitWithSpace(TC_traction_control_intensivity);

		uint16_t TC_brake_light_active = RxData[8];
		TransmitWithSpace(TC_brake_light_active);

		uint8_t TC_State = RxData[9];
		TransmitWithNewLine(TC_State);

		break;
		*/
	case TC_Temps:
		TransmitWithSpace(ID);
		for(int i = 2; i <= 7; i++)
		{
			TransmitWithSpace(RxData[i]);
		}
		TransmitWithNewLine(0);
		HAL_Delay(1000);
		break;
	}

}


