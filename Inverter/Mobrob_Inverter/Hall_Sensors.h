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


extern volatile int8_t sector;
extern volatile int8_t direction;              // direction +1/-1 -> CW and CCW

extern volatile uint32_t time_since_A ;	// time between to edges Hall A
extern volatile uint32_t time_since_B; // time between to edges Hall B
extern volatile uint32_t time_since_C; // time between to edges Hall C
extern volatile uint32_t time_180deg;        // time which last triggered HAL sensor needed for 180°
extern volatile char last_Hall_trig;			//last triggered Hall Sensor A/B/C

void Hall_init(void);
void HallSensor(void);
void Hall_postion_speed_estimation(void);

#endif /* HALL_SENSORS_H_ */
