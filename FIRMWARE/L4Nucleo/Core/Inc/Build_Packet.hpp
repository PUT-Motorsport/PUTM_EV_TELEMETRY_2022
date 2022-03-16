/*
 * Build_Packet.hpp
 *
 *  Created on: 8 lut 2022
 *      Author: wasyl
 */

#ifndef INC_BUILD_PACKET_HPP_
#define INC_BUILD_PACKET_HPP_

extern class Packet_1 *pck1;
extern class States *st1;

#include <Can_Send_Receive.hpp>
#include <Frame_IDs.hpp>
#include <cstring>

void Create_parsing_array();

class Packet_1{

private:
	uint16_t APPS;
	uint16_t Motor_RPM;
	uint16_t RMS_Current;
	uint16_t Status;
	uint8_t Temperature_1;
	uint8_t Temperature_2;

	uint8_t DataBuffer1[32] = { 0 };
	uint8_t DataBuffer2[32] = { 0 };
	uint8_t DataBuffer3[32] = { 0 };

	uint16_t flag_buffer;
public:
	Packet_1()
	{
		APPS = 0;
		Motor_RPM = 0;
		RMS_Current = 0;
		Status = 0;
		Temperature_1 = 0;
		Temperature_2 = 0;
		DataBuffer1[32] = { 0 };
		DataBuffer2[32] = { 0 };
		DataBuffer3[32] = { 0 };
		flag_buffer = 0;
	}
	~Packet_1();
///////////////////Methods///////////////////////
	uint8_t Assign_Data(Can_Message *msg1);
	uint8_t* Prepare_Data(uint16_t flag_buffer);
	bool Choose_Parser(Can_Message *msg1, Packet_1 *pck1, States *st1);
	void Clear_Packet();

	friend void APPS_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);
	friend void BMS_LV_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);

	uint16_t Return_flag_buffer()
	{
		return flag_buffer;
	}
};

class States{

private:
	uint8_t APPS_State;
	uint8_t BMS_LV_State;
	/*
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 */
public:
	uint8_t Data_Buffer1[32] = { 0 };
	States()
	{

	}
	uint8_t * Build_State_Message();

	friend void APPS_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);
	friend void BMS_LV_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);
};

class pointer_to_parser{
public:
	void (*ptr)(Packet_1*, Can_Message*, States*);
	int ID;
	pointer_to_parser(void (PTR)(Packet_1*, Can_Message*, States*), int id)
	{
		ptr = PTR;
		ID = id;
	}
};


#endif /* INC_BUILD_PACKET_HPP_ */
