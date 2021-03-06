/*
 * Ultrasonic_Collision_Avoidance.h
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef ULTRASONIC_COLLISION_AVOIDANCE_H_
#define ULTRASONIC_COLLISION_AVOIDANCE_H_

#include "Globals.h"
#include "DAVE.h"
#include "other_functions.h"
#include "CAN.h"
#include "DEFINE_CONST.h"

enum ULTRASONIC_SENSOR{
	FRONT_ = 0,
	LEFT = 1,
	RIGHT = 2,
};



typedef struct Obstacel{
	bool isValid;
	enum ULTRASONIC_SENSOR Sensor;
	float distance;
	float angle;
	float vector[2];
}Obstacel;

void Ultra_sonic_filter(int16_t Ultrasonic_cm_C_clc, int16_t Ultrasonic_cm_L_clc, int16_t Ultrasonic_cm_R_clc, float ALPHA_ULTARSONIC);
uint8_t Ultrasoinc_Obstructet_Test(float Angle);
void Obstacel_CA_check(Obstacel obstacel);
void set_CA_LED(uint8_t Sensor, uint8_t mode);

#endif /* ULTRASONIC_COLLISION_AVOIDANCE_H_ */
