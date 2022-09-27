/*
 * Parsers.h
 *
 *  Created on: 11 May 2022
 *      Author: wasyl
 */
#include "main.h"


#ifndef INC_PARSERS_H_
#define INC_PARSERS_H_

void Message_65(uint8_t RxData[]);
void Message_69(uint8_t RxData[]);
bool Message_66(uint8_t RxData[]);
void Message_67(uint8_t RxData[]);
void Message_68(uint8_t RxData[]);
bool Message_70(uint8_t RxData[]);
void Lap_time_handler(uint8_t RxData);

struct msg65{
	uint8_t msgid;
	uint16_t Pedal_Position;
	int Pedal_diff;
	uint16_t LV_Voltage;
	uint8_t LV_Soc;
	uint8_t LV_Temps;
	uint8_t LV_Current;
	uint16_t HV_Voltage;
	uint8_t HV_SoC;
	uint8_t HV_Temp_max;
	uint8_t HV_Temps;
	uint8_t HV_Current;
};
struct msg66{
	uint16_t susp_right_front;
	uint16_t susp_left_front;
	uint16_t brake_pressure_front;
	uint16_t brake_pressure_rear;
	uint16_t Acceleration;
	uint8_t Safety_front;
};
struct msg67{
	uint16_t vehicle_speed;
	uint8_t motor_current;
	uint16_t engine_speed;
	uint16_t susp_right_rear;
	uint16_t susp_left_rear;
	uint16_t wheel_left_front;
	uint16_t wheel_right_front;
	uint16_t wheel_left_rear;
	uint16_t wheel_right_rear;
};
struct msg68{
	uint8_t engine_temp;
	uint8_t inverter_temp;
};
struct msg70{
	uint8_t Safety_rear;
};
enum struct Safety_Rear{
	Firewall,

};
enum struct Safety_Front{
	Driver_kill,
	Left_kill,
	Right_kill,
	Inertia,
	Overtravel,
	BSPD
};
void Pass(uint8_t *RxData);
void Update_Terminal2();

#endif /* INC_PARSERS_H_ */
