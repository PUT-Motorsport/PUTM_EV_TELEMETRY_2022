#pragma once

#include "Can_Send_Receive.hpp"
#include "Frame_IDs.hpp"
#include "etl/vector.h"

#define DATA_FULL 1024
#define num_of_frames 23

using namespace etl;

typedef vector<class pointer, num_of_frames> Parsing_Vector; // define parsing vector
/* Define all parsing functions */

void APPS_Main_Parser(Can_Message msg1);
void BMS_LV_Main_Parser(Can_Message msg1);
void BMS_LV_Temps_Parser(Can_Message msg1);
void Dash_Tcs_Frame_Parser(Can_Message msg1);
void TC_Main_Parser(Can_Message msg1);
void TC_Rear_Suspension(Can_Message msg1);
void AQ_Card_Main_Parser(Can_Message msg1);
void BMS_HV_Main_Parser(Can_Message msg1);

/*
 * Data mangement class:
 * - Manages parsing data and states from CAN frames.
 * - Prepares data for radio transmission.
 * - Keeps track of arriving frames.
 */

class Data_management
{
private:
	// Buffers containing data. Used to pass data to radio.
	uint8_t DataBuffer1[32] = {0};
	uint8_t DataBuffer2[32] = {0};
	uint8_t DataBuffer3[32] = {0};
	// Buffers containing states. Used to pass states to radio.
	uint8_t StateBuffer1[32] = {0};
	uint8_t StateBuffer2[32] = {0};
	uint8_t StateBuffer3[32] = {0};

public:
	// flags used to track arriving frames. Each bit represent corresponding
	// frame.
	uint16_t DataBuffer1_flag = 0;
	uint16_t DataBuffer2_flag = 0;
	uint16_t DataBuffer3_flag = 0;
	/* Methods */
	uint8_t* Prepare_DataBuffer1();
	uint8_t* Prepare_DataBuffer2();
	uint8_t* Prepare_DataBuffer3();

	uint8_t* Prepare_StateBuffer1();
	uint8_t* Prepare_StateBuffer2();
	uint8_t* Prepare_StateBuffer3();


	void Init();
	void Choose_Parser(Can_Message msg1);

	void Update_DataBuffer1_flag(uint8_t update)
	{
		DataBuffer1_flag = DataBuffer1_flag | update;
	}
	void Update_DataBuffer2_flag(uint8_t update)
	{
		DataBuffer1_flag = DataBuffer1_flag | update;
	}
	void Update_DataBuffer3_flag(uint8_t update)
	{
		DataBuffer1_flag = DataBuffer1_flag | update;
	}

	uint8_t* Return_Data_Buffer1()
	{
		return DataBuffer1;
	}
	uint8_t* Return_Data_Buffer2()
	{
		return DataBuffer2;
	}
	uint8_t* Return_Data_Buffer3()
	{
		return DataBuffer3;
	}

	void Clear_msg1();
	void Clear_msg2();
	void Clear_msg3();
};

/*
 * Pointer class:
 * - Object used in parsing process.
 * - Each device have its own pointer object, wich have two attributes:
 * 	 - ID  - ID of corresponding device
 * 	 - PTR - Pointer to parsing function, uniqe for each device.
 * - These objects are stored in vector called parsing vector.
 * - Parsing vector is then used to help match incoming CAN frame with
 * corresponding parsing function.
 */

class pointer
{
public:
	void (*PTR)(Can_Message msg1);
	int ID;
};

/*Below are objects storing data from specific devices */

/*
 * APPS class:
 * - Used to store data and state of Apps device.
 */
class APPS : public Data_management
{
private:
	uint16_t Pedal_Position;
	uint8_t diff;
	uint8_t state;

public:
	// void(*Apps_ptr)(Can_Message msg1) = APPS_Parser;
	pointer apps_main;
	APPS()
	{
		Pedal_Position = 0;
		state = 0;
		diff = 0;
		apps_main.ID = Apps_main2;
		apps_main.PTR = APPS_Main_Parser;
	}
	uint16_t retrun_Pedal_Position()
	{
		return Pedal_Position;
	}
	uint8_t return_Diff()
	{
		return diff;
	}
	uint8_t return_state()
	{
		return state;
	}
	void Clear_APPS()
	{
		Pedal_Position = 0;
		state = 0;
		diff = 0;
	}
	friend void APPS_Main_Parser(Can_Message msg1);
};
/*
 * BMS_LV class:
 * - Used to store data and state of BMS LV device.
 */
