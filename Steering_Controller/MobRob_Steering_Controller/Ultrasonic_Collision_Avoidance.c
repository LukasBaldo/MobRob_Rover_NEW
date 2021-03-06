/*
 * Ultrasonic_Collision_Avoidance.c
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */
#include "Ultrasonic_Collision_Avoidance.h"
#include<stdio.h>

// filtering
//float ALPHA_ULTARSONIC = 0.3; // giltering prameter for the Exponential moving average
#define POS_OBSTRUCTED_ANGLE_MIN 50
#define POS_OBSTRUCTED_ANGLE_MAX 210
#define NEG_OBSTRUCTED_ANGLE_MIN -150
#define NEG_OBSTRUCTED_ANGLE_MAX -310
uint8_t obstructed_L = 0, obstructed_R = 0;

float Ultra_m_C = 4, Ultra_m_L = 4, Ultra_m_R = 4;


// collison avdoidance
#define ULRTA_NUM_NO_DATA_INVALID 20
uint8_t Ultrsonic_data_invaildcounter[3] = {ULRTA_NUM_NO_DATA_INVALID, ULRTA_NUM_NO_DATA_INVALID, ULRTA_NUM_NO_DATA_INVALID};

uint8_t Obstacel_valid[3] = {0, 0, 0};

#define CA_START 1
#define CA_STOP 0.2
#define CA_ANGLE 15

Obstacel Obstacel_F;
Obstacel Obstacel_L;
Obstacel Obstacel_R;

float Speed_reduction_ratio = 1;
float Speed_limit = MAX_Speed;
float new_Speed_limit = MAX_Speed;
float aktive_addition = 0;

float speed_CA_fl = 0;
float speed_CA_fr = 0;
float speed_CA_rl = 0;
float speed_CA_rr = 0;

uint8_t init = 0;

void Ultra_sonic_filter(int16_t Ultrasonic_cm_C_clc, int16_t Ultrasonic_cm_L_clc, int16_t Ultrasonic_cm_R_clc, float ALPHA_ULTARSONIC){

	if(Ultrasonic_cm_C_clc != 0){
		Ultrsonic_data_invaildcounter[0] = 0;
		Ultra_m_C = Exp_moving_average((float)Ultrasonic_cm_C_clc / 100 , Ultra_m_C, ALPHA_ULTARSONIC);
	}
	else Ultrsonic_data_invaildcounter[0]++;

	if(Ultrasonic_cm_L_clc != 0){
		obstructed_L = Ultrasoinc_Obstructet_Test(-Steering_Angles[0]);
		if(obstructed_L == 0) {
			Ultrsonic_data_invaildcounter[1] = 0;
			Ultra_m_L = Exp_moving_average((float)Ultrasonic_cm_L_clc / 100 , Ultra_m_L, ALPHA_ULTARSONIC);
		}
	}
	else Ultrsonic_data_invaildcounter[1]++;

	if(Ultrasonic_cm_R_clc != 0){
		obstructed_R = Ultrasoinc_Obstructet_Test(Steering_Angles[1]); // neagtiv becasue right wheel
		if(obstructed_R == 0){
			Ultrsonic_data_invaildcounter[2] = 0;
			Ultra_m_R = Exp_moving_average((float)Ultrasonic_cm_R_clc / 100 , Ultra_m_R, ALPHA_ULTARSONIC);
		}
	}
	else Ultrsonic_data_invaildcounter[2]++;

	for(uint8_t i = 0; i < 3; i++){ // counter overflow stop
		if( Ultrsonic_data_invaildcounter[i] > ULRTA_NUM_NO_DATA_INVALID) Ultrsonic_data_invaildcounter[i] = ULRTA_NUM_NO_DATA_INVALID;
	}

	if(Ultrsonic_data_invaildcounter[0] < ULRTA_NUM_NO_DATA_INVALID ){
		Obstacel_F.angle = 0;
		Obstacel_F.distance = Ultra_m_C;
		Obstacel_F.isValid = 1;
		Obstacel_F.vector[0] = Obstacel_F.distance;
		Obstacel_F.vector[1] = 0;
	}
	else Obstacel_F.isValid = 0;

	if(Ultrsonic_data_invaildcounter[1] < ULRTA_NUM_NO_DATA_INVALID ){
		Obstacel_L.angle = Steering_Angles[0];
		Obstacel_L.distance = Ultra_m_L;
		Obstacel_L.isValid = 1;
		Obstacel_L.vector[0] = Obstacel_L.distance * cos(Obstacel_L.angle / RAD_TO_DEG);
		Obstacel_L.vector[1] = Obstacel_L.distance * sin(Obstacel_L.angle / RAD_TO_DEG);

	}
	else Obstacel_L.isValid = 0;

	if(Ultrsonic_data_invaildcounter[2] < ULRTA_NUM_NO_DATA_INVALID ){
		Obstacel_R.angle = Steering_Angles[1];
		Obstacel_R.distance = Ultra_m_R;
		Obstacel_R.isValid = 1;
		Obstacel_R.vector[0] = Obstacel_R.distance * cos(Obstacel_R.angle / RAD_TO_DEG);
		Obstacel_R.vector[1] = Obstacel_R.distance * sin(Obstacel_R.angle / RAD_TO_DEG);
	}
	else Obstacel_R.isValid = 0;
}


