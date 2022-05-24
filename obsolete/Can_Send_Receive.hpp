/*
 * Can_Send_Receive.hpp
 *
 *  Created on: Feb 3, 2022
 *      Author: Adam Wasilewski
 */

#ifndef INC_CAN_SEND_RECEIVE_HPP_
#define INC_CAN_SEND_RECEIVE_HPP_

#include "CAN.h"

void Send_Message(uint32_t ID, uint32_t DLC, uint8_t *data);

class Can_Message
{
private:
	uint16_t ID;
	uint8_t DLC;
	uint8_t RxData[8];
	CAN_RxHeaderTypeDef RxHeader_HCAN1;
public:
//////////Returns/////////////
	uint16_t return_ID()
	{
		return ID;
	}
	uint8_t return_Data(int i)
	{
		return RxData[i];
	}
//////////Methods/////////////
	bool Build_Message();
	void Send_Message(uint32_t ID, uint32_t DLC, uint8_t *data);
};

#endif /* INC_CAN_SEND_RECEIVE_HPP_ */
