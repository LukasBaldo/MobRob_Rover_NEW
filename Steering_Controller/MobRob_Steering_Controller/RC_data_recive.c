/*
 * RC_data_recive.c
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */
#include "RC_data_recive.h"


uint8_t RC_Signal_OK = 0, RC_Signal_not_OK_counter = 0; // 0 no ok 1 OK
uint32_t  captured_time_Steering=0, captured_time_Speed=0;
uint8_t RC_no_Speed_data_counter = 0, RC_no_Steering_data_counter = 0;
float RC_Speed = 0, RC_Steering = 0;
#define RC_NP_SPEED  1500000
#define RC_NP_STEERING  1500000
//uint32_t RC_NP_STEERING = 1500000;
float RC_Speed_ns_to_mps = 0.000002; // increased
float RC_Steering_ns_to_deg = 0.00021;

volatile uint32_t RC_gear_100us_counter = 0, RC_AUX1_100us_counter = 0;
volatile uint8_t RC_Gear_state = 2, RC_AUX1_state = 2;
volatile uint8_t RC_Gear_duty = 2, RC_Gear_duty_OLD =2, RC_AUX1_duty = 2;
//uint8_t RC_Speed_Stop = 1;

volatile uint8_t RC_Speed_Stop_counter = RC_SPEED_SAFTY_FT;

void INTERRUPT_TIMER_10us_ISR(void){ // every 1ms so 10kHz
	if(RC_Gear_state == 1)RC_gear_100us_counter ++;
	if(RC_AUX1_state == 1)RC_AUX1_100us_counter ++;
}



void RC_Recive(void){

	CAPTURE_Start(&CAPTURE_RC_Steering);
	CAPTURE_Start(&CAPTURE_RC_Speed);

	CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_RC_Steering, &captured_time_Steering);
	CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_RC_Speed, &captured_time_Speed);

	if(RC_Speed_Stop_counter < RC_SPEED_SAFTY_FT ){
		if((captured_time_Speed >= 1000000) && (captured_time_Speed <= 2000000)){
		  RC_Speed = ((double)captured_time_Speed - RC_NP_SPEED) * RC_Speed_ns_to_mps; //  form +-0.6m/s
		 // if((RC_Speed < 0.1) && (RC_Speed > -0.1)) RC_Speed = 0; // to have accutal 0
		  RC_no_Speed_data_counter = 0;
		}
		else{
			RC_no_Speed_data_counter ++;
		}
	}
	else RC_Speed = 0;

	if((captured_time_Steering >= 1000000) && (captured_time_Steering <= 2000000)){
	  RC_Steering = -(((double)captured_time_Steering - RC_NP_STEERING) * RC_Steering_ns_to_deg); // form +-90m/s
	  if(within_MAX_MIN(RC_Steering, 2, -2)) RC_Steering = 0; // to have accutal 0
	  RC_no_Steering_data_counter = 0;
	}
	else{
	  RC_no_Steering_data_counter ++;
	}

	if(RC_no_Speed_data_counter > 20 || RC_no_Steering_data_counter > 20 ){ // internall values not ok
	  DIGITAL_IO_SetOutputLow(&RC_no_data_LED);
	  RC_no_Speed_data_counter = 20;
	  RC_no_Steering_data_counter = 20;

	  //set gobal vars 0
	  Driving_speed = 0;
	  Steering_direction = 0;
	}
	else{ // values filternig
	  DIGITAL_IO_SetOutputHigh(&RC_no_data_LED);

	  //set gobal vars stanart
	  Driving_speed = RC_Speed;
	  Steering_direction = RC_Steering;
	}

	New_Input = 1; //goalb var to tell it that there in new data to calculate
}

void RC_Connected_ISR(void){

	if(RC_controll == 1){

		RC_Gear_state =  PIN_INTERRUPT_GetPinValue(&RC_Connected); // pin haig or low

		if(RC_Gear_state == 0){// do nothing during high

			RC_Recive(); // get speed and steering data

			uint32_t RC_Gear_on_time_100us = RC_gear_100us_counter;
			RC_gear_100us_counter = 0;

			if((80 < RC_Gear_on_time_100us) && (RC_Gear_on_time_100us < 150)) RC_Gear_duty= 0;
			else if ((150 < RC_Gear_on_time_100us) && (RC_Gear_on_time_100us < 210)) RC_Gear_duty = 1;
			else RC_Gear_duty = 2;


			if(modeswitch_colldown == 0){
				if(RC_Gear_duty == 0 && RC_Gear_duty_OLD == 1){
					Steering_mode ++;
					if(Steering_mode > 5)Steering_mode = 0;
					modeswitch_colldown = 10;
				}
			}
			else modeswitch_colldown--;
			RC_Gear_duty_OLD = RC_Gear_duty;
		}
	}
}

void RC_AUX1_ISR(void){
	if(RC_controll == 1){
		RC_AUX1_state =  PIN_INTERRUPT_GetPinValue(&RC_AUX1); // pin haig or low

		if(RC_AUX1_state == 0){// do nothing during high
			uint32_t RC_AUX1_on_time_100us = RC_AUX1_100us_counter;
			RC_AUX1_100us_counter = 0;

			if((80 < RC_AUX1_on_time_100us) && (RC_AUX1_on_time_100us < 150)) RC_AUX1_duty= 0;
			else if ((150 <= RC_AUX1_on_time_100us) && (RC_AUX1_on_time_100us < 210)) RC_AUX1_duty = 1;
			else RC_AUX1_duty = 2;


			if(RC_AUX1_duty == 1)RC_Speed_Stop_counter = 0;
			else RC_Speed_Stop_counter ++ ;

			if(RC_Speed_Stop_counter > RC_SPEED_SAFTY_FT) RC_Speed_Stop_counter = RC_SPEED_SAFTY_FT;
		}
	}
}
