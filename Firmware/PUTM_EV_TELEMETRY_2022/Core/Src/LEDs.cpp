/*
 * LEDs.c
 *
 *  Created on: May 11, 2022
 *      Author: Adam Wasilewski
 */

#include "LEDs.hpp"

void Set_Error()
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_SET);
}
void Set_OK()
{
	HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
}
void Set_Radio_Warning()
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
}
void Clear_Radio_Warning()
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
}
void Set_CAN_Warning()
{
	HAL_GPIO_WritePin(GPIOC, LED_AUX_Pin, GPIO_PIN_RESET);
}
void Clear_CAN_Warning()
{
	HAL_GPIO_WritePin(GPIOC, LED_AUX_Pin, GPIO_PIN_SET);
}
void Clear_All()
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_ORANGE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_AUX_Pin, GPIO_PIN_SET);
}
void Set_All()
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_ORANGE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_AUX_Pin, GPIO_PIN_RESET);
}
void Start()
{
	Set_All();
	HAL_Delay(200);
	Clear_All();
}

