/*
 * Frame_by_frame_parser.h
 *
 *  Created on: 22 May 2022
 *      Author: Adam Wasilewski
 */

#include <Parsers.h>
#include "ID's.h"

extern UART_HandleTypeDef hlpuart1;

uint8_t UART_Buffer1[10];
const uint8_t next_line1[] = "\n\r";
const uint8_t space1[] = "/";

void Parse(uint8_t *RxData)
{
	uint16_t ID = RxData[0] << 8 | RxData[1];

	switch(ID)
	{
	case Apps_main:
		itoa(ID, UART_Buffer1, 10);
		HAL_UART_Transmit(&hlpuart1, UART_Buffer1, 2, 1000);
		HAL_UART_Transmit(&hlpuart1, space1, sizeof(space1), 1000);
		uint16_t APPS_Position = RxData[2] << 8 | RxData[3];
		itoa(APPS_Position, UART_Buffer1, 10);
		HAL_UART_Transmit(&hlpuart1, UART_Buffer1, 4, 1000);
		HAL_UART_Transmit(&hlpuart1, next_line1, sizeof(next_line1), 1000);
		break;
	case BMS_LV_main:

		break;
	}

}


