/*
 * CAN.c
 *
 *  Created on: Feb 3, 2022
 *      Author: Adam Wasilewski
 */

#include "CAN.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_FilterTypeDef flitr1;

/**
* @brief Can Initialization
* This function starts CAN protocol and activates interrupt from RX FIFO
* @param: none
* @retval none
*/
void CAN_Init()
{
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}
/**
* @brief Close filter
* This function Restrict any frames to be received except toggle frame.
* @param: none
* @retval none
*/
void Close_Filter()
{



}
/**
* @brief Open filter
* This function Allows every frame to be received.
* @param: none
* @retval none
*/

void Open_Filter()
{
	flitr1.FilterIdHigh = 0x0000;
	flitr1.FilterIdLow = 0x0000;
	flitr1.FilterMaskIdHigh = 0x0000;
	flitr1.FilterMaskIdLow = 0x0000;
	flitr1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	flitr1.FilterBank = 0;
	flitr1.FilterMode = CAN_FILTERMODE_IDMASK;
	flitr1.FilterActivation = CAN_FILTER_ENABLE;
	flitr1.FilterScale = CAN_FILTERSCALE_32BIT;
	flitr1.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(&hcan1, &flitr1);
}
