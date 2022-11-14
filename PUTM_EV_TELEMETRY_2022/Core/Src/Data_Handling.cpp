#include "Data_Handling.hpp"
#include "Radio_Control.hpp"
#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"
#include "etl/circular_buffer.h"
#include <vector>

extern HeartBeat hb1;
extern Data_management handler1;

extern CAN_HandleTypeDef hcan1;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;


//Declare Circular buffer to store past states.
etl::circular_buffer<PUTM_CAN::Telemetry_states, 5> States_buffer;
//Prototype declaration
void Send_Global_Time(PUTM_CAN::Telemetry_states state);

void Data_management::Init()
{
	//Send_Global_Time(PUTM_CAN::Telemetry_states::Power_up);
}
void Adjust_Waiting_time(uint16_t time)
{
    __HAL_TIM_SET_AUTORELOAD(&htim6, time);
    HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    // Send state and time frame
    if (htim->Instance == TIM2)
    {
       Send_Global_Time(PUTM_CAN::Telemetry_states::Transmitting);
    }
    // HIGH_SPEED_DATA HeartBeat
    if (htim->Instance == TIM3)
    {
        hb1 = HeartBeat::Buffer1;
    }
    // TC HeartBeat
    if (htim->Instance == TIM4)
    {
        hb1 = HeartBeat::Buffer2;
    }
    // DataBuffer3 HeartBeat
    if (htim->Instance == TIM5)
    {
        hb1 = HeartBeat::Buffer3;
    }
    //handler1.Pass_States(uint8_t(PUTM_CAN::Telemetry_states::Gathering_Data));
}
uint8_t* Data_management::Check_Buffer100hz()
{

    auto apps = PUTM_CAN::can.get_apps_main();
    auto bmslv = PUTM_CAN::can.get_bms_lv_main();
    auto sf = PUTM_CAN::can.get_sf_main();
    auto bmshv = PUTM_CAN::can.get_bms_hv_main();

    auto *ptr = DataBuffer100hz;
    *ptr = 'A';
    ptr++;

    memcpy(ptr, &apps, sizeof(apps));
    ptr += sizeof(apps);
    memcpy(ptr, &bmslv, sizeof(bmslv));
    ptr += sizeof(bmslv);
    memcpy(ptr, &bmshv, sizeof(bmshv));
    ptr += sizeof(bmshv);
    memcpy(ptr, &sf, sizeof(sf));
    ptr += sizeof(sf);

    hb1 = HeartBeat::DEFAULT;
    return DataBuffer100hz;
}
uint8_t* Data_management::Check_Buffer_Aq()
{
    auto aq = PUTM_CAN::can.get_aq_main();
    auto aq_gyro = PUTM_CAN::can.get_aq_gyroscope();
    auto aq_acc = PUTM_CAN::can.get_aq_acceleration();

    auto *ptr = DataBufferAq;
    *ptr = 'B';
    ptr++;

	memcpy(ptr, &aq, sizeof(aq));
	ptr += sizeof(aq);
	memcpy(ptr, &aq_gyro, sizeof(aq_gyro));
	ptr += sizeof(aq_gyro);
	memcpy(ptr, &aq_acc, sizeof(aq_acc));

    hb1 = HeartBeat::DEFAULT;
    return DataBufferAq;

}

uint8_t* Data_management::Check_Buffer50hz()
{
    auto tc_main = PUTM_CAN::can.get_tc_main();
    auto tc_sus = PUTM_CAN::can.get_tc_rear();
    auto tc_wheels = PUTM_CAN::can.get_tc_wheel_velocities();
    auto tc_imu_acc = PUTM_CAN::can.get_tc_imu_acc();
    auto tc_imu_gyro = PUTM_CAN::can.get_tc_imu_gyro();

    auto *ptr = DataBuffer100hz;
    *ptr = 'C';
    ptr++;

    memcpy(ptr, &tc_main, sizeof(tc_main));
    ptr += sizeof(tc_main);
    memcpy(ptr, &tc_sus, sizeof(tc_sus));
    ptr += sizeof(tc_sus);
    memcpy(ptr, &tc_wheels, sizeof(tc_wheels));
    ptr += sizeof(tc_wheels);
    memcpy(ptr, &tc_imu_acc, sizeof(tc_imu_acc));
    ptr += sizeof(tc_imu_acc);
    memcpy(ptr, &tc_imu_gyro, sizeof(tc_imu_gyro));

    hb1 = HeartBeat::DEFAULT;
    return DataBuffer50hz;
}

uint8_t* Data_management::Check_Buffer10hz()
{

    auto tc_temps = PUTM_CAN::can.get_tc_temperatures();

    auto *ptr = DataBuffer10hz;
    *ptr = 'D';
    ptr++;

    memcpy(ptr, &tc_temps, sizeof(tc_temps));
    ptr += sizeof(tc_temps);


    return DataBuffer10hz;
}
uint8_t* Data_management::Check_Buffer1hz()
{
	//auto wheel_temp = PUTM_CAN::can.get_


    return DataBuffer1hz;
}
uint8_t* Data_management::Check_Buffer_Laptimer()
{
	using namespace PUTM_CAN;

	auto *ptr = DataBufferLaptimer;
	*ptr = 'E';
	ptr++;

	if(can.get_laptimer_sector_new_data() == true)
	{
		auto sector_time = can.get_laptimer_sector();

		memcpy(ptr, &sector_time, sizeof(sector_time));
		ptr += sizeof(sector_time);
	}
	if(can.get_laptimer_acc_new_data() == true)
	{
			auto acc_time = can.get_laptimer_acc_time();

			memcpy(ptr, &acc_time, sizeof(acc_time));
			ptr += sizeof(acc_time);
	}
	if(can.get_laptimer_skidpad_new_data() == true)
	{
			auto skidpad_time = can.get_laptimer_skidpad_time();

			memcpy(ptr, &skidpad_time, sizeof(skidpad_time));
			ptr += sizeof(skidpad_time);
	}
	if(can.get_laptimer_laptime_new_dat() == true)
	{
			auto lap_time = can.get_laptimer_laptime();

			memcpy(ptr, &lap_time, sizeof(lap_time));
			ptr += sizeof(lap_time);
	}
	return DataBufferLaptimer;
}

void Send_Global_Time(PUTM_CAN::Telemetry_states state)
{
	RTC_TimeTypeDef time1;
	RTC_DateTypeDef dt1;

	HAL_RTC_GetDate(&hrtc, &dt1, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &time1, RTC_FORMAT_BIN);

	PUTM_CAN::Telemetry_Main global_time
	{
		.Hour = time1.Hours,
	   	.Minutes = time1.Minutes,
		.Seconds = time1.Seconds,
		.device_state = state
	};
	auto telemtry_frame = PUTM_CAN::Can_tx_message<PUTM_CAN::Telemetry_Main>(global_time, PUTM_CAN::can_tx_header_TELEMETRY_MAIN);
	auto status = telemtry_frame.send(hcan1);
	if (status != HAL_StatusTypeDef::HAL_OK)
	{
    	Error_Handler();
	}
}

