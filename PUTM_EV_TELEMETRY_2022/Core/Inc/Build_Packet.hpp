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
	//APPS
	uint16_t Pedal_Position;
	//BMS LV
	uint16_t LV_Voltage_Sum;
	uint8_t  LV_Soc;
	uint8_t  LV_Temps;
	uint8_t  LV_Current;
	//TC
	uint16_t Wehicle_Speed;
	uint8_t  Water_Temp;
	uint8_t  Water_pressure;
	uint8_t  Motor_Current;
	uint8_t  TS_Bools;
	uint8_t  TC_Intensity;
	uint16_t Adc_Susp_Right_r;
	uint16_t Adc_Susp_Left_r;
	uint16_t Acc_Lateral;
	uint16_t Acc_Longitunal;
	//SF
	uint16_t SF_State;
	//AQ Card
	uint16_t Adc_Susp_Right_f;
	uint16_t Adc_Susp_Left_f;
	uint8_t  Break_Pressure_f;
	uint8_t  Break_Pressure_r;
	uint16_t Air_Flow_Vel;
	//BMS_HV
	uint16_t HV_Voltage_Sum;
	uint8_t  HV_Soc;
	uint8_t  HV_Temp_Max;
	uint8_t  HV_Temps;
	uint8_t  HV_Current;

	uint8_t DataBuffer1[32] = { 0 };
	uint8_t DataBuffer2[32] = { 0 };
	uint8_t DataBuffer3[32] = { 0 };

	uint16_t flag_buffer;
public:
	Packet_1()
	{
		flag_buffer = 0;
	}
///////////////////Methods///////////////////////
	uint8_t Assign_Data(Can_Message *msg1);

	uint8_t* Prepare_Data1(uint16_t flag_buffer);
	uint8_t* Prepare_Data2(uint16_t flag_buffer);
	uint8_t* Prepare_Data3(uint16_t flag_buffer);

	bool Choose_Parser(Can_Message msg1, Packet_1 pck1, States st1);

	friend void APPS_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);
	friend void BMS_LV_Parser(Packet_1 *pck1, Can_Message *msg1, States *st1);





	void Clear_Packet();
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
	uint8_t dev_states[32] = { 0 };
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
