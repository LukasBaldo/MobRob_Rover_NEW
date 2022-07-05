/*
 * Driving_dynamics.h
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef DRIVING_DYNAMICS_H_
#define DRIVING_DYNAMICS_H_


#include <stdint.h> // need for data types
#include "Globals.h"
#include "DEFINE_CONST.h"
#include <math.h>
#include <DAVE.h>
#include "other_functions.h"

void Steering_Function(float Steering_direction_cal, float Driving_speed_cal, uint8_t Steering_mode);
void Trajcetory_calc();
void mode_led(uint8_t var);

#endif /* DRIVING_DYNAMICS_H_ */
