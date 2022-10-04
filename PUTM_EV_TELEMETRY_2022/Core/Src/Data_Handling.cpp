#include "Data_Handling.hpp"
#include "Radio_Control.hpp"
#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"
#include "etl/circular_buffer.h"

extern HeartBeat hb1;
extern Data_management handler1;

extern CAN_HandleTypeDef hcan1;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

time lap_time;

bool timer_interrupt = true;
bool Lap_is_measured = false;
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
    // DataBuffer1 HeartBeat
    if (htim->Instance == TIM3)
    {
        hb1 = HeartBeat::Buffer1;
    }
    // DataBuffer2 HeartBeat
    if (htim->Instance == TIM4)
    {
        hb1 = HeartBeat::Buffer2;
    }
    // DataBuffer3 HeartBeat
    if (htim->Instance == TIM5)
    {
        hb1 = HeartBeat::Buffer3;
    }

    if (htim->Instance == TIM6)
    {
        timer_interrupt = !timer_interrupt;
    }
    if(htim->Instance == TIM7)
    {
    	Update_Time();
    }
    //handler1.Pass_States(uint8_t(PUTM_CAN::Telemetry_states::Gathering_Data));
}
uint8_t* Data_management::Check_Buffer1()
{
    // Timer to avoid endless loops
	/*
    Adjust_Waiting_time(10);

    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        // Jak to zrobić na range loopie?
        for (int dev = 0; dev <= 4; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer1_flag = DataBuffer1_flag || device->get_position();
            }
        }
    }
	*/
    // Get data
    auto apps = PUTM_CAN::can.get_apps_main();
    auto bmslv = PUTM_CAN::can.get_bms_lv_main();
    auto sf = PUTM_CAN::can.get_sf_main();
    auto bmshv = PUTM_CAN::can.get_bms_hv_main();
    auto sw = PUTM_CAN::can.get_steering_wheel_main();

    // move data to buffer.
    // APPS Data:
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

    // Include buffer flag.
    DataBuffer1[22] = DataBuffer1_flag;

    // also move states to Statebuffer.
    StateBuffer1[0] = 'G';
    StateBuffer1[1] = (uint8_t)apps.device_state;
    StateBuffer1[2] = (uint8_t)bmslv.device_state;
    StateBuffer1[3] = (uint8_t)bmshv.device_state;
    StateBuffer1[4] = (uint8_t)sw.device_state;
    StateBuffer1[5] = (uint8_t)sf.device_state;

    // reset timers and heartbeat
    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    hb1 = HeartBeat::DEFAULT;

    handler1.Pass_States(uint8_t(PUTM_CAN::Telemetry_states::Transmitting));
    return DataBuffer1;
}
uint8_t* Data_management::Check_AQBuffer()
{
	/*
    Adjust_Waiting_time(10);

    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        // Jak to zrobić na range loopie?
        for (int dev = 5; dev <= 7; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer1_flag = DataBuffer1_flag || device->get_position();
            }
        }
    }
	*/
    auto aq = PUTM_CAN::can.get_aq_main();
    auto aq_gyro = PUTM_CAN::can.get_aq_gyroscope();
    auto aq_acc = PUTM_CAN::can.get_aq_acceleration();

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
uint8_t* Data_management::Check_Buffer2()
{
	/*
    Adjust_Waiting_time(500);
    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        for (int dev = 8; dev <= 12; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer2_flag = DataBuffer2_flag || device->get_position();
            }
        }
    }
    // Get data
	*/
    auto tc_main = PUTM_CAN::can.get_tc_main();
    auto tc_sus = PUTM_CAN::can.get_tc_rear();
    auto tc_wheels = PUTM_CAN::can.get_tc_wheel_velocities();
    auto tc_imu_acc = PUTM_CAN::can.get_tc_imu_acc();
    auto tc_imu_gyro = PUTM_CAN::can.get_tc_imu_gyro();

    // move data to buffer.
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

    StateBuffer2[0] = 'E';
    StateBuffer2[1] = (uint8_t)(tc_main.device_state);

    /*
    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    __HAL_TIM_SET_COUNTER(&htim6, 0); //?
    */
    hb1 = HeartBeat::DEFAULT;

    return DataBuffer2;
}
uint8_t* Data_management::Check_BufferSFy()
{
	/*
    Adjust_Waiting_time(500);
    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        for (int dev = 13; dev <= 12; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer2_flag = DataBuffer2_flag || device->get_position();
            }
        }
    }
    */
    // Get data
    auto sf_f_box = PUTM_CAN::can.get_sf_frontbox();

    DataBufferSFy[0] = 'F';

    uint8_t states = ((sf_f_box.fuse_0_apps.ok << 3) ||
                      (sf_f_box.fuse_0_apps.overcurrent << 2) ||
                      (sf_f_box.fuse_0_apps.overheat << 1) ||
                      (sf_f_box.fuse_0_apps.undercurrent) << 4);
    DataBufferSFy[1] = (uint8_t)(sf_f_box.fuse_0_apps.current >> 4 || states);
    DataBufferSFy[2] = (uint8_t)(sf_f_box.fuse_0_apps.current);

    auto sf_safety = PUTM_CAN::can.get_sf_safety();

    /*
    DataBuffer1[18] = ((sf.fuses_overall_state.ok << 3) ||
                           (sf.fuses_overall_state.overcurrent << 2) ||
                           (sf.fuses_overall_state.overheat << 1) ||
                           sf.fuses_overall_state.undercurrent);
    DataBuffer1[19] = (uint8_t)(sf.fuses_overall_state.current >> 8);
    DataBuffer1[20] = (uint8_t)(sf.fuses_overall_state.current);
*/
    uint8_t Safety =
       		sf_safety.dv << 5 		|
			sf_safety.firewall << 4 |
			sf_safety.hvd << 3      |
			sf_safety.inverter << 2 |
   			sf_safety.tsms << 1     ;

       DataBufferSFy[3] = Safety;

    return DataBufferSFy;
}
uint8_t* Data_management::Check_BufferTtemps()
{
	/*

    Adjust_Waiting_time(500);
    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        for (int dev = 13; dev <= 12; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer2_flag = DataBuffer2_flag || device->get_position();
            }
        }
    }
    */

    auto tc_temps = PUTM_CAN::can.get_tc_temperatures();


    DataBuffer2[0] = 'D';
    DataBuffer2[1] = tc_temps.engine;
    DataBuffer2[2] = tc_temps.inverter;
    DataBuffer2[3] = tc_temps.water_pressure_in;
    DataBuffer2[4] = tc_temps.water_pressure_out;
    DataBuffer2[5] = tc_temps.water_temp_in;
    DataBuffer2[6] = tc_temps.water_temp_out;

    return DataBuffer2;
}
void Data_management::Clear_msg1()
{
    memset(DataBuffer1, 0, sizeof(DataBuffer1));
    DataBuffer1_flag = 0;
}
void Data_management::Clear_msg2()
{
    memset(DataBuffer2, 0, sizeof(DataBuffer2));
    DataBuffer2_flag = 0;
}
void Data_management::Clear_msg3()
{
   // memset(Buffer3_SFy, 0, sizeof(Buffer3_SFy));
    //DataBuffer3_flag = 0;
}

/*
 *
 *
 * Frame by frame
 *
 *
 */

using namespace PUTM_CAN;

void Cycle_frames()
{
    uint8_t FrameBuffer[12] = {0};

    FrameBuffer[10] = 0xff;
    FrameBuffer[11] = 0xff;
    FrameBuffer[12] = 0xff;

    if (PUTM_CAN::can.get_apps_main_new_data() == true)
    {
        auto apps = PUTM_CAN::can.get_apps_main();

        FrameBuffer[0] = (uint8_t)(APPS_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(APPS_MAIN_CAN_ID);

        FrameBuffer[2] = (uint8_t)(apps.pedal_position >> 8);
        FrameBuffer[3] = (uint8_t)(apps.pedal_position);
        FrameBuffer[4] = apps.position_diff;
        FrameBuffer[5] = (uint8_t)apps.device_state;

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_bms_lv_main_new_data() == true)
    {
        auto bmslv = PUTM_CAN::can.get_bms_lv_main();

        FrameBuffer[0] = (uint8_t)(BMS_LV_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(BMS_LV_MAIN_CAN_ID);

        FrameBuffer[1] = (uint8_t)(bmslv.voltage_sum >> 8);
        FrameBuffer[2] = (uint8_t)(bmslv.voltage_sum);
        FrameBuffer[3] = bmslv.soc;
        FrameBuffer[4] = bmslv.temp_avg;
        FrameBuffer[5] = bmslv.current;
        FrameBuffer[6] = (uint8_t)bmslv.device_state;

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_aq_main_new_data() == true)
    {
        auto aq = PUTM_CAN::can.get_aq_main();

        FrameBuffer[0] = (uint8_t)(AQ_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(AQ_MAIN_CAN_ID);

        FrameBuffer[1] = (uint8_t)(aq.suspension_right >> 8);
        FrameBuffer[2] = (uint8_t)(aq.suspension_right);
        FrameBuffer[3] = (uint8_t)(aq.suspension_left >> 8);
        FrameBuffer[4] = (uint8_t)(aq.suspension_left);
        FrameBuffer[5] = (uint8_t)(aq.brake_pressure_front >> 8);
        FrameBuffer[6] = (uint8_t)(aq.brake_pressure_front);
        FrameBuffer[7] = (uint8_t)(aq.brake_pressure_back >> 8);
        FrameBuffer[8] = (uint8_t)(aq.brake_pressure_back);

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_aq_acceleration_new_data() == true)
    {
        auto aq_acc = PUTM_CAN::can.get_aq_acceleration();

        FrameBuffer[0] = (uint8_t)(AQ_ACCELERATION_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(AQ_ACCELERATION_CAN_ID);

        FrameBuffer[2] = (uint8_t)(aq_acc.acc_x >> 8);
        FrameBuffer[3] = (uint8_t)(aq_acc.acc_x);

        FrameBuffer[4] = (uint8_t)(aq_acc.acc_y >> 8);
        FrameBuffer[5] = (uint8_t)(aq_acc.acc_y);

        FrameBuffer[6] = (uint8_t)(aq_acc.acc_z >> 8);
        FrameBuffer[7] = (uint8_t)(aq_acc.acc_z);

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_aq_gryoscope_new_data() == true)
    {

        auto aq_gyro = PUTM_CAN::can.get_aq_gyroscope();

        FrameBuffer[0] = (uint8_t)(AQ_GYROSCOPE_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(AQ_GYROSCOPE_CAN_ID);

        FrameBuffer[2] = (uint8_t)(aq_gyro.speed_x >> 8);
        FrameBuffer[3] = (uint8_t)(aq_gyro.speed_x);

        FrameBuffer[4] = (uint8_t)(aq_gyro.speed_y >> 8);
        FrameBuffer[5] = (uint8_t)(aq_gyro.speed_y);

        FrameBuffer[6] = (uint8_t)(aq_gyro.speed_z >> 8);
        FrameBuffer[7] = (uint8_t)(aq_gyro.speed_z);

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_bms_hv_main_new_data() == true)
    {
        auto bmshv = PUTM_CAN::can.get_bms_hv_main();

        FrameBuffer[0] = (uint8_t)(BMS_HV_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(BMS_HV_MAIN_CAN_ID);

        FrameBuffer[2] = (uint8_t)(bmshv.voltage_sum >> 8);
        FrameBuffer[3] = (uint8_t)(bmshv.voltage_sum);

        FrameBuffer[4] = bmshv.soc;
        FrameBuffer[5] = bmshv.temp_max;
        FrameBuffer[6] = bmshv.temp_avg;
        FrameBuffer[8] = (uint8_t)(bmshv.current >> 8);
        FrameBuffer[9] = (uint8_t)(bmshv.current);
        FrameBuffer[10] = (uint8_t)(bmshv.device_state);

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_sf_main_new_data() == true)
    {
        auto sf = PUTM_CAN::can.get_sf_main();

        FrameBuffer[0] = (uint8_t)(SF_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(SF_MAIN_CAN_ID);

        FrameBuffer[2] = ((sf.fuses_overall_state.ok << 3) ||
                          (sf.fuses_overall_state.overcurrent << 2) ||
                          (sf.fuses_overall_state.overheat << 1) ||
                          sf.fuses_overall_state.undercurrent);
        FrameBuffer[3] = (uint8_t)(sf.fuses_overall_state.current >> 8);
        FrameBuffer[4] = (uint8_t)(sf.fuses_overall_state.current);

        Send_Data(FrameBuffer);
    }
    if (PUTM_CAN::can.get_tc_main_new_data() == true)
    {
        auto tc = PUTM_CAN::can.get_tc_main();

        FrameBuffer[0] = (uint8_t)(TC_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(TC_MAIN_CAN_ID);

        FrameBuffer[2] = (uint8_t)(tc.vehicle_speed >> 8);
        FrameBuffer[3] = (uint8_t)(tc.vehicle_speed);
        FrameBuffer[4] = tc.motor_current;
        // FrameBuffer[5] = tc.water_pressure;
        // FrameBuffer[6] = tc.water_temp;
        FrameBuffer[7] = tc.traction_control_intensivity;
        FrameBuffer[8] = tc.brake_light_active << 5 || tc.regen_active << 4 ||
                         tc.regen_enable << 3 || tc.rtds_active << 2 ||
                         tc.traction_control_enable << 1 ||
                         tc.tractive_system_on;
        FrameBuffer[9] = (uint8_t)(tc.device_state);
        Send_Data(FrameBuffer);
    }
    if(PUTM_CAN::can.get_laptimer_pass_new_data() == true)
    {
    	auto laptimer = PUTM_CAN::can.get_laptimer_pass();
    }
    if(PUTM_CAN::can.get_tc_temperatures_new_data() == true)
    {
    	auto tc_temps = PUTM_CAN::can.get_tc_temperatures();

    	  FrameBuffer[0] = (uint8_t)(TC_TEMPERATURES_CAN_ID >> 8);
    	  FrameBuffer[1] = (uint8_t)(TC_TEMPERATURES_CAN_ID);

    	  FrameBuffer[2] = (uint8_t)(tc_temps.engine);
    	  FrameBuffer[3] = (uint8_t)(tc_temps.inverter);

    	  FrameBuffer[4] = (uint8_t)(tc_temps.water_pressure_in);
    	  FrameBuffer[5] = (uint8_t)(tc_temps.water_pressure_out);

    	  FrameBuffer[6] = (uint8_t)(tc_temps.water_temp_in);
    	  FrameBuffer[7] = (uint8_t)(tc_temps.water_temp_out);

    	  Send_Data(FrameBuffer);

    }
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
void Data_management::Pass_States(uint8_t state)
{
	for(int i = 0; i <= 5; i++)
	{
		if(uint8_t(States_buffer[i]) == state)
		{

		}
		else
		{
			States_buffer.push(PUTM_CAN::Telemetry_states(state));
		}
	}
}
bool Check_Laptimer()
{
	if((PUTM_CAN::can.get_laptimer_pass_new_data() == true) & (Lap_is_measured == false))
	{
		Lap_is_measured = true;
		HAL_TIM_Base_Start_IT(&htim7);
		return false;
	}
	else if((PUTM_CAN::can.get_laptimer_pass_new_data() == true) & (Lap_is_measured == true))
	{
		Lap_is_measured = false;
		return true;
	}
}
void Update_Time()
{
			lap_time.miliseconds++;
			if(lap_time.miliseconds == 1000)
			{
				lap_time.seconds++;
				lap_time.miliseconds = 0;
			}
			if(lap_time.seconds == 60)
			{
					lap_time.minute++;
					lap_time.seconds = 0;
			}
}
void reset_time()
{
	lap_time.minute = 0;
	lap_time.seconds = 0;
	lap_time.miliseconds = 0;
}
time return_lap_time()
{
	return lap_time;
}
