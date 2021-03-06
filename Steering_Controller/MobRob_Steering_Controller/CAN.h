/*
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef CAN_STUFF
#define CAN_STUFF

#include "DAVE.h"
#include "Globals.h"
#include "Ultrasonic_Collision_Avoidance.h"


extern volatile uint8_t CAN_Ultrasonic_No_messasge_count;
extern volatile uint8_t CAN_state_Ultrasonic;

void CAN_send_Speeds(float Speed_data[4]);
void CAN_reset_distance(uint8_t distance_to_reset[4]);
void CAN_RX_Inverter_Read_Data();

#endif /* CAN_STUFF */
