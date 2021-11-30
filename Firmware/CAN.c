#include "stm32g4xx_hal.h"
#include "NRF24L01.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef hlpuart1;
extern FDCAN_RxHeaderTypeDef RxHeader_FDCAN1;
extern FDCAN_TxHeaderTypeDef TxHeader_FDCAN1;

extern uint8_t Rxdata[8];
extern uint8_t TxData[8];

extern uint8_t DataBuffer1[32];
extern uint8_t DataBuffer2[32];
extern uint8_t DataBuffer3[32];

extern uint8_t MsReady;

uint16_t flag_buffer = 0;
/**
  * @brief  Callback function called when new data frames arrives from CAN bus.
  *
  * @param  Pointer to FDCAN handler.
  *         
  * @param  Wich RxFifo should Trigger a callback.
  * @retval None
  */
__weak void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan1, uint32_t RxFifo0ITs)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfdcan1);
  UNUSED(RxFifo0ITs);
 // HAL_FDCAN_GetProtocolStatus(hfdcan1, &Error);
  if(HAL_FDCAN_GetRxMessage(hfdcan1, FDCAN_RX_FIFO0, &RxHeader_FDCAN1, Rxdata) == HAL_OK)
  {
	  BuildMessageAll(Rxdata);
  }
  else
  {
	  //Error_Handler();
  }
}
/**
  * @brief  This function puts a newly arrived data to corresponding place in a data packet.
  *	    It is triggered by CAN Callback.
  * @param  Data aquired from CAN bus.
  *         
  * @retval None
  */
void BuildMessageAll(uint8_t Rxdata[])
{
///////////////////////////////////DATA BUFFER 1//////////////////////////////////////////
	if(RxHeader_FDCAN1.Identifier == 0x0A)// APPS[2]
		{
			flag_buffer = flag_buffer | 0x01;
			for(int i=1;i<3;i++)
			{
				DataBuffer1[i] = Rxdata[i];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0B)//Prędkość obrotowa silnika[2] / Prąd RMS[2] / Status[2] / Temperatura1[1] / Temperatura2[1]
		{
			flag_buffer = flag_buffer | 0x02;
			for(int i=3;i<11;i++)
			{
				DataBuffer1[i] = Rxdata[i-3];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0C)//Kod błędu[1] / State od charge[1] / napięcie 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x04;
			for(int i=11;i<19;i++)
			{
				DataBuffer1[i] = Rxdata[i-11];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0D)//Ready to Drive[1] / Prędkość koła lewego[2] / prędkość koła prawego[2] / ciśnienie hamulców1[2] / tem. mono[1]
		{
			flag_buffer = flag_buffer | 0x08;
			for(int i=19;i<27;i++)
			{
				DataBuffer1[i] = Rxdata[i-19];
			}
		}
///////////////////////////////////DATA BUFFER 2//////////////////////////////////////////
	else if(RxHeader_FDCAN1.Identifier == 0x0E)//HV prąd[2] / HV napięcie[2] / Temp max[1] / Poziom naładowania[1] / Status[1] / Kody błędów[1]
		{
			flag_buffer = flag_buffer | 0x10;
			for(int i=1;i<9;i++)
			{
				DataBuffer2[i] = Rxdata[i];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x60)//Prędkość obrotwa koła prawego[2] / lewego[2] / Kąt ugięcia dampera prawego tył[1] / lewego[1] / temp. wody za pompą[1] / za chłodnicą[1]
		{
			flag_buffer = flag_buffer | 0x20;
			for(int i=8;i<17;i++)
			{
				DataBuffer2[i] = Rxdata[i-8];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x5F)// avg.temp[1] / tem 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x40;
			for(int i=16;i<24;i++)
			{
				DataBuffer2[i] = Rxdata[i-16];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x5B)//GyroX[2] / GyroY[2] / GyroZ[2]
		{
			flag_buffer = flag_buffer | 0x80;
			for(int i=23;i<30;i++)
			{
				DataBuffer2[i] = Rxdata[i-22];
			}
		}
///////////////////////////////////DATA BUFFER 3//////////////////////////////////////////
	else if(RxHeader_FDCAN1.Identifier == 0x5A)// AccX[2] / AccY[2] / AccZ[2]
		{
			flag_buffer = flag_buffer | 0x100;
			for(int i=1;i<7;i++)
			{
				DataBuffer3[i] = Rxdata[i];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0F)//Kąt ugięcia dampera prawego przód[2] / lewego[2] / ciśnienie hamulców2[2]
		{
			flag_buffer = flag_buffer | 0x200;
			for(int i=7;i<13;i++)
			{
				DataBuffer3[i] = Rxdata[i-7];
			}
		}
		//SEND
		if(flag_buffer == 1023)
		{
			MsReady = 0xff;
			flag_buffer = 0;
			DataBuffer1[0] = 'a';
			DataBuffer2[0] = 'b';
			DataBuffer3[0] = 'c';
		}
}
/**
  * @brief  Function to send data to a CAN bus
  *
  * @param  Frame ID
  *
  * @param Pointer to a data buffer.
  *
  * @retval None
  */
void CAN1_TX(uint16_t ID, uint8_t *data)//Test Function
{
	TxHeader_FDCAN1.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader_FDCAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_FDCAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_FDCAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_FDCAN1.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader_FDCAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_FDCAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_FDCAN1.MessageMarker = 0;
	TxHeader_FDCAN1.Identifier = ID;
	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_FDCAN1, data) != HAL_OK)
	{
		//Error_Handler();
	}
	else
	{


	}
}


