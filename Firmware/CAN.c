#include "stm32g4xx_hal.h"
#include "NRF24L01.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef hlpuart1;
extern FDCAN_RxHeaderTypeDef RxHeader_FDCAN1;
extern FDCAN_TxHeaderTypeDef TxHeader_FDCAN1;

extern uint8_t Rxdata[8];
extern uint8_t TxData[8];
extern uint8_t DataToSend[63];

extern uint8_t MsReady;

uint16_t flag_buffer = 0;

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

void BuildMessageAll(uint8_t Rxdata[])
{
	if(RxHeader_FDCAN1.Identifier == 0x0A)// APPS
		{
			flag_buffer = flag_buffer | 0x01;
			for(int i=0;i<2;i++)
			{
				DataToSend[i] = Rxdata[i];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0B)//Prędkość obrotowa silnika[2] / Prąd RMS[2] / Status[2] / Temperatura1[1] / Temperatura2[1]
		{
			flag_buffer = flag_buffer | 0x02;
			for(int i=2;i<10;i++)
			{
				DataToSend[i] = Rxdata[i-2];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0C)//Kod błędu[1] / State od charge[1] / napięcie 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x04;
			for(int i=10;i<18;i++)
			{
				DataToSend[i] = Rxdata[i-10];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0D)//Ready to Drive[1] / Prędkość koła lewego[2] / prędkość koła prawego[2] / ciśnienie hamulców1[2] / tem. mono[1]
		{
			flag_buffer = flag_buffer | 0x08;
			for(int i=18;i<26;i++)
			{
				DataToSend[i] = Rxdata[i-18];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0E)//HV prąd[2] / HV napięcie[2] / Temp max[1] / Poziom naładowania[1] / Status[1] / Kody błędów[1]
		{
			flag_buffer = flag_buffer | 0x10;
			for(int i=26;i<34;i++)
			{
				DataToSend[i] = Rxdata[i-26];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x0F)//Kąt ugięcia dampera prawego przód[1] / lewego[1] / ciśnienie hamulców2[2]
		{
			flag_buffer = flag_buffer | 0x20;
			for(int i=34;i<42;i++)
			{
				DataToSend[i] = Rxdata[i-34];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x60)//Prędkość obrotwa koła prawego[2] / lewego[2] / Kąt ugięcia dampera prawego tył[1] / lewego[1] / temp. wody za pompą[1] / za chłodnicą[1]
		{
			flag_buffer = flag_buffer | 0x40;
			for(int i=42;i<50;i++)
			{
				DataToSend[i] = Rxdata[i-42];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x5F)// avg.temp[1] / tem 6 celli LV[6]
		{
			flag_buffer = flag_buffer | 0x80;
			for(int i=50;i<57;i++)
			{
				DataToSend[i] = Rxdata[i-50];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x5B)//GyroX[2] / GyroY[2] / GyroZ[2]
		{
			flag_buffer = flag_buffer | 0x100;
			for(int i=57;i<63;i++)
			{
				DataToSend[i] = Rxdata[i-57];
			}
		}
	else if(RxHeader_FDCAN1.Identifier == 0x5A)// AccX[2] / AccY[2] / AccZ[2]
		{
			flag_buffer = flag_buffer | 0x200;
			for(int i=63;i<69;i++)
			{
				DataToSend[i] = Rxdata[i-63];
			}
		}
		//SEND
		if(flag_buffer >= 30)
		{
			MsReady = 0xff;
			flag_buffer = 0;
		}
}
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
void Send_Frame(uint8_t ID, uint8_t *data[])
{
	TxHeader_FDCAN1.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader_FDCAN1.Identifier = ID;
	TxHeader_FDCAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_FDCAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_FDCAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_FDCAN1.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader_FDCAN1.FDFormat = FDCAN_CLASSIC_CAN;\
	TxHeader_FDCAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_FDCAN1.MessageMarker = 0;
	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_FDCAN1, &data) != HAL_OK)
	{
		//Error_Handler();
	}
	else
	{


	}


}

