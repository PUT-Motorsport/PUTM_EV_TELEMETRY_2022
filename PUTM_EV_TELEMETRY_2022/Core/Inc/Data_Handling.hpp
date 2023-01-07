#ifndef INC_DATA_HANDLING_HPP_
#define INC_DATA_HANDLING_HPP_

#include "main.h"
#include "etl/circular_buffer.h"
#include <vector>

enum struct HeartBeat
{
    Buffer1,
    Buffer2,
    Buffer3,
	Buffer4,
    DEFAULT,
    FRAME_BY_FRAME
};

class Data_management
{
private:
    // Buffers containing data and states.
    uint8_t DataBuffer100hz[32] = {0};
    uint8_t DataBufferAq[32] = {0};
    uint8_t DataBuffer50hz[32] = {0};
    uint8_t DataBuffer10hz[32] = {0};
    uint8_t DataBuffer1hz[32] = {0};
    uint8_t DataBufferTimeAcc[32] = {0};
    uint8_t DataBufferTimeSkidpad[32] = {0};
    uint8_t DataBufferTimeSector[32] = {0};
    uint8_t DataBufferTimeLap[32] = {0};

public:
    std::vector<uint8_t*> timer_buffer;
    /* Methods */

    void Init();

    uint8_t* Check_Buffer100hz();
    uint8_t* Check_Buffer_Aq();
    uint8_t* Check_Buffer50hz();
    uint8_t* Check_Buffer10hz();
    uint8_t* Check_Buffer1hz();
    void Check_Buffer_Laptimer();

    void Clear_msg1();
    void Clear_msg2();
    void Clear_msg3();
    void Clear_time();

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
#endif
