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
    auto aq = PUTM_CAN::can.get_aq_main();
    auto aq_gyro = PUTM_CAN::can.get_aq_gyroscope();
    auto aq_acc = PUTM_CAN::can.get_aq_acceleration();

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
    memcpy(ptr, &aq, sizeof(aq));
    ptr += sizeof(aq);
    memcpy(ptr, &aq_gyro, sizeof(aq_gyro));
    ptr += sizeof(aq_gyro);
    memcpy(ptr, &aq_acc, sizeof(aq_acc));
    // move data to buffer.
    // APPS Data:
    /*
    DataBuffer1[0] = 'A';
    DataBuffer1[1] = (uint8_t)(apps.pedal_position);
    DataBuffer1[2] = (uint8_t)(apps.pedal_position >> 8);
    DataBuffer1[3] = apps.position_diff;
    // LV Battery Data:
    DataBuffer1[4] = (uint8_t)(bmslv.voltage_sum);
    DataBuffer1[5] = (uint8_t)(bmslv.voltage_sum >> 8);
    DataBuffer1[6] = bmslv.soc;
    DataBuffer1[7] = bmslv.temp_avg;
    DataBuffer1[8] = bmslv.current;
    //HV Battery Data:
    DataBuffer1[9]  = (uint8_t)(bmshv.voltage_sum);
    DataBuffer1[10] = (uint8_t)(bmshv.voltage_sum >> 8);
    DataBuffer1[11] = (uint8_t)(bmshv.current);
    DataBuffer1[12] = (uint8_t)(bmshv.current >> 8);
    DataBuffer1[13] = (uint8_t)(bmshv.soc);
    DataBuffer1[14] = (uint8_t)(bmshv.soc >> 8);
    DataBuffer1[15] = bmshv.temp_max;
    DataBuffer1[16] = bmshv.temp_avg;
    //Steering Wheel Data:
    DataBuffer1[17] = (uint8_t)(sw.s_w_a);
    DataBuffer1[18] = (uint8_t)(sw.s_w_a >> 8);
    //Smart Fuses Data:
    DataBuffer1[19] = ((sf.fuses_overall_state.ok << 3) |
                       (sf.fuses_overall_state.overcurrent << 2) |
                       (sf.fuses_overall_state.overheat << 1) |
                       sf.fuses_overall_state.undercurrent);
    DataBuffer1[20] = (uint8_t)(sf.fuses_overall_state.current);
    DataBuffer1[21] = (uint8_t)(sf.fuses_overall_state.current >> 8);
    */
    hb1 = HeartBeat::DEFAULT;
    return DataBuffer100hz;
}
/*
uint8_t* Data_management::aq()
{
    /*
    DataBufferAQ[0] = 'B';

    DataBufferAQ[1] = (uint8_t)(aq.suspension_right);
    DataBufferAQ[2] = (uint8_t)(aq.suspension_right >> 8);
    DataBufferAQ[3] = (uint8_t)(aq.suspension_left);
    DataBufferAQ[4] = (uint8_t)(aq.suspension_left >> 8);

    DataBufferAQ[5] = (uint8_t)(aq.brake_pressure_front);
    DataBufferAQ[6] = (uint8_t)(aq.brake_pressure_front >> 8);
    DataBufferAQ[7] = (uint8_t)(aq.brake_pressure_back);
    DataBufferAQ[8] = (uint8_t)(aq.brake_pressure_back >> 8);

    DataBufferAQ[9]  = (uint8_t)(aq_gyro.speed_x);
    DataBufferAQ[10] = (uint8_t)(aq_gyro.speed_x >> 8);
    DataBufferAQ[11] = (uint8_t)(aq_gyro.speed_y);
    DataBufferAQ[12] = (uint8_t)(aq_gyro.speed_y >> 8);
    DataBufferAQ[13] = (uint8_t)(aq_gyro.speed_z);
    DataBufferAQ[14] = (uint8_t)(aq_gyro.speed_z >> 8);

    DataBufferAQ[15] = (uint8_t)(aq_acc.acc_x);
    DataBufferAQ[16] = (uint8_t)(aq_acc.acc_x >> 8);
    DataBufferAQ[17] = (uint8_t)(aq_acc.acc_y);
    DataBufferAQ[18] = (uint8_t)(aq_acc.acc_y >> 8);
    DataBufferAQ[19] = (uint8_t)(aq_acc.acc_z);
    DataBufferAQ[20] = (uint8_t)(aq_acc.acc_z >> 8);

    uint8_t Safety =
    		aq.bspd << 6 		  |
    		aq.driver_kill << 5   |
			aq.ebs << 4           |
			aq.inertia << 3 	  |
			aq.left_kill << 2     |
			aq.overtravel << 1    |
			aq.right_kill		  ;

    DataBufferAQ[21] = Safety;

    return DataBufferAQ;
}
*/
uint8_t* Data_management::Check_Buffer50hz()
{
    auto tc_main = PUTM_CAN::can.get_tc_main();
    auto tc_sus = PUTM_CAN::can.get_tc_rear();
    auto tc_wheels = PUTM_CAN::can.get_tc_wheel_velocities();
    auto tc_imu_acc = PUTM_CAN::can.get_tc_imu_acc();
    auto tc_imu_gyro = PUTM_CAN::can.get_tc_imu_gyro();

    // move data to buffer.
    /*
    DataBuffer2[0] = 'C';
    DataBuffer2[1] = (uint8_t)(tc_main.vehicle_speed);
    DataBuffer2[2] = (uint8_t)(tc_main.vehicle_speed >> 8);
    DataBuffer2[5] = tc_main.motor_current;
    DataBuffer2[3] = (uint8_t)(tc_main.engine_speed);
    DataBuffer2[4] = (uint8_t)(tc_main.engine_speed >> 8);
    DataBuffer2[6] = tc_main.traction_control_enable;
    DataBuffer2[7] = tc_main.traction_control_intensivity;

    DataBuffer2[8] = (uint8_t)(tc_sus.adc_susp_left);
    DataBuffer2[9] = (uint8_t)(tc_sus.adc_susp_left >> 8);
    DataBuffer2[10] = (uint8_t)(tc_sus.adc_susp_right);
    DataBuffer2[11] = (uint8_t)(tc_sus.adc_susp_right >> 8);

    DataBuffer2[12] = (uint8_t)(tc_wheels.left_front);
    DataBuffer2[13] = (uint8_t)(tc_wheels.left_front >> 8);
    DataBuffer2[14] = (uint8_t)(tc_wheels.right_front);
    DataBuffer2[15] = (uint8_t)(tc_wheels.right_front >> 8);
    DataBuffer2[16] = (uint8_t)(tc_wheels.left_rear);
    DataBuffer2[17] = (uint8_t)(tc_wheels.left_rear >> 8);
    DataBuffer2[18] = (uint8_t)(tc_wheels.right_rear);
    DataBuffer2[19] = (uint8_t)(tc_wheels.right_rear >> 8);

    DataBuffer2[20] = (uint8_t)(tc_imu_acc.acc_x);
    DataBuffer2[21] = (uint8_t)(tc_imu_acc.acc_x >> 8);
    DataBuffer2[22] = (uint8_t)(tc_imu_acc.acc_y);
    DataBuffer2[23] = (uint8_t)(tc_imu_acc.acc_y >> 8);
    DataBuffer2[24] = (uint8_t)(tc_imu_acc.acc_z);
    DataBuffer2[25] = (uint8_t)(tc_imu_acc.acc_z >> 8);

    DataBuffer2[26] = (uint8_t)(tc_imu_gyro.gyro_x);
    DataBuffer2[27] = (uint8_t)(tc_imu_gyro.gyro_x >> 8);
    DataBuffer2[28] = (uint8_t)(tc_imu_gyro.gyro_y);
    DataBuffer2[29] = (uint8_t)(tc_imu_gyro.gyro_y >> 8);
    DataBuffer2[30] = (uint8_t)(tc_imu_gyro.gyro_z);
    DataBuffer2[31] = (uint8_t)(tc_imu_gyro.gyro_z >> 8);

    DataBuffer2[32] = DataBuffer2_flag;
	*/
    hb1 = HeartBeat::DEFAULT;
    return DataBuffer50hz;
}

uint8_t* Data_management::Check_Buffer10hz()
{

    auto tc_temps = PUTM_CAN::can.get_tc_temperatures();

    /*
    DataBuffer2[0] = 'D';
    DataBuffer2[1] = tc_temps.engine;
    DataBuffer2[2] = tc_temps.inverter;
    DataBuffer2[3] = tc_temps.water_pressure_in;
    DataBuffer2[4] = tc_temps.water_pressure_out;
    DataBuffer2[5] = tc_temps.water_temp_in;
    DataBuffer2[6] = tc_temps.water_temp_out;
    */


    return DataBuffer10hz;
}
uint8_t* Data_management::Check_Buffer1hz()
{


    return DataBuffer1hz;
}
void Data_management::Clear_msg1()
{
    memset(DataBuffer100hz, 0, sizeof(DataBuffer100hz));
}
void Data_management::Clear_msg2()
{
    memset(DataBuffer50hz, 0, sizeof(DataBuffer50hz));
}
void Data_management::Clear_msg3()
{

}

/*
 *
 *
 * Frame by frame
 *
 *
 */

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

