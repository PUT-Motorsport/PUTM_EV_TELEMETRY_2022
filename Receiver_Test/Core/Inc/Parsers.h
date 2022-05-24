/*
 * Parsers.h
 *
 *  Created on: 11 May 2022
 *      Author: wasyl
 */
#include "main.h"


#ifndef INC_PARSERS_H_
#define INC_PARSERS_H_

void Message_65(uint8_t RxData[]);
void Message_69(uint8_t RxData[]);
void Message_66(uint8_t RxData[]);
void Message_68(uint8_t RxData[]);

void Parse(uint8_t *RxData);

#endif /* INC_PARSERS_H_ */