class BMS_LV : public Data_management
{
private:
	uint16_t Voltage_Sum;
	uint8_t Soc;
	uint8_t Temps;
	uint8_t Current;
	uint8_t state;

public:
	pointer bms_lv_main;
	pointer bms_lv_temps;
	BMS_LV()
	{
		Voltage_Sum = 0;
		Soc = 0;
		Temps = 0;
		Current = 0;
		state = 0;

		bms_lv_main.ID = BMS_LV_main2;
		bms_lv_main.PTR = BMS_LV_Main_Parser;

		bms_lv_temps.ID = BMS_LV_temperatures2;
		bms_lv_temps.PTR = BMS_LV_Temps_Parser;
	}
	uint16_t return_Voltage_Sum()
	{
		return Voltage_Sum;
	}
	uint8_t return_Soc()
	{
		return Soc;
	}
	uint8_t return_Temps()
	{
		return Temps;
	}
	uint8_t return_Current()
	{
		return Current;
	}
	uint8_t return_state()
	{
		return state;
	}
	friend void BMS_LV_Main_Parser(Can_Message msg1);
	friend void BMS_LV_Temps_Parser(Can_Message msg1);
};
/*
 * Traction Control class:
 * - Used to store data and state of Traction Control device.
 *
 */

class Traction_Control : public Data_management
{
private:
	uint16_t Wehicle_Speed;
	uint8_t Water_Temp;
	uint8_t Water_pressure;
	uint8_t Motor_Current;
	uint8_t TS_Bools;
	uint8_t TC_Intensity;
	uint16_t Adc_Susp_Right_r;
	uint16_t Adc_Susp_Left_r;
	uint16_t Acc_Lateral;
	uint16_t Acc_Longitunal;

public:
	pointer dash_tcs_frame;
	pointer tc_main;
	pointer tc_rear_suspension;
	Traction_Control()
	{
		Wehicle_Speed = 0;
		Water_Temp = 0;
		Water_pressure = 0;
		Motor_Current = 0;
		TS_Bools = 0;
		TC_Intensity = 0;
		Adc_Susp_Right_r = 0;
		Adc_Susp_Left_r = 0;
		Acc_Lateral = 0;
		Acc_Longitunal = 0;

		dash_tcs_frame.ID = Dash_tcs_frame2;
		dash_tcs_frame.PTR = Dash_Tcs_Frame_Parser;

		tc_main.ID = TC_main2;
		tc_main.PTR = TC_Main_Parser;

		tc_rear_suspension.ID = TC_rear_suspension2;
		tc_rear_suspension.PTR = TC_Rear_Suspension;
	}
	uint16_t return_Wehicle_Speed()
	{
		return Wehicle_Speed;
	}
	uint8_t return_Water_Temp()
	{
		return Water_Temp;
	}
	uint8_t return_Water_pressure()
	{
		return Water_pressure;
	}
	uint8_t return_Motor_Current()
	{
		return Motor_Current;
	}
	uint8_t return_TS_Bools()
	{
		return TS_Bools;
	}
	uint8_t return_TC_Intensity()
	{
		return TC_Intensity;
	}
	uint16_t return_Adc_Susp_Right_r()
	{
		return Adc_Susp_Right_r;
	}
	uint16_t return_Adc_Susp_Left_r()
	{
		return Adc_Susp_Left_r;
	}
	uint16_t return_Acc_Lateral()
	{
		return Acc_Lateral;
	}
	uint16_t return_Acc_Longitunal()
	{
		return Acc_Longitunal;
	}

