/*
 * CAN.c

 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */
#include "CAN.h"


float speed_fl_act = 0, speed_fr_act = 0, speed_rl_act = 0, speed_rr_act = 0; // in m/s
float distance_fl_act = 0, distance_fr_act = 0;
float distance_rl_act = 0;
float distance_rr_act = 0; // in m/s

volatile uint8_t Inveter_CAN_OK[4] = {0};
volatile uint8_t CAN_RX_Inverter_count = 0;


volatile uint8_t CAN_Ultrasonic_No_messasge_count = 100;
volatile uint8_t CAN_state_Ultrasonic = 0;
volatile uint16_t Ultrasonic_cm_C = 0;
volatile uint16_t Ultrasonic_cm_L = 0;
volatile uint16_t Ultrasonic_cm_R = 0;

void CAN_reset_distance(uint8_t distance_to_reset[4]){
	uint8_t TX[8] ={0};
	uint8_t i;
	for(i = 0; i < 4; i++){	TX[i] = distance_to_reset[i];}

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[1], (uint8_t*)TX);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[1]); //Transmit the data of message object1
}

void CAN_send_Speeds(float Speed_data[4]){

	int16_t Speeds_int16[4] ={Speed_data[0] *1000, Speed_data[1] *1000, Speed_data[2] *1000, Speed_data[3] *1000}; // in int16_t mm/s converter back on inverter

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[0], (uint8_t*)Speeds_int16);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[0]); //Transmit the data of message object1

}

void CAN_RX_INVERTER_ISR(void) {
	WATCHDOG_Service();
	DIGITAL_IO_SetOutputLow(&WATCHDOG_LED_BLUE);

	CAN_RX_Inverter_count++;
	if(CAN_RX_Inverter_count == 4) CAN_RX_Inverter_Read_Data(); // read data after all for have reviced asseumes all 4 are working
}

void CAN_RX_Inverter_Read_Data(){
	uint8_t Motor_on_rover;
	for(Motor_on_rover = 0; Motor_on_rover < 4; Motor_on_rover ++){
		XMC_CAN_MO_t* lmsgobjct_ptr_1 = CAN_NODE_0.lmobj_ptr[Motor_on_rover + 2]->mo_ptr;
		CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[Motor_on_rover + 2]);

		uint8_t data_RX[8];
		uint8_t i;
		for(i = 0 ; i < 8 ; i ++){
			 data_RX[i] = lmsgobjct_ptr_1->can_data_byte[i];
		}

		uint8_t Motor_on_rover_RX = (data_RX[0 * 2 + 1] << 8) | data_RX[0 * 2];
		if(Motor_on_rover_RX == Motor_on_rover){
			DIGITAL_IO_SetOutputLow(&LED_CAN_ERROR);

			uint8_t i;
			int16_t values_int16[3] = {0};
			for(i = 1 ; i < 4 ; i ++){
				 if(i == 1) values_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
				 else if(i == 2) values_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
				 else if(i == 3) values_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
			 }

			Inveter_CAN_OK[Motor_on_rover] = values_int16[0];
			Actual_Speeds[Motor_on_rover] = (float)values_int16[1] / 1000;
			Actual_Distance[Motor_on_rover] = (float)values_int16[2] / 1000;
			}
		else DIGITAL_IO_SetOutputHigh(&LED_CAN_ERROR);
	}

	speed_fl_act = Actual_Speeds[0];
	speed_fr_act = Actual_Speeds[1];
	speed_rl_act = Actual_Speeds[2];
	speed_rr_act = Actual_Speeds[3];
}


void CAN_RX_ULTRASONIC_ISR(void) { // recide data
	XMC_CAN_MO_t* lmsgobjct_ptr_1 = CAN_NODE_0.lmobj_ptr[6]->mo_ptr;
	CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[6]);

	uint8_t data_RX[8];
	uint8_t i;
	for(i = 0 ; i < 8 ; i ++){
		 data_RX[i] = lmsgobjct_ptr_1->can_data_byte[i];
	}

	CAN_state_Ultrasonic = (data_RX[0 * 2 + 1] << 8) | data_RX[0 * 2];
	Ultrasonic_cm_C = (data_RX[1 * 2 + 1] << 8) | data_RX[1 * 2];
	Ultrasonic_cm_L = (data_RX[2 * 2 + 1] << 8) | data_RX[2 * 2];
	Ultrasonic_cm_R = (data_RX[3 * 2 + 1] << 8) | data_RX[3 * 2];

	Ultra_sonic_filter(Ultrasonic_cm_C,  Ultrasonic_cm_L,  Ultrasonic_cm_R,   ALPHA_ULTARSONIC);

	CAN_Ultrasonic_No_messasge_count = 0;

	WATCHDOG_Service();
	DIGITAL_IO_SetOutputLow(&WATCHDOG_LED_BLUE);
}