/*
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
*/


uint8_t Ultrasoinc_Obstructet_Test(float Angle){
		if(Angle > 0){
			if(within_MAX_MIN(Angle, POS_OBSTRUCTED_ANGLE_MAX , POS_OBSTRUCTED_ANGLE_MIN)){
				return 1;
			}
		}

		if(Angle < 0){
			if(within_MAX_MIN(Angle, NEG_OBSTRUCTED_ANGLE_MIN , NEG_OBSTRUCTED_ANGLE_MAX)){
				return 1;
			}
		}

	return 0;
}

void Collision_voidance(){
	if(init == 0){
		Obstacel_F.Sensor = FRONT_;
		Obstacel_L.Sensor = LEFT;
		Obstacel_R.Sensor = RIGHT;
		init = 1;
	}



	// check if reciveing from Ulatrasoic sesnors node
	CAN_Ultrasonic_No_messasge_count ++;
	if(CAN_Ultrasonic_No_messasge_count > 200) {
		CAN_Ultrasonic_No_messasge_count = 200;
		return;
	}

	Speed_reduction_ratio = 1;
	Speed_limit = MAX_Speed + 0.01; // + 0.01 to avoid float bs
	new_Speed_limit = MAX_Speed;


	Obstacel_CA_check(Obstacel_F);
	//Obstacel_CA_check(Obstacel_L);
	//Obstacel_CA_check(Obstacel_R);#

	//CA aktive
	if(Speed_reduction_ratio == 0) aktive_addition = 0.05 ;
	else if(aktive_addition > 0) aktive_addition --;

	if(Speed_limit < MAX_Speed){
		if(Speeds[0] != 0) Speed_reduction_ratio = Speed_limit / Speeds[0];
	}

	// speed output with reduction if CA active
	for(uint8_t i = 0; i < 4; i ++){
		Speeds_CA[i] = Speeds[i] * Speed_reduction_ratio;
	}

	speed_CA_fl = Speeds_CA[0];
	speed_CA_fr = Speeds_CA[1];
	speed_CA_rl = Speeds_CA[2];
	speed_CA_rr = Speeds_CA[3];

}

void Obstacel_CA_check(Obstacel obstacel){
	set_CA_LED(obstacel.Sensor,0);
	if(obstacel.isValid == 0) return;

	if(obstacel.distance > CA_START) return; // not far away no CA

	float Trajctory_at_obstacel_abs = vector_projection_abs(Trajctory, obstacel.vector);
	if(Trajctory_at_obstacel_abs < 0.05) return; // if trajcetor in diection of obstacel is smaller then no cA

	if(obstacel.distance  < CA_STOP + aktive_addition){
						Speed_reduction_ratio = 0; // stop
						set_CA_LED(obstacel.Sensor,1);
						return;
	}
	else if(Trajctory_at_obstacel_abs > obstacel.distance  - CA_STOP){
		   new_Speed_limit = obstacel.distance  - CA_STOP; // speed reduction
		if( Speed_limit > new_Speed_limit) {
			Speed_limit = new_Speed_limit;
			set_CA_LED(obstacel.Sensor,2); //led on
			return;
		}
	}
}

uint8_t toggel_ervy_x = 0;

void set_CA_LED(uint8_t Sensor, uint8_t mode){
	if(Sensor == 0)
		{
		if(mode == 0)DIGITAL_IO_SetOutputLow(&LED_CA_FRONT);
		else if(mode == 1)DIGITAL_IO_SetOutputHigh(&LED_CA_FRONT);
		else if(mode == 2 && toggel_ervy_x == 0) DIGITAL_IO_ToggleOutput(&LED_CA_FRONT);
		}
	else if (Sensor == 1){
		if(mode == 0)DIGITAL_IO_SetOutputLow(&LED_CA_LEFT);
		else if(mode == 1)DIGITAL_IO_SetOutputHigh(&LED_CA_LEFT);
		else if(mode == 2 && toggel_ervy_x == 0) DIGITAL_IO_ToggleOutput(&LED_CA_LEFT);
	}
	else if (Sensor == 2){
		if(mode == 0)DIGITAL_IO_SetOutputLow(&LED_CA_RIGHT);
		else if(mode == 1)DIGITAL_IO_SetOutputHigh(&LED_CA_RIGHT);
		else if(mode == 2 && toggel_ervy_x == 0) DIGITAL_IO_ToggleOutput(&LED_CA_RIGHT);
	}

	toggel_ervy_x++;
	if(toggel_ervy_x > 8) toggel_ervy_x = 0;
}
