/*
 * main.c
 *
 *  Created on: 2020 Jan 02 10:21:43
 *  Author: Baldo
 */

// status: 11.05.2022

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>
#include <stdio.h> //for printf

#include "Globals.h"
#include "DEFINE_CONST.h"
#include "Hall_Sensors.h"
#include "rec_data_function.h"
#include "Read_Current_Sensor.h"
#include "CAN.h"
#include "PI_Controller.h"

//#######################
//FUNCTION DECLARATION
//########################
void Control_calc(void);

//########################
//VARIABLE DECLARATION
//########################

//control type
uint8_t Torque_control = 1;
uint8_t Speed_control = 1; //if 0 is torque control if 1 is speed control
uint8_t CAN_control = 1; // id 1 CAN speed controll aktive

float Current_U, Current_V, Current_W;

float ialpha,ibeta,iq,id,Valpha,Vbeta;


// REF vaules given to PI controller
float speed_ref = 0.0; //meters per second
float omega_ele_rads_ref = 0.0; // electrical rad per second
float iq_ref = 0.0;
float T_ref = 0.0;
float T_ref_controlled;
float omega_mech_rps_ref = 0;

float Vd = 0;
float Vq = 0;

volatile float modulation_index; //SK		// modulation index --> calculated via m = V / K_inv
volatile float modulation_index_scaled; //SK	//scaled from -1 to +1 towards -10000 to +10000 which is required from the SVM app

float angle_ab;
float angle_ab_rad;
float amplitude_ab;

float Ts = 1.0 /20000.0; //T sample correspond to SPVM update rate
float Tcyc = 1.0 /5000.0; //Tcyc sample correspond to control update rate to PI controler for I

// scaling factor for SVM Update -> amplitude scaling (max_amplitude = 16384U)
float scaling_factor = 135;
float amplitude_corrected;

// Counter for calculation function -> only use a multiple of the SVM Interrupt
volatile uint8_t count = 0;


uint8_t Speed_detection_OK = 0;

//step rec
uint32_t setting_cyc = 15000;
float step_start = 0.06; //0.13 ; M1 //  0.06; M2
float step_to = 0.08;  //0.17;  // 0l09 ; M2

//start help
uint8_t start_help_count = 0, start_help = 0, start_help_flag = 0;
#define START_HELP_T 0.1

//########################
// MAIN
//########################

int main(void)
{
  DAVE_STATUS_t status;
  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  Hall_init();

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  //Initialization of CAN
  	CAN_NODE_STATUS_t init_status;
  	init_status = CAN_NODE_Init(&CAN_NODE_0);

  	if (init_status != CAN_NODE_STATUS_SUCCESS) {
  		XMC_DEBUG("CAN initialization failed\n");
  	}

  if((MOTOR_ON_ROVER > 3) || MOTOR_ON_ROVER < 0){ // if possibel number eneterd
	  while(1U)
	      {
		//  printf("MOTOR_ON_ROVER number false");
	      }
  }


  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	  omega_param_selctor(MOTOR_NUM); //set motor spefici speeed parameters

	  PWM_SVM_Start(&PWM_SVM_0); // synchronous start for the CC8 slices.
		  while(1U)
		    {
			  if(count >=4)		//only calculate a multiple of 20kHz -> 4*50us=200us
			  {
				  DIGITAL_IO_SetOutputHigh(&status_LED_red_cal_time);
				  count=0;
				  Hall_postion_speed_estimation();
				  Control_calc();

				 // f_rec_data(iq	,Vq,&Vq,1,2,1,10000);  //  iq char

				  //f_rec_data(iq	,Vq,&iq_ref,1,2,1,10000); // iq step test

				   //f_rec_data(omega_mech_rps,T_ref,&T_ref,step_start,step_to,10,setting_cyc); //10000 settling  //  tref speedd spech char

				   //f_rec_data(omega_mech_rps,T_ref,&omega_mech_rps_ref,2,4,50,2000); // speed step test

				   //rec_step_from_0(omega_mech_rps, iq_ref,iq, &omega_mech_rps_ref, 4, 20); // start help test
				   start_help_flag = 0;

				   DIGITAL_IO_SetOutputLow(&status_LED_red_cal_time);
			  }
		    }
  }
}


//########################
// INTERRUPTS
//########################

/*
// Turn off driver at Overcurrent
void OverCurrent_U_ISR(void)
{
	//DIGITAL_IO_SetOutputLow(&Driver_U);	// switch off U driver
}
void OverCurrent_V_ISR(void)
{
	//DIGITAL_IO_SetOutputLow(&Driver_U);	// switch off U driver
}
void OverCurrent_W_ISR(void)
{
	//DIGITAL_IO_SetOutputLow(&Driver_U);	// switch off U driver
}
*/

//ISR based on Timer_1 --> 100us time base
void ISR_INT1(void)
{
}

