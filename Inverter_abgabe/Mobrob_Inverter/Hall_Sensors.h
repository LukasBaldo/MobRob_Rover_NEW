/*
 * Hall_Sensors.h
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */

#ifndef HALL_SENSORS_H_
#define HALL_SENSORS_H_

#include <stdint.h> // need for data types
#include <DAVE.h>
#include "Globals.h"
#include "DEFINE_CONST.h"

void Hall_init(void);
void HallSensor(void);
void Hall_postion_speed_estimation(void);

#endif /* HALL_SENSORS_H_ */
