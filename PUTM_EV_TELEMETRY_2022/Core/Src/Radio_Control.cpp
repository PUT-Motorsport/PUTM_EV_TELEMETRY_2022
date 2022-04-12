/*
 * Radio_Control.cpp
 *
 *  Created on: 12 lut 2022
 *      Author: Adam Wasilewski
 */
#include <Radio_Control.hpp>
#include "NRF24L01.c"
#include "NRF24L01.h"

uint8_t TxAddress[5] = {0xEE,0xDD,0xCC,0xBB,0xAA};

/**
* @brief Radio initialization
* This function turns on the radio, initilize it and set up as transmiter
* @param: none
* @retval true if everything is ok, false if error occured.
*/
bool Setup_Radio_As_Transmiter()
{
	NRF24_Init();
	if(NRF24_TxMode(TxAddress, 10) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**
* @brief Send data
* This function Sends the data packet to the radio, wich trigger transmission.
* @param: Pointer to the data packet
* @retval true if everything is ok, false if error occured.
*/
bool Send_Data(uint8_t* pData)
{
	if(NRF24_Transmit(pData) == 1)
	{
		return true;
	}
	else
	{
		Clear_retrasmission_flag();
		return false;
	}
}
/**
* @brief Radio de-initialization
* This function turns off the radio
* @param: none
* @retval true if everything is ok, false if error occured.
*/
bool Turn_Radio_Off()
{



	return true;
}



