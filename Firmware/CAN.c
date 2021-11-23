#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_RxHeaderTypeDef RxHeader_FDCAN1;
extern FDCAN_TxHeaderTypeDef TxHeader_FDCAN1;
extern uint8_t Rxdata[8];
extern uint8_t TxData[8];
extern uint8_t DataToSend[96];
extern time_t t;

__weak void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan1, uint32_t RxFifo0ITs)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfdcan1);
  UNUSED(RxFifo0ITs);
 // HAL_FDCAN_GetProtocolStatus(hfdcan1, &Error);
  if(HAL_FDCAN_GetRxMessage(hfdcan1, FDCAN_RX_FIFO0, &RxHeader_FDCAN1, Rxdata) == HAL_OK)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	  Build_Message(Rxdata);
  }
  else
  {
	  Error_Handler();
  }
}
void Build_Message(uint8_t Rxdata[])
{
	if(RxHeader_FDCAN1.Identifier == 0x2AA)
	{
		//Add this frame to DataToSend buffer
	}
	else if(RxHeader_FDCAN1.Identifier == 0x2BA)
	{
		//Add another frame to DataToSend buffer
	}
	/**
	 *
	 *
	 *
	 *
	 *
	 */

}
void CAN1_TX()//Test Function
{
	TxHeader_FDCAN1.DataLength = FDCAN_DLC_BYTES_8;
	//TxHeader_FDCAN1.Identifier = 682;
	TxHeader_FDCAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_FDCAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_FDCAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_FDCAN1.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader_FDCAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_FDCAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_FDCAN1.MessageMarker = 0;
	uint16_t r = rand() % 2046 + 1;
	TxHeader_FDCAN1.Identifier = r;
	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_FDCAN1, TxData) != HAL_OK)
	{
		Error_Handler();
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
		Error_Handler();
	}
	else
	{


	}


}

