/*
 * CAN.h
 *
 *  Created on: Nov 21, 2021
 *      Author: wasyl
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_


__weak void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan1, uint32_t RxFifo0ITs);
void Build_Message(uint8_t Rxdata[]);
void CAN1_TX(uint16_t ID, uint8_t *data);


#endif /* INC_CAN_H_ */
