/*
 * Ultrasonic_Collision_Avoidance.c
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */
#include "Ultrasonic_Collision_Avoidance.h"

// filtering
//float ALPHA_ULTARSONIC = 0.3; // giltering prameter for the Exponential moving average
#define POS_OBSTRUCTED_ANGLE_MIN 50
#define POS_OBSTRUCTED_ANGLE_MAX 210
#define NEG_OBSTRUCTED_ANGLE_MIN -150
#define NEG_OBSTRUCTED_ANGLE_MAX -310
uint8_t obstructed_L = 0, obstructed_R = 0;
float Ultra_m_C = 4, Ultra_m_L = 4, Ultra_m_R = 4;

#define ULRTA_NUM_NO_DATA_INVALID 40
uint8_t Ultrsonic_data_invaildcounter[3] = {ULRTA_NUM_NO_DATA_INVALID, ULRTA_NUM_NO_DATA_INVALID, ULRTA_NUM_NO_DATA_INVALID};


float obstacel_F_x = 0, obstacel_L_x = 0, obstacel_L_y = 0, obstacel_R_x = 0, obstacel_R_y = 0;
uint8_t Obstacel_valid[3] = {0, 0, 0};


// collison avdoidance
#define CA_START 1
#define CA_STOP 0.2
/*
float avg_Speeds = 0;
float trajctory_x = 0, trajctory_y = 0;
float Trajctory[2] = {0, 0};
float trajctory_Angel = 0;
*/

// obstacels
float Obstacel_F = 4, Obstacel_L = 4, Obstacel_R = 4; // y vaule alwasy stays 4



void Ultra_sonic_filter(int16_t Ultrasonic_cm_C_clc, int16_t Ultrasonic_cm_L_clc, int16_t Ultrasonic_cm_R_clc, float Steering_Angles_clc[4], float ALPHA_ULTARSONIC){

	if(Ultrasonic_cm_C_clc != 0){
		Ultrsonic_data_invaildcounter[0] = 0;
		Ultra_m_C = Exp_moving_average((float)Ultrasonic_cm_C_clc / 100 , Ultra_m_C, ALPHA_ULTARSONIC);
	}
	else Ultrsonic_data_invaildcounter[0]++;

	if(Ultrasonic_cm_L_clc != 0){
		obstructed_L = Ultrasoinc_Obstructet_Test(Steering_Angles_clc[0]);
		if(obstructed_L == 0) {
			Ultrsonic_data_invaildcounter[1] = 0;
			Ultra_m_L = Exp_moving_average((float)Ultrasonic_cm_L_clc / 100 , Ultra_m_L, ALPHA_ULTARSONIC);
		}
	}
	else Ultrsonic_data_invaildcounter[1]++;

	if(Ultrasonic_cm_R_clc != 0){
		obstructed_R = Ultrasoinc_Obstructet_Test( - Steering_Angles_clc[1]); // neagtiv becasue right wheel
		if(obstructed_R == 0){
			Ultrsonic_data_invaildcounter[2] = 0;
			Ultra_m_R = Exp_moving_average((float)Ultrasonic_cm_R_clc / 100 , Ultra_m_R, ALPHA_ULTARSONIC);
		}
	}
	else Ultrsonic_data_invaildcounter[2]++;

	uint8_t i;
	for(i = 0; i < 3; i++){
		if( Ultrsonic_data_invaildcounter[i] > ULRTA_NUM_NO_DATA_INVALID) Ultrsonic_data_invaildcounter[i] = ULRTA_NUM_NO_DATA_INVALID;
	}

	// front obstacel
	if(Ultrsonic_data_invaildcounter[0] < ULRTA_NUM_NO_DATA_INVALID ){
	//	Obstacel_F[0] = Ultra_m_C;
		Obstacel_F = Ultra_m_C;
		Obstacel_valid[0] = 1;
	}
	else {
		Obstacel_valid[0] = 0;
		//Obstacel_F[0]  = 4;
	}

	if(Ultrsonic_data_invaildcounter[1] < ULRTA_NUM_NO_DATA_INVALID && obstructed_L == 0 ){
		//Obstacel_L[0] = Ultra_m_L * cos(Steering_Angles[0] / RAD_TO_DEG);
		//Obstacel_L[1] = Ultra_m_L * sin(Steering_Angles[0] / RAD_TO_DEG);
		Obstacel_L = Ultra_m_L;
		Obstacel_valid[1] = 1;
	}
	else {
		Obstacel_valid[1] = 0;
		//Obstacel_L[0]  = 4;
		//Obstacel_L[1]  = 4;
	}

	if(Ultrsonic_data_invaildcounter[2]  < ULRTA_NUM_NO_DATA_INVALID && obstructed_R == 0 ){
		//Obstacel_R[0] = Ultra_m_R * cos(Steering_Angles[1] / RAD_TO_DEG);
		//Obstacel_R[1] = Ultra_m_R * sin(Steering_Angles[1] / RAD_TO_DEG);
		Obstacel_valid[2] = 1;
		Obstacel_R = Ultra_m_R;
	}
	else {
		Obstacel_valid[2] = 0;
		//Obstacel_R[0]  = 4;
		//Obstacel_R[1]  = 4;
	}

	/*
	obstacel_F_x = Obstacel_F[0];
	obstacel_L_x = Obstacel_L[0];
	obstacel_L_y = Obstacel_L[1];
	obstacel_R_x = Obstacel_R[0];
	obstacel_R_y = Obstacel_R[1];
	*/
}

