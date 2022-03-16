/*
 * Can_Send_Receive.cpp
 *
 *  Created on: Feb 3, 2022
 *      Author: Adam Wasilewski
 */

#include <Can_Send_Receive.hpp>
#include "CAN.h"
#include "CAN.c"
extern CAN_HandleTypeDef hcan1;
/**
* @brief Receive Message
* This function receives CAN message.
* @param: none
* @retval Bool. True if its data, false if its a toggle frame.
*/
bool Can_Message::Build_Message()
{
	CAN_RxHeaderTypeDef RxHeader_HCAN1;
	uint8_t RxData[8];

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader_HCAN1, RxData);
	ID = RxHeader_HCAN1.StdId;
	if(ID == 0x001)
	{
		return false;
	}
	DLC = RxHeader_HCAN1.DLC;
	Data = RxData;
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	return true;
}
/**
* @brief Send message
* This function puts message into a CAN bus.
* @param ID of message
*
* @param Data lenght
*
* @param Pointer to data buffer
*
* @retval none
*/
void Send_Message(uint32_t ID, uint32_t DLC, uint8_t *data)
{
	CAN_TxHeaderTypeDef TxHeader_HCAN1;

	TxHeader_HCAN1.DLC = DLC;
	TxHeader_HCAN1.StdId = ID;
	TxHeader_HCAN1.IDE = CAN_ID_STD;
	uint32_t mailbox1 = 0;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_HCAN1, data, &mailbox1);
}