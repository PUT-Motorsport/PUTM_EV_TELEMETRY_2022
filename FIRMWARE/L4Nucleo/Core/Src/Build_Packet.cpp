/*
 * Build_Packet.cpp
 *
 *  Created on: 8 lut 2022
 *      Author: Adam Wasilewski
 */

#include <Build_Packet.hpp>
#include <Frame_IDs.hpp>
#include <Device_State.hpp>

//Declarations of all available data parsers

void APPS_Parser  (Packet_1 *pck1, Can_Message *msg1, States *states1);
void BMS_LV_Parser(Packet_1 *pck1, Can_Message *msg1, States *states1);

pointer_to_parser* parsing_array[10];//array stores pointers to specific parsing functions.

/**
* @brief Assign Data
* This is a basic function to assign received via CAN data and put it in Packet.
* @param: Pointer to the received CAN message.
* @retval flag_buffer variable wich carries information about Packet fullfilment.
*/
uint8_t Packet_1::Assign_Data(Can_Message *msg1)
{
	if(msg1->return_ID() == 0x0A)// APPS[2]
		{
			flag_buffer   = flag_buffer | 0x01;
			APPS          = msg1->return_Data(0) << 8 | msg1->return_Data(1);
		}
	else if(msg1->return_ID() == 0x0B)//Prędkość obrotowa silnika[2] / Prąd RMS[2] / Status[2] / Temperatura1[1] / Temperatura2[1]
		{
			flag_buffer   = flag_buffer | 0x02;
			Motor_RPM     = msg1->return_Data(0) << 8 | msg1->return_Data(1);
			RMS_Current   = msg1->return_Data(2) << 8 | msg1->return_Data(3);
			Status        = msg1->return_Data(4) << 8 | msg1->return_Data(5);
			Temperature_1 = msg1->return_Data(6);
		}
	else if(msg1->return_ID() == 0x0C)//Kod błędu[1] / State od charge[1] / napięcie 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x04;

		}
	else if(msg1->return_ID() == 0x0D)//Ready to Drive[1] / Prędkość koła lewego[2] / prędkość koła prawego[2] / ciśnienie hamulców1[2] / tem. mono[1]
		{
			flag_buffer = flag_buffer | 0x08;

		}
	else if(msg1->return_ID() == 0x0E)//HV prąd[2] / HV napięcie[2] / Temp max[1] / Poziom naładowania[1] / Status[1] / Kody błędów[1]
		{
			flag_buffer = flag_buffer | 0x10;

		}
	else if(msg1->return_ID() == 0x60)//Prędkość obrotwa koła prawego[2] / lewego[2] / Kąt ugięcia dampera prawego tył[1] / lewego[1] / temp. wody za pompą[1] / za chłodnicą[1]
		{
			flag_buffer = flag_buffer | 0x20;

		}
	else if(msg1->return_ID() == 0x5F)// avg.temp[1] / tem 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x40;

		}
	else if(msg1->return_ID() == 0x5B)//GyroX[2] / GyroY[2] / GyroZ[2]
		{
			flag_buffer = flag_buffer | 0x80;

		}
	else if(msg1->return_ID() == 0x5A)// AccX[2] / AccY[2] / AccZ[2]
		{
			flag_buffer = flag_buffer | 0x100;

		}
	else if(msg1->return_ID() == 0x0F)//Kąt ugięcia dampera prawego przód[2] / lewego[2] / ciśnienie hamulców2[2]
		{
			flag_buffer = flag_buffer | 0x200;

		}
	return flag_buffer;
}
/**
* @brief Creat a parsing array
* This function creates pointer_to_parser objects, which contains ID and pointer to parsing function, and puts them into a array.
* @param: none
* @retval none
*/
void Create_parsing_array()
{
	 void (*APPS_ptr)  (Packet_1*, Can_Message*, States*) = APPS_Parser;
	 void (*BMS_LV_ptr)(Packet_1*, Can_Message*, States*) = BMS_LV_Parser;
	 	 	 	 	 	 	 	 /*
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  */
	 pointer_to_parser* APPS   = new pointer_to_parser(APPS_ptr, APPS_ID);
	 pointer_to_parser* BMS_LV = new pointer_to_parser(BMS_LV_ptr, BMS_LV_ID);
	 	 	 	 	 	 	 	 /*
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  */
	 parsing_array[0] = APPS;
	 parsing_array[1] = BMS_LV;
	 	 	 	 	 	 	 	 /*
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  *
	 	 	 	 	 	 	 	  */
}
/**
* @brief Parse data from can frame.
* This function uses a binary search, to match newly arrived can frame with corresponding parsing function.
* @param: Pointer to data packet
* @param: Pointer to can message.
* @retval true if matched, false if not.
*/
bool Packet_1::Choose_Parser(Can_Message *msg1, Packet_1 *pck1, States *st1)
{
	if(msg1->return_ID() == APPS_ID)
	{
		parsing_array[0]->ptr(pck1, msg1, st1);
	}
return true;
}
/**
* @brief Prepare Data
* This function assigns data into 32-bytes long buffer, that is being forwarded to the radio.
* @param: flag_buffer variable, wich is used to indicate missing frames in packet.
* @retval Pointer to DataBuffer1.
*/
uint8_t * Packet_1::Prepare_Data(uint16_t flag_buffer)
{
	//Buffer Indetificator.
	DataBuffer1[0] = 'a';
	//APPS
	DataBuffer1[1] = APPS>>8;
	DataBuffer1[2] = APPS;
	//Motor RPM
	DataBuffer1[3] = Motor_RPM>>8;
	DataBuffer1[4] = Motor_RPM;
	//RMS Current
	DataBuffer1[5] = RMS_Current>>8;
	DataBuffer1[6] = RMS_Current;

	DataBuffer1[30] = flag_buffer>>8;
	DataBuffer1[31] = flag_buffer;
	//Add flag_buffer to DataBuffer;
	return DataBuffer1;
}
/**
* @brief Clear data
* This function sets all fields in pck1 to 0.
* @param: none.
* @retval none.
*/
void Packet_1::Clear_Packet()
{
	memset(DataBuffer1, 0, sizeof(DataBuffer1));
}

uint8_t * States::Build_State_Message()
{
	Data_Buffer1[0] = 'a';
	Data_Buffer1[1] = 1;//APPS State etc...
	/*
	 *
	 *
	 *
	 *
	 *
	 *
	 */
	return Data_Buffer1;
}

void APPS_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1)
{
	//Update flag_buffer
	pck1->flag_buffer = pck1->flag_buffer | 0x01;
	//Parse Data
	pck1->APPS = msg1->return_Data(0) << 8 | msg1->return_Data(1);
	//Parse device state
	st1->APPS_State = msg1->return_Data(8);
}

void BMS_LV_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1)
{



}




