/*
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef CAN_STUFF
#define CAN_STUFF

void CAN_send_Speeds(float Speed_data[4]);
void CAN_reset_distance(uint8_t distance_to_reset[4]);
void CAN_RX_Inverter_Read_Data();

#endif /* CAN_STUFF */