#ifndef INC_DATA_HANDLING_HPP_
#define INC_DATA_HANDLING_HPP_

#include "main.h"

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
    uint8_t DataBuffer100hz[96] = {0};
    uint8_t DataBuffer50hz[96] = {0};
    uint8_t DataBuffer10hz[96] = {0};
    uint8_t DataBuffer1hz[32] = {0};
public:
    /* Methods */

    void Init();

    uint8_t* Check_Buffer100hz();
    uint8_t* Check_Buffer50hz();
    uint8_t* Check_Buffer10hz();
    uint8_t* Check_Buffer1hz();

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
#endif