	friend void Dash_Tcs_Frame_Parser(Can_Message msg1);
	friend void TC_Main_Parser(Can_Message msg1);
	friend void TC_Rear_Suspension(Can_Message msg1);
};
/*
 * AQ Card class:
 * - Used to store data and state of Data acqusition card device.
 */
class AQ_Card : public Data_management
{
private:
	uint16_t Adc_Susp_Right_f;
	uint16_t Adc_Susp_Left_f;
	uint8_t Break_Pressure_f;
	uint8_t Break_Pressure_r;
	uint16_t Air_Flow_Vel;

public:
	pointer aq_card_main;
	AQ_Card()
	{
		Adc_Susp_Right_f = 0;
		Adc_Susp_Left_f = 0;
		Break_Pressure_f = 0;
		Break_Pressure_r = 0;
		Air_Flow_Vel = 0;

		aq_card_main.ID = Aq_card_main2;
		aq_card_main.PTR = AQ_Card_Main_Parser;
	}
	uint16_t return_Adc_Susp_Right_f()
	{
		return Adc_Susp_Right_f;
	}
	uint16_t return_Adc_Susp_Left_f()
	{
		return Adc_Susp_Left_f;
	}
	uint8_t return_Break_Pressure_f()
	{
		return Break_Pressure_f;
	}
	uint8_t return_Break_Pressure_r()
	{
		return Break_Pressure_r;
	}
	uint16_t return_Air_Flow_Vel()
	{
		return Air_Flow_Vel;
	}

	friend void AQ_Card_Main_Parser(Can_Message msg1);
};
/*
 * BMS HV class:
 * - Used to store data and state of BMS HV device.
 */
class BMS_HV : Data_management
{
private:
	uint16_t Voltage_Sum;
	uint8_t Soc;
	uint8_t Temp_Max;
	uint8_t Temps;
	uint8_t Current;

public:
	pointer bms_hv_main;
	BMS_HV()
	{
		Voltage_Sum = 0;
		Soc = 0;
		Temp_Max = 0;
		Temps = 0;
		Current = 0;

		bms_hv_main.ID = BMS_HV_main2;
		bms_hv_main.PTR = BMS_HV_Main_Parser;
	}
	uint16_t return_Voltage_Sum()
	{
		return Voltage_Sum;
	}
	uint8_t return_Soc()
	{
		return Soc;
	}
	uint8_t return_Temp_Max()
	{
		return Temp_Max;
	}
	uint8_t return_Temps()
	{
		return Temps;
	}
	uint8_t return_Current()
	{
		return Current;
	}

	friend void BMS_HV_Main_Parser(Can_Message msg1);
};
class SF : Data_management
{
private:
	uint8_t combined_states;
	uint16_t sum_of_currents;

	uint8_t inv_state;
	uint16_t inv_current;

	uint8_t box_state;
	uint16_t box_current;

	uint8_t apps_state;
	uint16_t apps_current;

	uint8_t fan_l_state;
	uint16_t fan_l_current;

	uint8_t fan_r_state;
	uint16_t fan_r_current;

	uint8_t pump_state;
	uint16_t pump_current;

	uint8_t safety_state;
	uint16_t safety_current;

	uint8_t box_dv_state;
	uint16_t box_dv_current;

	uint8_t tsal_state;
	uint16_t tsal_current;

	uint8_t steering_state;
	uint16_t steering_current;

	uint8_t dash_state;
	uint16_t dash_current;

	uint8_t ws_rl_state;
	uint16_t ws_rl_current;

	uint8_t ws_fl_state;
	uint16_t ws_fl_current;

	uint8_t ws_rr_state;
	uint16_t ws_rr_current;

	uint8_t ws_fr_state;
	uint16_t ws_fr_current;

	uint8_t jetson_state;
	uint16_t jetson_current;

	uint8_t nuc_state;
	uint16_t nuc_current;
};

class Wheel_Temps : Data_management
{
private:
	uint8_t Wheel_Temp;
	uint8_t state;
};
class Steering_Wheel : Data_management
{
private:
	uint16_t angle;
	uint8_t state;
};