// ISR for Updating the SVM PWM -> 50us = 20kHz
void PeriodMatchInterruptHandler(void)
{
	count++;	// only use a multiple of the period match frequency 20*20kHz = 1ms
	if(count == 1){ // only updating evry time there is new data safes 5-8 % duty on the Control_calc
		PWM_SVM_SVMUpdate(&PWM_SVM_0, (uint16_t)modulation_index_scaled, (uint32_t)angle_ab * ANGLE_ONE_DEGREE); // SK, 11.01.2019: outcommented to enable manual testing in ISR_INT1
	}
}

//########################
// FUNCTIONS
//########################


// Routine for Control_calc
//5kHz loop since every 4th step of 50us --> 200us --> 5kHz
void Control_calc(void){

		// Current read out over SPI
		Current_U = readCurrent(ChipSelect_U);
		Current_V = readCurrent(ChipSelect_V);
		Current_W = readCurrent(ChipSelect_W);

		// conversion from rad/s to degree
		float angle_phi_rad = angle_phi*Pi180;

		// to decrease multiple calculations and computational effort
		float cos_phi = cosf(angle_phi_rad);
		float sin_phi = sinf(angle_phi_rad);

		// Clark-Park transformation (from U,V,W to dq)
		id = TwoThird*(cos_phi*Current_U	+ cosf(angle_phi_rad-TwoPiThird)*Current_V + cosf(angle_phi_rad-FourPiThird)*Current_W);
		iq = TwoThird*(-sin_phi*Current_U	 -sinf(angle_phi_rad-TwoPiThird)*Current_V - sinf(angle_phi_rad-FourPiThird)*Current_W);

		if(CAN_control == 1){// can control

			if(CAN_new_meassage == 1){
				speed_ref = CAN_speed_ref;
				CAN_new_meassage = 0;
			}
			else{
				CAN_no_com_counter ++;
			}

			if(CAN_no_com_counter > CAN_NO_COM_TH){
				//CAN_new_meassage = 0;
				CAN_no_com_counter = CAN_NO_COM_TH;
				speed_ref = 0;
			}
			omega_mech_rps_ref = (speed_ref / WHEEL_R) / (2 * PI);
		}

		// Always run speed controll calc
		if(omega_mech_rps_ref == 0  && (( -2 < omega_mech_rps) && (omega_mech_rps < 2))){ // no contorl is standing stil
			T_ref_controlled = 0;
		}
		else{
			T_ref_controlled = PI_Controller(omega_mech_rps_ref,omega_mech_rps,&omega_param); // outer control loop q for omega

			if(start_help == 1 ){// troque added at start
				if(omega_mech_rps_ref != 0 && (copysign(1,omega_mech_rps_ref) != copysign(1,omega_mech_rps) || omega_mech_rps == 0)){
					start_help_count = 10;
				}

				if(start_help_count > 0){
					start_help_flag = 1;
					start_help_count --;
					if(omega_mech_rps_ref > 0)T_ref_controlled += START_HELP_T;
					else if(omega_mech_rps_ref < 0)T_ref = T_ref_controlled -= START_HELP_T;
				}
			}
		}

		if (Speed_control == 1){// activation of speed control
			T_ref = T_ref_controlled;
		}

		if(Torque_control == 1){

			if(MOTOR_NUM == 1 || MOTOR_NUM == 10) {
				iq_ref =  T_ref / K_T; // dirction for motor 1 and 10
			}
			else{
				iq_ref = - T_ref / K_T;// dirction for others
			}

			//limit to IQ_REF_MAx limit
			if(iq_ref < -IQ_REF_MAX) iq_ref = -IQ_REF_MAX;
			if(iq_ref > IQ_REF_MAX) iq_ref = IQ_REF_MAX;


			Vd = PI_Controller(0.0,id,&Id_param);
			Vq = PI_Controller(iq_ref,iq,&Iq_param); // toque controll
		}

		// dq to alpha beta for voltage
		Valpha = Vd*cos_phi - Vq*sin_phi;
		Vbeta = Vd*sin_phi + Vq*cos_phi;

		// calc angle for SVM app
		if(Valpha == 0) Valpha =  0.00001; // prevent exeption div by 0
		angle_ab_rad = atanf(Vbeta / Valpha);
		angle_ab = angle_ab_rad / Pi180;

		if(Valpha < 0.0){angle_ab = angle_ab + 180.0;}			// failure correction of angle (tangens)
		else if(Vbeta < 0.0){angle_ab = angle_ab + 360.0;}

		// amplitude calculation
		amplitude_ab = sqrtf(Valpha*Valpha + Vbeta*Vbeta);

		//K_inv = V/m --> m = V / K_inv
		modulation_index = amplitude_ab / (V_DC_link / 1.732);

		//scale maximum 1 to 10000 -->
		modulation_index_scaled = 10000*modulation_index;

		DIGITAL_IO_SetOutputLow(&status_LED_red_cal_time); // 83% duty amplitued calc takes 16% of the time

}




