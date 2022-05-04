#include "Data_handling.hpp"

/*list all devices*/
APPS apps1;
BMS_HV bmshv1;
BMS_LV bmslv1;
Traction_Control tc1;




AQ_Card aq1;

/* Initialize Parsing vector */
Parsing_Vector devs = {apps1.apps_main,
					   bmshv1.bms_hv_main,
					   bmslv1.bms_lv_main,
					   bmslv1.bms_lv_temps,
					   tc1.dash_tcs_frame,
					   tc1.tc_main,
					   tc1.tc_rear_suspension,
					   aq1.aq_card_main,
};

extern Data_management handler1;

/**
* @brief Parsing vector initialization.
* Data_management class method. Used to put pointer objects into a parsing vector. Objects inside parsing vector are sorted by ID.
* @param: none.
* @retval none.
*/
void Data_management::Init()
{

}
/**
* @brief Function matching incoming frame with parser.
* Data_management class method. Used to match newly arrived CAN frame with corresponding parsing function using a binary search algorithm.
* @param: Received CAN frame.
* @retval none.
*/
void Data_management::Choose_Parser(Can_Message msg1)
{
	uint16_t ID = msg1.return_ID();
	int l = 0;
	int p = 8-1;
	int i = 0;
	while(l <= p)
	{
		i = floor((l+p)/2);
		if(devs.at(i).ID < ID)
		{
			l = i+1;
		}
		else if(devs.at(i).ID > ID)
		{
			p = i-1;
		}
		else
		{
			devs.at(i).PTR(msg1);
			break;
		}
	}
}
/**
* @brief Prepare data in DataBuffer1.
* Data_management class method. Used to put data into a 32-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to DataBuffer1.
*/
uint8_t * Data_management::Prepare_DataBuffer1()
{
	handler1.DataBuffer1[0] = 'A';
	handler1.DataBuffer1[1] = apps1.retrun_Pedal_Position();
	for(int i=2;i<32;i++)
	{
		handler1.DataBuffer1[i] = rand()%255;
	}
	/*
	 *
	 *
	 *
	 *
	 *
	 */
	//dodać DataBuffer1_flag
	return DataBuffer1;
}
/**
* @brief Prepare data in DataBuffer2.
* Data_management class method. Used to put data into a 32-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to DataBuffer2.
*/
uint8_t * Data_management::Prepare_DataBuffer2()
{
	handler1.DataBuffer2[0] = 'B';
	for(int i=2;i<32;i++)
	{
		handler1.DataBuffer2[i] = rand()%255;
	}
	/*
	 *
	 *
	 *
	 *
	 *
	 */
	//dodać DataBuffer2_flag

	return DataBuffer2;
}
/**
* @brief Prepare data in DataBuffer3.
* Data_management class method. Used to put data into a 32-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to DataBuffer3.
*/
uint8_t * Data_management::Prepare_DataBuffer3()
{
	handler1.DataBuffer3[0] = 'C';
	for(int i=2;i<32;i++)
	{
		handler1.DataBuffer3[i] = rand()%255;
	}
	/*
	 *
	 *
	 *
	 *
	 *
	 */
	//dodać DataBuffer3_flag

	return DataBuffer3;
}
/**
* @brief Prepare states in StateBuffer1.
* Data_management class method. Used to put states into a ??-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to StateBuffer1.
*/
uint8_t * Data_management::Prepare_StateBuffer1()
{
	handler1.StateBuffer1[0] = 'D';
	handler1.StateBuffer1[1] = apps1.return_state();
	for(int i=2;i<32;i++)
		{
			handler1.StateBuffer1[i] = rand()% 5+1;
		}
	return StateBuffer1;
}
/**
* @brief Prepare states in StateBuffer1.
* Data_management class method. Used to put states into a ??-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to StateBuffer1.
*/
uint8_t * Data_management::Prepare_StateBuffer2()
{
	handler1.StateBuffer2[0] = 'E';
	for(int i=2;i<32;i++)
		{
			handler1.StateBuffer2[i] = rand()% 5+1;
		}
	return StateBuffer2;
}
/**
* @brief Prepare states in StateBuffer1.
* Data_management class method. Used to put states into a ??-byte long buffer in order to be sent to radio module.
* @param: none.
* @retval Pointer to StateBuffer1.
*/
uint8_t * Data_management::Prepare_StateBuffer3()
{
	handler1.StateBuffer3[0] = 'F';
		for(int i=2;i<32;i++)
			{
				handler1.StateBuffer3[i] = rand()% 5+1;
			}
	return StateBuffer3;
}




void APPS_Main_Parser(Can_Message msg1)
{
	handler1.Update_DataBuffer1_flag(0x01);
	apps1.Pedal_Position = msg1.return_Data(1);
	apps1.state = msg1.return_Data(2);
}

void BMS_LV_Main_Parser(Can_Message msg1)
{

}

void BMS_LV_Temps_Parser(Can_Message msg1)
{

}

void Dash_Tcs_Frame_Parser(Can_Message msg1)
{

}
void TC_Main_Parser(Can_Message msg1)
{

}
void TC_Rear_Suspension(Can_Message msg1)
{

}
void AQ_Card_Main_Parser(Can_Message msg1)
{

}
void BMS_HV_Main_Parser(Can_Message msg1)
{

}

