/*
 * CAN.c
 *
 *  Created on: Feb 3, 2022
 *      Author: Adam Wasilewski
 */

#include "CAN.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
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

}
