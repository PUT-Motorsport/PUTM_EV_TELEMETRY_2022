/*
 * Device_States.hpp
 *
 *  Created on: Jul 28, 2022
 *      Author: wasyl
 */

#ifndef INC_DEVICE_STATES_HPP_
#define INC_DEVICE_STATES_HPP_

#include "main.h"
#include <cstdlib>
#include <string>

enum struct Apps_states: uint8_t {
	Normal_operation,
	Power_up,
	Sensor_Implausiblity,
	Left_sensor_out_of_range_lower_bound,
	Left_sensor_out_of_range_upper_bound,
	Right_sensor_out_of_range_lower_bound,
	Right_sensor_out_of_range_upper_bound,
};

enum struct AQ_states: uint8_t {
	Power_up,
	Normal_operation,
	Sensor_impossibility,
};

enum struct BMS_HV_states: uint8_t {
	AIR_opened,	// normal
	AIR_closed,	// normal
	Precharge,	// normal
	Charger_connected,	// normal
	Unbalanced,	// warning
	Unbalanced_critical,	// shut down
	Voltage_low,	// shut down
	Voltage_high,	// shut down
	Temp_high,	// shut down
	Current_high,	// shut down
};

enum struct BMS_LV_states: uint8_t {
	Normal,
	Charging,	// warning
	Unbalanced,	// warning
	Temp_warning,	// warning
	Voltage_low,	// shut down
	Voltage_high,	// shut down
	Temp_high,	// shut down
	Current_high,	// shut down
	Sleep,	// shut down
};

enum struct Dash_states: uint8_t {
	Power_up,
	Normal_operation,
	Driverless,
};

enum struct Lap_timer_states: uint8_t {
	Power_up,
	Error,
};

enum struct SmartFuseState : uint8_t
{
        Ok,
        ResetState,
        SPIError,
        /*
         * over temperature event +
         * power limitation event +
         * VDS mask
         */
        OTPLVDS,
        TempFail,
        LatchOff,
        /*
         * open load event  +
         * shorted to ground
         */
        OLOFF,
        /*
         * device enters FailSafe on start up or when watch dog is
         * failed to be toggled within every 30 ms to 70 ms
         */
        FailSafe,
        NotResponding,
};

enum struct Steering_Wheel_states: uint8_t {
	OK,
	Power_up,
	Normal_operation,
};

enum struct TS_states: uint8_t {
	NORMAL_OPERATION,
	Power_up,
	APPS_TIMEOUT,
	APPS_INVALID_VALUE,
	APPS_SKIP_FRAME,
	INV_TIMEOUT,
	INV_IPEAK,
};

enum struct Telemetry_states: uint8_t {
	Power_up,
	Gathering_Data,
	Transmitting,
};

enum struct WheelTemp_states: uint8_t {
	Power_up,
	Normal_operation,
	Sensor_impossibility,
};

enum struct YawProbe_states: uint8_t {
	Power_up,
	Normal_operation,
	Sensor_impossibility,
};

struct Device_States {

	Apps_states apps;
	BMS_LV_states lv;
	BMS_HV_states hv;
	AQ_states aq;
	Dash_states dash;
	Lap_timer_states lap_timer;
	SmartFuseState sf;
	Steering_Wheel_states sw;
	TS_states ts;
	Telemetry_states telemetry;
	WheelTemp_states wht;
	YawProbe_states yaw;
};
std::string Return_apps_state(Apps_states state);
std::string Return_lv_state  (BMS_LV_states state);


#endif /* INC_DEVICE_STATES_HPP_ */
