/*
 * Globas.c

 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#include "Globals.h"

volatile uint8_t RC_controll = 1; // if 1 RC controll

float Steering_direction = 0; // steering Angle
float Driving_speed = 0;

volatile float Speeds[4] = {0, 0, 0, 0};
volatile float Steering_Angles[4] = {0, 0, 0, 0};

float Actual_Speeds[4] = {0};
float Actual_Distance[4] = {0};

volatile enum STEERING_MODES Steering_mode = ZERO;
volatile uint8_t New_Input = 0;
volatile uint32_t NO_New_Input_counter = 0;
volatile uint8_t modeswitch_colldown = 10;
volatile uint8_t reset_distance = 0;

volatile uint8_t CAN_Ultrasonic_No_messasge_count = 100;
volatile uint8_t CAN_state_Ultrasonic = 0;
volatile uint16_t Ultrasonic_cm_C = 0;
volatile uint16_t Ultrasonic_cm_L = 0;
volatile uint16_t Ultrasonic_cm_R = 0;

float ALPHA_ULTARSONIC = 0.3; // giltering prameter for the Exponential moving average
