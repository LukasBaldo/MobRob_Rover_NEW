/*
 * Steering_Servos.c
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */
#include "Steering_Servos.h"

uint16_t NP[4] = {805, 733, 708, 700}; //servo nutral points PWM duty values
uint16_t NPfl = 805; int NPfr = 750; int NPrl = 750; int NPrr = 750; // serov defalut 750

float Servo_PWMs[4] = {0, 0, 0, 0};

void Steering_set_Angles(float Angles_data[4]){
	float deg_PWM = 0.645; // deg to PWM singlan converstion factor

	// calcuation of PWM for Angle on each wheel
	int i;
	for(i = 0 ; i < 4 ; i ++){
		Servo_PWMs[i] = Angles_data[i]*deg_PWM + NP[i];
		}

	//limitinbg max servor Angle to 1.75 rot in each dirction
	for(i = 0 ; i < 4 ; i ++){
		if(Servo_PWMs[i] > 950) { Servo_PWMs[i] = 950;}
		if(Servo_PWMs[i] < 550) { Servo_PWMs[i] = 550;}
	}

	//seting PWM values
	PWM_SetDutyCycle(&PWM_servo_fl,Servo_PWMs[0]);
	PWM_SetDutyCycle(&PWM_servo_fr,Servo_PWMs[1]);
	PWM_SetDutyCycle(&PWM_servo_rl,Servo_PWMs[2]);
	PWM_SetDutyCycle(&PWM_servo_rr,Servo_PWMs[3]);

}

void Servo_NP_setting(void){
	  NP[0] = NPfl;
	  NP[1] = NPfr;
	  NP[2] = NPrl;
	  NP[3] = NPrr;
}