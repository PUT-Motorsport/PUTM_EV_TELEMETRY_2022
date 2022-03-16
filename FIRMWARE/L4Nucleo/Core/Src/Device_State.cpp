/*
 * Device_State.cpp
 *
 *  Created on: 16 mar 2022
 *      Author: Adam Wasilewski
 */

#include "Device_state.hpp"

enum struct Telemetry_States : uint8_t {
    ok,
    max_retransmissions, // Warning
    Time_IRQ,			 // Warning
    CAN_Rx_Fifo_Full,    // Warning?
    Error_Handler,		 // Error
};


