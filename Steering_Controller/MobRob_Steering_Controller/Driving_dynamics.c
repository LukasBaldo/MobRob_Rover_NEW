/*
 * Driving_dynamics.c
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#include <stdint.h> // need for data types
#include "Globals.h"
#include "DEFINE_CONST.h"
#include <math.h>
#include "Driving_dynamics.h"
#include <DAVE.h>

#define ROTATION_ANGLE 53.96
#define ROTATION_ANGLE_OFFSET 5
#define ROTATION_SPEED  0.25

float R_Turn;
float Speed_ratio;

float speed_fl = 0, speed_fr = 0, speed_rl = 0, speed_rr = 0; // in m/s
float angle_fl = 0, angle_fr = 0, angle_rl = 0, angle_rr = 0; // in deg  0 straing ahead stering right positive left negative


void Steering_Function(float Steering_direction_cal, float Driving_speed_cal, uint8_t Steering_mode_cal){
	New_Input = 0;

	if(Driving_speed_cal > MAX_Speed) Driving_speed_cal = MAX_Speed;
	if(Driving_speed_cal < -MAX_Speed) Driving_speed_cal = -MAX_Speed;
	if((0 < Driving_speed_cal) & (Driving_speed_cal < MIN_Speed)) Driving_speed_cal = 0;
	if((0 > Driving_speed_cal) & (Driving_speed_cal > -MIN_Speed)) Driving_speed_cal = 0;

	switch(Steering_mode_cal) // options 'Front'; 'Rear'; '4_Wheel'; 'Carb'; 'Rotate'
	{
		case FRONT://#############################################################
			// steering limits
			if(Steering_direction_cal > 50) Steering_direction_cal = 50;
			if(Steering_direction_cal < -50) Steering_direction_cal = -50;

			// pre calc
			R_Turn = WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{
				angle_fl = 0;
				angle_fr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}

			//rear axle
			angle_rl = 0;
			angle_rr = 0;
			break;

		case BACK:// rear #############################################################
			// steering limits
			if(Steering_direction_cal > 50) Steering_direction_cal = 50;
			if(Steering_direction_cal < -50) Steering_direction_cal = -50;

			// pre calc
			R_Turn = WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){
				angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel


				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){
				angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{
				angle_rl = 0;
				angle_rr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}

			//rear axle
			angle_fl = 0;
			angle_fr = 0;
			break;

		case ALL_WHEEL: // 4 wheel #############################################################
			// steering limits
			if(Steering_direction_cal > 30) Steering_direction_cal = 30;
			if(Steering_direction_cal < -30) Steering_direction_cal = -30;

			// pre calc
			R_Turn = HALF_WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){ // trun right positive R
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel
				//angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				//angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				angle_rl = -angle_fl;
				angle_rr = -angle_fr;

				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){ // trun left positive R
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel
				//angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				//angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				angle_rl = -angle_fl;
				angle_rr = -angle_fr;

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{// starigt driving
				angle_fl = 0;
				angle_fr = 0;
				angle_rl = 0;
				angle_rr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}
			break;

		case CRAB:  //carb //#############################################################
			Steering_direction_cal = Steering_direction_cal * 2.5; // mach data from remote
			if(Steering_direction_cal > 190) Steering_direction_cal = 190;
			if(Steering_direction_cal < -190) Steering_direction_cal = -190;
			// steering
			angle_fl = Steering_direction_cal;
			angle_fr = Steering_direction_cal;
			angle_rl = Steering_direction_cal;
			angle_rr = Steering_direction_cal;
			 // speed
			 speed_fl = Driving_speed_cal;
			 speed_fr = Driving_speed_cal;
			 speed_rl = Driving_speed_cal;
			 speed_rr = Driving_speed_cal;
			break;

		case ROTATE: // rotate #############################################################
			// steering agel for turng in place
			angle_fl = ROTATION_ANGLE + ROTATION_ANGLE_OFFSET;
			angle_fr = -ROTATION_ANGLE + ROTATION_ANGLE_OFFSET;
			angle_rl = -ROTATION_ANGLE + ROTATION_ANGLE_OFFSET;
			angle_rr = ROTATION_ANGLE + ROTATION_ANGLE_OFFSET;

			 // speed
			if (Steering_direction_cal > 10){
				speed_fl = ROTATION_SPEED;
				speed_fr = -ROTATION_SPEED;
				speed_rl = ROTATION_SPEED;
				speed_rr = -ROTATION_SPEED;
			}
			else if(Steering_direction_cal < -10){
				speed_fl = -ROTATION_SPEED;
				speed_fr = ROTATION_SPEED;
				speed_rl = -ROTATION_SPEED;
				speed_rr = ROTATION_SPEED;
			}
			else{
				speed_fl = 0;
				speed_fr = 0;
				speed_rl = 0;
				speed_rr = 0;
			}
			break;

		default:
			angle_fl = 0;
			angle_fr = 0;
			angle_rl = 0;
			angle_rr = 0;

			speed_fl = 0;
			speed_fr = 0;
			speed_rl = 0;
			speed_rr = 0;

	}
  // set gobal vars
  Steering_Angles[0] = angle_fl;
  Steering_Angles[1] = angle_fr;
  Steering_Angles[2] = angle_rl;
  Steering_Angles[3] = angle_rr;

  Speeds[0] = speed_fl;
  Speeds[1] = speed_fr;
  Speeds[2] = speed_rl;
  Speeds[3] = speed_rr;

}



void mode_led(uint8_t var){
	if(CHECK_BIT(var,0)){
		DIGITAL_IO_SetOutputLow(&MODE_001);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_001);
	}

	if(CHECK_BIT(var,1)){
		DIGITAL_IO_SetOutputLow(&MODE_010);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_010);
	}

	if(CHECK_BIT(var,2)){
		DIGITAL_IO_SetOutputLow(&MODE_100);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_100);
	}
}
