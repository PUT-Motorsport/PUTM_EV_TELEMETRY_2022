#ifndef INC_DATA_HANDLING_HPP_
#define INC_DATA_HANDLING_HPP_

//#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"
#include "main.h"

struct time
{
	uint8_t minute;
	uint8_t seconds;
	uint16_t miliseconds;
};

enum struct HeartBeat
{
    Buffer1,
    Buffer2,
    Buffer3,
    DEFAULT,
    FRAME_BY_FRAME
};

class Data_management
{
private:
    // Buffers containing data and states.
    uint8_t DataBuffer1[32] = {0};
    uint8_t DataBufferAQ[32] = {0};
    uint8_t DataBuffer2[32] = {0};
    uint8_t DataBufferSFy[32] = {0};
    uint8_t DataBufferTemps[32] = {0};

    uint8_t StateBuffer1[10] = {0};
    uint8_t StateBuffer2[10] = {0};
    uint8_t StateBuffer3[10] = {0};

public:
    // flags used to track arriving frames. Each bit represent corresponding
    // frame.
    uint8_t DataBuffer1_flag = 0;
    uint8_t DataBuffer2_flag = 0;
    uint8_t DataBuffer3_flag = 0;

    // Const values indicating full state of a buffer.
    const uint8_t Buffer1_full = 63;
    const uint8_t Buffer2_full = 3;
    const uint8_t Buffer3_full = 255;


    enum struct SAFETY_Front{
    	BSPD,
		Driver_Kill,
		EBS,
		INERTIA,
		Left_Kill,
		Overtravel,
		Right_Kill
    };
    enum struct SAFETY_Rear{
        BSPD,
    	Driver_Kill,
   		EBS,
   		INERTIA,
   		Left_Kill,
   		Overtravel,
		Right_Kill
    };

    bool Missing_Safety_Front;
    bool Missing_Safety_Rear;

    /* Methods */

    void Init();

    uint8_t* Check_Buffer1();
    uint8_t* Check_AQBuffer();
    uint8_t* Check_Buffer2();
    uint8_t* Check_BufferSFy();
    uint8_t* Check_BufferTtemps();

    uint8_t* return_state1_pointer() { return StateBuffer1; }
    uint8_t* return_state2_pointer() { return StateBuffer2; }
    uint8_t* return_state3_pointer() { return StateBuffer3; }

    void Clear_msg1();
    void Clear_msg2();
    void Clear_msg3();

    void Pass_States(uint8_t state);
};

void Send_Frame();
void Cycle_frames();
//CAN.c
void Open_Filter(void);
void Close_Filter(void);
void CAN_Init(void);
bool Check_Laptimer();
void Update_Time();
time return_lap_time();
#endif
