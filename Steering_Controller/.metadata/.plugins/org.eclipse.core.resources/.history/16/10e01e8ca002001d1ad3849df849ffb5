/*
 * main.c
 *
 *  Created on: 2022 Apr 25 09:31:37
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>

#include "Globals.h"
#include "other_functions.h"
#include "Steering_servos.h"
#include "Driving_dynamics.h"
#include "CAN.h"
#include "Ultrasonic_Collision_Avoidance.h"
#include "RC_data_recive.h"

//VAR CAN
CAN_NODE_STATUS_t init_status;
int CAN_OK = 0;

uint8_t test = 0;


int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if (status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  //Initialization of CAN
  	//CAN_NODE_STATUS_t init_status;
  	init_status = CAN_NODE_Init(&CAN_NODE_0);

  	if (init_status != CAN_NODE_STATUS_SUCCESS) {
  		XMC_DEBUG("CAN initialization failed\n");
  	}
  	else{
  		CAN_OK = 1;
  	}

  	WATCHDOG_Start(); // watchdog restart if CAN not working


  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	//Servo_NP_setting();	  // nutral point servo setting

  }
}

void Watchdog_Handler(void) // to do restates if can is not working a problem wenn power from the buck or plab supplie but ok like this
{
  WATCHDOG_ClearAlarm(); /* Clear the alarm event */
  DIGITAL_IO_SetOutputHigh(&WATCHDOG_LED_BLUE);
}


void TIMER_CONTROL_ISR(void){
	DIGITAL_IO_SetOutputHigh(&CALC_TIME_INDICATOR);
	mode_led(Steering_mode);

	if(RC_controll == 1){
		if( NO_New_Input_counter > 9){
			DIGITAL_IO_SetOutputLow(&RC_no_data_LED);
		}
	}


	if(New_Input == 1){
	Steering_Function(Steering_direction, Driving_speed, Steering_mode);
	NO_New_Input_counter = 0;

	}
	else{
		NO_New_Input_counter ++;
		if( NO_New_Input_counter > 10){
			NO_New_Input_counter = 10;

			  Steering_Angles[0] = 0;
			  Steering_Angles[1] = 0;
			  Steering_Angles[2] = 0;
			  Steering_Angles[3] = 0;

			  Speeds[0] = 0;
			  Speeds[1] = 0;
			  Speeds[2] = 0;
			  Speeds[3] = 0;
		}
	}

	//set Angles PWM
	Steering_set_Angles(Steering_Angles);

	Trajcetory_calc();

	Collision_voidance();

	//send traget speeds to inverter
	CAN_send_Speeds(Speeds_CA);

	//
	if(reset_distance == 1){
		reset_distance = 0;
		uint8_t all[4] = {1,1,1,1};
		CAN_reset_distance(all);
	}


	DIGITAL_IO_SetOutputLow(&CALC_TIME_INDICATOR);
}
