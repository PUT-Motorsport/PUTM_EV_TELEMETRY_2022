#include "Data_handling.hpp"
#include "Radio_Control.hpp"
#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"

extern HeartBeat hb1;

extern CAN_HandleTypeDef hcan1;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;

bool timer_interrupt = true;

void Data_management::Init() {}

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
        RTC_TimeTypeDef time1;
        RTC_DateTypeDef dt1;

        HAL_RTC_GetDate(&hrtc, &dt1, RTC_FORMAT_BIN);
        HAL_RTC_GetTime(&hrtc, &time1, RTC_FORMAT_BIN);

        Telemetry_Main global_time{.device_state = Telemetry_states::Power_up};
        auto Telemetry_main = PUTM_CAN::Can_tx_message<Telemetry_Main>(
            global_time, can_tx_header_TELEMETRY_MAIN);
        auto status = Telemetry_main.send(hcan1);
        if (status != HAL_StatusTypeDef::HAL_OK)
        {
            Error_Handler();
        }
    }

    // DataBuffer1 HeartBeat
    if (htim->Instance == TIM3)
    {
        hb1 = HeartBeat::Buffer1;
    }
    // DataBuffer2 HeartBeat
    else if (htim->Instance == TIM4)
    {
        hb1 = HeartBeat::Buffer2;
    }
    // DataBuffer3 HeartBeat
    else if (htim->Instance == TIM5)
    {
        hb1 = HeartBeat::Buffer3;
    }

    if (htim->Instance == TIM6)
    {
        timer_interrupt = !timer_interrupt;
    }
}
uint8_t* Data_management::Check_Buffer1()
{
    // Timer to avoid endless loops
    Adjust_Waiting_time(10);

    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        // Jak to zrobić na range loopie?
        for (int dev = 0; dev <= 5; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer1_flag = DataBuffer1_flag || device->get_position();
            }
        }
    }

    // Get data
    auto apps = PUTM_CAN::can.get_apps_main();
    auto bmslv = PUTM_CAN::can.get_bms_lv_main();
    auto aq = PUTM_CAN::can.get_aq_main();
    auto bmshv = PUTM_CAN::can.get_bms_hv_main();
    auto sf = PUTM_CAN::can.get_sf_main();
    auto sw = PUTM_CAN::can.get_steering_wheel_main();

    // move data to buffer.
    DataBuffer1[0] = 'A';
    DataBuffer1[1] = (uint8_t)(apps.pedal_position >> 8);
    DataBuffer1[2] = (uint8_t)(apps.pedal_position);
    DataBuffer1[3] = apps.position_diff;
    DataBuffer1[4] = (uint8_t)(bmslv.voltage_sum >> 8);
    DataBuffer1[5] = (uint8_t)(bmslv.voltage_sum);
    DataBuffer1[6] = bmslv.soc;
    DataBuffer1[7] = bmslv.temp_avg;
    DataBuffer1[8] = bmslv.current;
    DataBuffer1[9] = (uint8_t)(aq.adc_susp_right >> 8);
    DataBuffer1[10] = (uint8_t)aq.adc_susp_right;
    DataBuffer1[11] = (uint8_t)(aq.adc_susp_left >> 8);
    DataBuffer1[12] = (uint8_t)(aq.adc_susp_left);
    DataBuffer1[13] = aq.brake_pressure_front;
    DataBuffer1[14] = aq.brake_pressure_back;
    DataBuffer1[15] = 0;
    DataBuffer1[16] = (uint8_t)(bmshv.voltage_sum >> 8);
    DataBuffer1[17] = (uint8_t)(bmshv.voltage_sum);
    DataBuffer1[18] = bmshv.soc;
    DataBuffer1[19] = bmshv.temp_max;
    DataBuffer1[20] = bmshv.temp_avg;
    DataBuffer1[21] = bmshv.current;
    DataBuffer1[22] = ((sf.fuses_overall_state.ok << 3) ||
                       (sf.fuses_overall_state.overcurrent << 2) ||
                       (sf.fuses_overall_state.overheat << 1) ||
                       sf.fuses_overall_state.undercurrent);
    DataBuffer1[23] = (uint8_t)(sf.fuses_overall_state.current >> 8);
    DataBuffer1[24] = (uint8_t)(sf.fuses_overall_state.current);
    DataBuffer1[25] = (uint8_t)(sw.s_w_a >> 8);
    DataBuffer1[26] = (uint8_t)(sw.s_w_a);

    // test
    for (int i = 3; i < 27; i++)
    {
        DataBuffer1[i] = 100;
    }

    // Include buffer flag.
    DataBuffer1[27] = DataBuffer1_flag;

    // also move states to Statebuffer.
    StateBuffer1[0] = 'D';
    StateBuffer1[1] = (uint8_t)apps.device_state;
    StateBuffer1[2] = (uint8_t)bmslv.device_state;
    StateBuffer1[3] = 0;
    StateBuffer1[4] = (uint8_t)bmshv.device_state;
    StateBuffer1[5] = (uint8_t)sf.device_state;
    StateBuffer1[6] = (uint8_t)sw.device_state;

    // reset timers and heartbeat
    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    hb1 = HeartBeat::DEFAULT;

    return DataBuffer1;
}
uint8_t* Data_management::Check_Buffer2()
{
    Adjust_Waiting_time(500);
    while ((DataBuffer1_flag != Buffer1_full) && (timer_interrupt == false))
    {
        for (int dev = 6; dev <= 7; dev++)
        {
            auto& device = PUTM_CAN::can.device_array[dev];
            if (device->get_new_data() == true)
            {
                DataBuffer2_flag = DataBuffer2_flag || device->get_position();
            }
        }
    }
    // Get data

    auto tc_main = PUTM_CAN::can.get_tc_main();
    auto tc_sus = PUTM_CAN::can.get_tc_rear();

    // move data to buffer.

    DataBuffer2[0] = 'B';

    StateBuffer2[0] = 'E';

    timer_interrupt = false;
    HAL_TIM_Base_Stop_IT(&htim6);
    __HAL_TIM_SET_COUNTER(&htim6, 0); //?
    hb1 = HeartBeat::DEFAULT;

    return DataBuffer2;
}
uint8_t* Data_management::Check_Buffer3() { return DataBuffer3; }
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
    memset(DataBuffer3, 0, sizeof(DataBuffer3));
    DataBuffer3_flag = 0;
}
void Send_Frame()
{
    Apps_main apps_test{
        .pedal_position = rand() % 2000,
        .counter = 0,
        .position_diff = 0,
        .device_state = Apps_states::Normal_operation,
    };
    auto apps_main_frame =
        PUTM_CAN::Can_tx_message<Apps_main>(apps_test, can_tx_header_APPS_MAIN);
    auto status = apps_main_frame.send(hcan1);
}
/*
 *
 *
 * Frame by frame
 *
 *
 */
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

        FrameBuffer[2] = (uint8_t)(aq.adc_susp_right >> 8);
        FrameBuffer[3] = (uint8_t)aq.adc_susp_right;
        FrameBuffer[4] = (uint8_t)(aq.adc_susp_left >> 8);
        FrameBuffer[5] = (uint8_t)(aq.adc_susp_left);
        FrameBuffer[6] = aq.brake_pressure_front;
        FrameBuffer[7] = aq.brake_pressure_back;
        // nie ma state'u aq.

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
        FrameBuffer[7] = bmshv.current;
        FrameBuffer[8] = (uint8_t)bmshv.device_state;

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
    if (PUTM_CAN::can.get_ts_main_new_data() == true)
    {
        auto tc = PUTM_CAN::can.get_tc_main();

        FrameBuffer[0] = (uint8_t)(TS_MAIN_CAN_ID >> 8);
        FrameBuffer[1] = (uint8_t)(TS_MAIN_CAN_ID);

        FrameBuffer[2] = (uint8_t)(tc.vehicle_speed >> 8);
        FrameBuffer[3] = (uint8_t)(tc.vehicle_speed);
        FrameBuffer[4] = tc.motor_current;
        FrameBuffer[5] = tc.water_pressure;
        FrameBuffer[6] = tc.water_temp;
        FrameBuffer[7] = tc.traction_control_intensivity;
        FrameBuffer[8] = tc.brake_light_active << 5 || tc.regen_active << 4 ||
                         tc.regen_enable << 3 || tc.rtds_active << 2 ||
                         tc.traction_control_enable << 1 ||
                         tc.tractive_system_on;

        Send_Data(FrameBuffer);
    }
}
