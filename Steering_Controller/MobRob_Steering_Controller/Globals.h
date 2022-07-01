/*
 * Globals.h
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h> // need for data types

volatile uint8_t RC_controll;

extern float Steering_direction; // steering Angle
extern float Driving_speed;

extern volatile float Speeds[4];
extern volatile float Steering_Angles[4];

extern float Actual_Speeds[4];
extern float Actual_Distance[4];

enum STEERING_MODES{
	ZERO = 0,
	FRONT = 1,
	BACK = 2,
	ALL_WHEEL = 3,
	CRAB = 4,
	ROTATE =5
};

extern volatile enum STEERING_MODES Steering_mode;
extern volatile uint8_t New_Input;
extern volatile uint32_t NO_New_Input_counter;
extern volatile uint8_t modeswitch_colldown;
extern volatile uint8_t reset_distance;



extern volatile uint8_t CAN_Ultrasonic_No_messasge_count;
extern volatile uint8_t CAN_state_Ultrasonic;
extern volatile uint16_t Ultrasonic_cm_C;
extern volatile uint16_t Ultrasonic_cm_L;
extern volatile uint16_t Ultrasonic_cm_R;

extern float ALPHA_ULTARSONIC;

#endif /* GLOBALS_H_ */
