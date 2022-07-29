#include "Data_Handling.hpp"
#include "Radio_Control.hpp"
#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"
#include "etl/circular_buffer.h"

//*********Messages ID*********//
/*
 * 65 - 'A' - APPS, LV,  HV Data.
 * 66 - 'B' - AQ Card Data.
 * 67 - 'C' - TC Data.
 * 68 - 'D' - TC Temps Data.
 * 69 - 'E' - SF Data and states.
 * 70 - 'F' - Lap timer pass.
 * 71 - 'G' - spare
 * 72 - 'H' - spare
 * 73 - 'I' - APPS, LV, HV, SF, AQ states.
 * 74 - 'J' - TC and ivnerter states.
 * 75 - 'K' -
 */

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

    }
    //handler1.Pass_States(uint8_t(PUTM_CAN::Telemetry_states::Gathering_Data));
}
uint8_t* Data_management::Check_DataBufferAPPSLVHV()
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
    DataBufferAPPSLVHV[0] = 65;
    DataBufferAPPSLVHV[1] = (uint8_t)(apps.pedal_position >> 8);
    DataBufferAPPSLVHV[2] = (uint8_t)(apps.pedal_position);
    DataBufferAPPSLVHV[3] = apps.position_diff;
    // LV Battery Data:
    DataBufferAPPSLVHV[4] = (uint8_t)(bmslv.voltage_sum >> 8);
    DataBufferAPPSLVHV[5] = (uint8_t)(bmslv.voltage_sum);
    DataBufferAPPSLVHV[6] = bmslv.soc;
    DataBufferAPPSLVHV[7] = bmslv.temp_avg;
    DataBufferAPPSLVHV[8] = bmslv.current;
    //HV Battery Data:
    DataBufferAPPSLVHV[9]  = (uint8_t)(bmshv.voltage_sum >> 8);
    DataBufferAPPSLVHV[10] = (uint8_t)(bmshv.voltage_sum);
    DataBufferAPPSLVHV[11] = (uint8_t)(bmshv.current >> 8);
    DataBufferAPPSLVHV[12] = (uint8_t)(bmshv.current);
    DataBufferAPPSLVHV[13] = (uint8_t)(bmshv.soc >> 8);
    DataBufferAPPSLVHV[14] = (uint8_t)(bmshv.soc);
    DataBufferAPPSLVHV[15] = bmshv.temp_max;
    DataBufferAPPSLVHV[16] = bmshv.temp_avg;
    //Steering Wheel Data:
    DataBufferAPPSLVHV[17] = (uint8_t)(sw.s_w_a >> 8);
    DataBufferAPPSLVHV[18] = (uint8_t)(sw.s_w_a);
    //Smart Fuses Data:
    DataBufferAPPSLVHV[19] = uint8_t(sf.device_state);
    // Include buffer flag.
    DataBufferAPPSLVHV[22] = DataBuffer1_flag;

    // also move states to Statebuffer.
    StateBuffer1_APPSLVHVSFAQ[0] = 73;
    StateBuffer1_APPSLVHVSFAQ[1] = (uint8_t)apps.device_state;
    StateBuffer1_APPSLVHVSFAQ[2] = (uint8_t)bmslv.device_state;
    StateBuffer1_APPSLVHVSFAQ[3] = (uint8_t)bmshv.device_state;
    StateBuffer1_APPSLVHVSFAQ[4] = (uint8_t)sw.device_state;
    StateBuffer1_APPSLVHVSFAQ[5] = (uint8_t)sf.device_state;

    // reset timers and heartbeat
    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    hb1 = HeartBeat::DEFAULT;

    handler1.Pass_States(uint8_t(PUTM_CAN::Telemetry_states::Transmitting));
    return DataBufferAPPSLVHV;
}
uint8_t* Data_management::Check_DataBufferAQ()
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

    DataBufferAQ[0] = 66;

    DataBufferAQ[1] = (uint8_t)(aq.suspension_right >> 8);
    DataBufferAQ[2] = (uint8_t)(aq.suspension_right);
    DataBufferAQ[3] = (uint8_t)(aq.suspension_left >> 8);
    DataBufferAQ[4] = (uint8_t)(aq.suspension_left);

    DataBufferAQ[5] = (uint8_t)(aq.brake_pressure_front >> 8);
    DataBufferAQ[6] = (uint8_t)(aq.brake_pressure_front);
    DataBufferAQ[7] = (uint8_t)(aq.brake_pressure_back >> 8);
    DataBufferAQ[8] = (uint8_t)(aq.brake_pressure_back);

    DataBufferAQ[9] = (uint8_t)(aq_gyro.speed_x >> 8);
    DataBufferAQ[10] = (uint8_t)(aq_gyro.speed_x);
    DataBufferAQ[11] = (uint8_t)(aq_gyro.speed_y >> 8);
    DataBufferAQ[12] = (uint8_t)(aq_gyro.speed_y);
    DataBufferAQ[13] = (uint8_t)(aq_gyro.speed_z >> 8);
    DataBufferAQ[14] = (uint8_t)(aq_gyro.speed_z);

    DataBufferAQ[15] = (uint8_t)(aq_acc.acc_x >> 8);
    DataBufferAQ[16] = (uint8_t)(aq_acc.acc_x);
    DataBufferAQ[17] = (uint8_t)(aq_acc.acc_y >> 8);
    DataBufferAQ[18] = (uint8_t)(aq_acc.acc_y);
    DataBufferAQ[19] = (uint8_t)(aq_acc.acc_z >> 8);
    DataBufferAQ[20] = (uint8_t)(aq_acc.acc_z);


    uint8_t Safety =
    		aq.bspd << 6 		  |
    		aq.driver_kill << 5   |
			aq.ebs << 4           |
			aq.inertia << 3 	  |
			aq.left_kill << 2     |
			aq.overtravel << 1    |
			aq.right_kill		  ;

    DataBufferAQ[21] = Safety;

    StateBuffer1_APPSLVHVSFAQ[6] = (uint8_t)aq.device_state;

    return DataBufferAQ;
}
uint8_t* Data_management::Check_DataBufferTC()
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

    //move data to buffer.
    DataBufferTC[0] =  67;
    DataBufferTC[1] =  (uint8_t)(tc_main.vehicle_speed >> 8);
    DataBufferTC[2] =  (uint8_t)(tc_main.vehicle_speed);
    DataBufferTC[5] =  tc_main.motor_current;
    DataBufferTC[3] =  (uint8_t)(tc_main.engine_speed >> 8);
    DataBufferTC[4] = (uint8_t)tc_main.engine_speed;
    DataBufferTC[6] =  tc_main.traction_control_enable;
    DataBufferTC[7] =  tc_main.traction_control_intensivity;

    DataBufferTC[8] =  (uint8_t)(tc_sus.adc_susp_left >> 8);
    DataBufferTC[9] =  (uint8_t)(tc_sus.adc_susp_left);
    DataBufferTC[10] = (uint8_t)(tc_sus.adc_susp_right >> 8);
    DataBufferTC[11] = (uint8_t)(tc_sus.adc_susp_right);

    DataBufferTC[12] = (uint8_t)(tc_wheels.left_front >> 8);
    DataBufferTC[13] = (uint8_t)(tc_wheels.left_front);
    DataBufferTC[14] = (uint8_t)(tc_wheels.right_front >> 8);
    DataBufferTC[15] = (uint8_t)(tc_wheels.right_front);
    DataBufferTC[16] = (uint8_t)(tc_wheels.left_rear >> 8);
    DataBufferTC[17] = (uint8_t)(tc_wheels.left_rear);
    DataBufferTC[18] = (uint8_t)(tc_wheels.right_rear >> 8);
    DataBufferTC[19] = (uint8_t)(tc_wheels.right_rear);

    DataBufferTC[20] = (uint8_t)(tc_imu_acc.acc_x >> 8);
    DataBufferTC[21] = (uint8_t)(tc_imu_acc.acc_x);
    DataBufferTC[22] = (uint8_t)(tc_imu_acc.acc_y >> 8);
    DataBufferTC[23] = (uint8_t)(tc_imu_acc.acc_y);
    DataBufferTC[24] = (uint8_t)(tc_imu_acc.acc_z >> 8);
    DataBufferTC[25] = (uint8_t)(tc_imu_acc.acc_z);

    DataBufferTC[26] = (uint8_t)(tc_imu_gyro.gyro_x >> 8);
    DataBufferTC[27] = (uint8_t)(tc_imu_gyro.gyro_x);
    DataBufferTC[28] = (uint8_t)(tc_imu_gyro.gyro_y >> 8);
    DataBufferTC[29] = (uint8_t)(tc_imu_gyro.gyro_y);
    DataBufferTC[30] = (uint8_t)(tc_imu_gyro.gyro_z >> 8);
    DataBufferTC[31] = (uint8_t)(tc_imu_gyro.gyro_z);

    DataBufferTC[32] = DataBuffer2_flag;

    StateBuffer2_TC[0] = 74;
    StateBuffer2_TC[1] = (uint8_t)(tc_main.device_state);

    /*
    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    __HAL_TIM_SET_COUNTER(&htim6, 0); //?
    */
    hb1 = HeartBeat::DEFAULT;

    return DataBufferTC;
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

    DataBufferTemps[0] = 68;
    DataBufferTemps[1] = tc_temps.engine;
    DataBufferTemps[2] = tc_temps.inverter;
    DataBufferTemps[3] = tc_temps.water_pressure_in;
    DataBufferTemps[4] = tc_temps.water_pressure_out;
    DataBufferTemps[5] = tc_temps.water_temp_in;
    DataBufferTemps[6] = tc_temps.water_temp_out;

    return DataBufferTemps;
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
    auto sf_f_box = PUTM_CAN::can.get_sf_main();
    auto sf_passive_elements = PUTM_CAN::can.get_sf_passive_elements();
    auto sf_supply = PUTM_CAN::can.get_sf_supply();
    auto sf_safety = PUTM_CAN::can.get_sf_safety();

    DataBufferSFy[0] = 69;

    DataBufferSFy[1] =  uint8_t(sf_f_box.device_state);
    DataBufferSFy[2] =  uint8_t(sf_f_box.fuse_0_state);
    DataBufferSFy[3] =  uint8_t(sf_f_box.fuse_1_state);
    DataBufferSFy[4] =  uint8_t(sf_f_box.fuse_2_state);
    DataBufferSFy[5] =  uint8_t(sf_f_box.fuse_3_state);

    DataBufferSFy[6] =  uint8_t(sf_passive_elements.break_light);
    DataBufferSFy[7] =  uint8_t(sf_passive_elements.fan_l);
    DataBufferSFy[8] =  uint8_t(sf_passive_elements.fan_r);
    DataBufferSFy[9] =  uint8_t(sf_passive_elements.fan_mono);
    DataBufferSFy[10] = uint8_t(sf_passive_elements.tsal_assi);
    DataBufferSFy[11] = uint8_t(sf_passive_elements.water_potentiometer);
    DataBufferSFy[12] = uint8_t(sf_passive_elements.wheel_speed_1);
    DataBufferSFy[13] = uint8_t(sf_passive_elements.wheel_speed_2);

    DataBufferSFy[14] = uint8_t(sf_supply.bat_hv);
    DataBufferSFy[15] = uint8_t(sf_supply.dash);

    uint8_t Safety =
       		sf_safety.dv << 5 		|
			sf_safety.firewall << 4 |
			sf_safety.hvd << 3      |
			sf_safety.inverter << 2 |
   			sf_safety.tsms << 1     ;

    DataBufferSFy[21] = Safety;

    return DataBufferSFy;
}
uint8_t* Data_management::Check_BufferLowFreq()
{

}
uint8_t* Data_management::Laptimer_indicator()
{
	DataBufferLapTimer[0] = 71;
	DataBufferLapTimer[1] = 0xff;
	return DataBufferLapTimer;
}
void Data_management::Clear_msg1()
{
    memset(DataBufferAPPSLVHV, 0, sizeof(DataBufferAPPSLVHV));
    DataBuffer1_flag = 0;
}
void Data_management::Clear_msg2()
{
    memset(DataBufferTC, 0, sizeof(DataBufferTC));
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
	if(PUTM_CAN::can.get_laptimer_pass_new_data() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
