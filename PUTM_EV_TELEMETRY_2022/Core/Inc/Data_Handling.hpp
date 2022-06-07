#ifndef INC_DATA_HANDLING_HPP_
#define INC_DATA_HANDLING_HPP_

#include "main.h"
#include "stdio.h"
#include <cstring>
#include <stdlib.h>

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
};

void Send_Frame();

void Cycle_frames();

#endif
