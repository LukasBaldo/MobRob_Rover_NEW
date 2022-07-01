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

void Ultra_sonic_filter(int16_t Ultrasonic_cm_C_clc, int16_t Ultrasonic_cm_L_clc, int16_t Ultrasonic_cm_R_clc, float Steering_Angles_clc[4], float ALPHA_ULTARSONIC);
uint8_t Ultrasoinc_Obstructet_Test(float Angle);

#endif /* ULTRASONIC_COLLISION_AVOIDANCE_H_ */
