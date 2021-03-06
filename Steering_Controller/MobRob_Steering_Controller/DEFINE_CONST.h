/*
 * DEFINE_CONST.h
 *
 *  Created on: 1 Jul 2022
 *      Author: lukas
 */

#ifndef DEFINE_CONST_H_
#define DEFINE_CONST_H_

//#define PI180 0.01745329
#define PI 3.14
#define HALF_PI 1.57
#define RAD_TO_DEG 57.297795

// steering parameter
//const float WHEEL_BASE = 0.448;
#define WHEEL_BASE 0.448
#define	HALF_WHEEL_BASE 			0.22				// halfbase of mobrob
//#define 	HALFBASEPOW2 		(HALFBASE*HALFBASE)		// halfbase of mobrob to the power of two
#define	HALF_TRACK_WIDTH		0.163					// halftrack of mobrob
#define 	R_WHEEL				0.067			// wheel radius of mobrob
#define	R_TRUN_MAX				20.000				// maximum corner radius (before straight)
#define 	R_TRUN_MIN				0.05				// minimum corner/reorientation radius
//#define		P_RO_VEL			0.500					// reduction factor for reorientation velocity
//#define		CWHEEL				(2.000*M_PI*RWHEEL)		// wheel circumference of mobrob

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

#endif /* DEFINE_CONST_H_ */
