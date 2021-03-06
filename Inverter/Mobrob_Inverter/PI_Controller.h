/*
 * PI_Controller.h
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */

#ifndef PI_CONTROLLER_H_
#define PI_CONTROLLER_H_

#include <stdint.h> // need for data types

//################ PI
// PID Controller declaration for Id and Iq component
typedef struct PI_param{
	float P,I;
	float MaxLimit, MinLimit;
	float Output;
	float Deviation_old, Deviation_old2;
	float Output_z;
}PI_param;

extern PI_param Iq_param;
extern PI_param Id_param;
extern PI_param omega_param;

void omega_param_selctor(uint8_t Motor_num);

float PI_Controller(float SetPoint,float ActValue,PI_param *param);


#endif /* PI_CONTROLLER_H_ */
