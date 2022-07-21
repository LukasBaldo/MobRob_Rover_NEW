/*
 * RC_data_recive.h
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef RC_DATA_RECIVE_H_
#define RC_DATA_RECIVE_H_

#include "Globals.h"
#include "DAVE.h"
#include "other_functions.h"

void RC_Recive(void);


#define RC_SPEED_SAFTY_FT 10

volatile extern uint8_t RC_Speed_Stop_counter;

#endif /* RC_DATA_RECIVE_H_ */