uint8_t Ultrasoinc_Obstructet_Test(float Angle){
	uint8_t i;
	for(i = 0; i < 3; i++){ // if rotate around multipel times
		if(Angle > 0){
			if((Angle - (POS_OBSTRUCTED_ANGLE_MIN + (i * 360))) * (Angle - (POS_OBSTRUCTED_ANGLE_MAX + (i * 360))) <= 0){
				return 1;
			}
		}

		if(Angle < 0){
			if((Angle - (NEG_OBSTRUCTED_ANGLE_MIN - (i * 360))) * (Angle - (NEG_OBSTRUCTED_ANGLE_MAX - (i * 360))) <= 0){
						return 1;
			}
		}
	}

	return 0;
}


/*
void Collision_voidance(){
	DIGITAL_IO_SetOutputLow(&LED_COLLISION_AVOID);
	CAN_Ultrasonic_No_messasge_count ++;
	if(CAN_Ultrasonic_No_messasge_count > 200) {
		CAN_Ultrasonic_No_messasge_count = 200;
		return;
	}

	float Speed_reduction_ratio = 1;
	float Max_Speed = 0;

	float trajctory_dist_O_L = 4;

	float Obstacel_Angel_L = Steering_Angles[0];
	float Obstacel_Angel_R = Steering_Angles[1];
	float CA_Angel = 15;


	if(Steering_mode == CRAB){
		// Front sensor
		if(Obstacel_valid[0] == 1){
			if(Obstacel_F < CA_START){
				if(trajctory_x > 0.01){ // // check that we are goig in dirction of obstacel and at a no practical 0 speed
					if(Obstacel_F < CA_STOP) Speed_reduction_ratio = 0; // stop
					else if(trajctory_x > Obstacel_F - CA_STOP)	Max_Speed = Obstacel_F - CA_STOP; // speed reduction
				}
			}
		}



		/// left
		if(Obstacel_valid[1] == 1){
			if(Obstacel_L < CA_START){
				if((trajctory_Angel - Obstacel_Angel_L - CA_Angel) * (trajctory_Angel - Obstacel_Angel_L + CA_Angel)){
					float abs_trajctory = vector_abs_value(Trajctory);
					if( abs_trajctory > 0.01){
						if(Obstacel_L < CA_STOP) Speed_reduction_ratio = 0; // stop
						else if(abs_trajctory > Obstacel_L - CA_STOP)	Max_Speed = Obstacel_L - CA_STOP; // speed reduction
					}
				}
			}
		}




	}

	if(Speed_reduction_ratio != 1)DIGITAL_IO_SetOutputHigh(&LED_COLLISION_AVOID);
	else if(Max_Speed != 0){
		Speed_reduction_ratio = Max_Speed / Speeds[0];
		DIGITAL_IO_SetOutputHigh(&LED_COLLISION_AVOID);
	}



	uint8_t i;
	for(i = 0; i < 4; i ++){
		Speeds[i] = Speeds[i] * Speed_reduction_ratio;
	}
}
*/
