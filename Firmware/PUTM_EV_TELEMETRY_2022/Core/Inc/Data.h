#pragma once


enum struct HeartBeat
{
    Buffer1,
    Buffer2,
    Buffer3,
    DEFAULT,
    FRAME_BY_FRAME
};

void Send_Global_Time(PUTM_CAN::Telemetry_states state);

struct __attribute__((packed)) Buffer_100hz
{
	uint8_t  id;
	uint16_t Apps_Pedal_Position;
	uint8_t  Apps_Counter;
	uint8_t  Apps_Difference;
	uint8_t  Apps_State;
	uint16_t Bms_Lv_Voltage;
	uint8_t  Bms_Lv_SoC;
	uint8_t  Bms_Lv_Temp;
	uint8_t  Bms_Lv_Current;
	uint16_t Bms_Hv_Voltage;
	uint16_t Bms_Hv_Current;
	uint16_t Bms_HV_SoC;
	uint8_t  Bms_Hv_Temp_Max;
	uint8_t  Bms_Hv_Temp_Avg;
	uint16_t Steering_Wheel_Angle;
	uint8_t  Fuse_States;
	uint16_t Fuse_Current;

	uint16_t Aq_Suspension_Right_Front;
	uint16_t Aq_Suspension_Left_Front;
	uint16_t Aq_Brake_Pressure_Front;
	uint16_t Aq_Brake_Pressure_Rear;
	uint16_t Aq_Speed_X;
	uint16_t Aq_Speed_Y;
	uint16_t Aq_Speed_Z;
	uint16_t Aq_Acc_X;
	uint16_t Aq_Acc_Y;
	uint16_t Aq_Acc_Z;
	uint8_t  Aq_Safety_Front;

	uint8_t  Bms_Lv_State;
	uint8_t  Bms_Hv_State;
	uint8_t  Sw_State;
	uint8_t  Fuse_State;
	uint8_t  Aq_State;

}buffer100hz;

struct __attribute__((packed)) Buffer_50hz
{
	uint8_t  id;
	uint16_t Tc_Vehicle_Speed;
	uint8_t  Tc_Motor_Current;
	uint16_t Tc_Motor_Speed;
	uint8_t  Tc_Traction_Control_Enable;
	uint8_t  Tc_Tracion_Control_Intensity;
	uint16_t Tc_Suspension_Left_Rear;
	uint16_t Tc_Suspension_Right_Rear;
	uint16_t Tc_Wheel_Speed_Left_Front;
	uint16_t Tc_Wheel_Speed_Right_Front;
	uint16_t Tc_Wheel_Speed_Left_Rear;
	uint16_t Tc_Wheel_Speed_Right_Rear;
	uint16_t Tc_Acc_X;
	uint16_t Tc_Acc_Y;
	uint16_t Tc_Acc_Z;
	uint16_t Tc_Gyro_X;
	uint16_t Tc_Gyro_Y;
	uint16_t Tc_Gyro_Z;
	uint8_t  Tc_State;
}buffer50hz;

struct __attribute__((packed)) Buffer_10hz
{
	uint8_t id;
	uint8_t Tc_Temps_Motor;
	uint8_t Tc_Temps_Inverter;
	uint8_t Tc_Temps_Water_Pressure_In;
	uint8_t Tc_Temps_Water_Pressure_Out;
	uint8_t Tc_Temps_Water_In;
	uint8_t Tc_Temps_Water_Out;
}buffer10hz;

struct __attribute__((packed)) Buffer_1hz{

}buffer1hz;









