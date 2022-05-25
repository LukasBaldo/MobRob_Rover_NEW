/*
 * main.c
 *
 *  Created on: 2022 Apr 25 09:31:37
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

//constant
int NP[4] = {775, 735, 735, 700}; //servo nutral points PWM duty vaules
int NPfl = 750; int NPfr = 750; int NPrl = 750; int NPrr = 750;
#define MIN_Speed  0.05
#define MAX_Speed  1

#define ROTATION_ANGEL 53.96
#define ROTATION_ANGEL_OFFSET 5
#define ROTATION_SPEED  0.25

#define PI180 0.01745329
#define PI 3.14
#define HALF_PI 1.57
#define RAD_TO_DEG 57.296

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


// Var MODES
uint8_t RC_controll = 1; // if 1 RC controll
uint8_t Steering_mode = 3; // otions 1 'Front';  2'Rear'; 3 '4_Wheel'; 4 'Carb'; 5 'Rotate' 0 nothing set in mircum or all 0
uint8_t New_Input = 0;
uint32_t NO_New_Input_counter = 0;


//VAR CAN
int16_t Speeds_int16_r[4] = {0};
int16_t CAN_RX_0 = 0;
CAN_NODE_STATUS_t init_status;
int CAN_OK = 0;
uint8_t Inverters_CAN_state[4] = {0};
uint32_t CAN_10ms_counter = 0; // to send ever 10ms

uint8_t reset_distance = 0;// reste from mircum by setting 1
float speed_fl_act = 0;float speed_fr_act = 0;float speed_rl_act = 0;float speed_rr_act = 0; // in m/s
float distance_fl_act = 0;float distance_fr_act = 0;float distance_rl_act = 0;float distance_rr_act = 0; // in m/s

volatile uint8_t CAN_RX_Inverter_count = 0;

//ULTRASOCNIC
uint8_t CAN_state_Ultrasonic = 0;
uint16_t distance_C = 0;
uint16_t distance_L = 0;
uint16_t distance_R = 0;

// Var RC
uint8_t RC_Signal_OK = 0, RC_Signal_not_OK_counter = 0; // 0 no ok 1 OK
uint32_t  captured_time_Steering=0, captured_time_Speed=0;
uint8_t RC_no_Speed_data_counter = 0, RC_no_Steering_data_counter = 0;
float RC_Speed = 0, RC_Steering = 0;
#define RC_NP_SPEED  1500000
#define RC_NP_STEERING  1540000
float RC_Speed_ns_to_mps = 0.00000125;
float RC_Steering_ns_to_deg = 0.00021;

uint32_t RC_gear_100us_counter = 0, RC_AUX1_100us_counter = 0;
uint8_t RC_Gear_state = 2, RC_AUX1_state = 2;
uint8_t RC_Gear_duty = 2, RC_Gear_duty_OLD =2, RC_AUX1_duty = 2;
uint8_t RC_Speed_Stop = 1;

//VAR general
float Steering_direction = 0; // steering angel
float Driving_speed = 0;

float R_Turn;
float Speed_ratio;

float speed_fl = 0;float speed_fr = 0;float speed_rl = 0;float speed_rr = 0; // in m/s
float angle_fl = 0;float angle_fr = 0;float angle_rl = 0;float angle_rr = 0; // in deg  0 straing ahead stering right positive left negative

float Speeds[4] = {0, 0, 0, 0};
float Steering_Angles[4] = {0, 0, 0, 0};
float Servo_PWMs[4] = {0, 0, 0, 0};

//vaules form inveter bioard over CAN
uint8_t Inveter_CAN_OK[4] = {0};
float Actual_Speeds[4] = {0};
float Actual_Distance[4] = {0};

// FUNCTIONS
void CAN_send_Speeds(float Speed_data[4]);
void CAN_reset_distance(uint8_t distance_to_reset[4]);
void Steering_set_Angles(float Angles_data_int[4]);
void Steering_Function(float Steering_direction_cal, float Driving_speed_cal, uint8_t Steering_mode);
void RC_Recive(void);
void Servo_NP_setting(void);
void mode_led(uint8_t var);
float average(float a[],int num_vaules);
void CAN_Inveter_RX_Interperter(uint8_t data_RX[8], uint8_t Motor_on_rover);
void CAN_RX_Inverter_Read_Data();


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

  	// watchdog
  	WATCHDOG_Start();

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	//  Servo_NP_setting();	  // nutral point servo setting
  }
}


void Steering_Function(float Steering_direction_cal, float Driving_speed_cal, uint8_t Steering_mode_cal){
	New_Input = 0;

	if(Driving_speed_cal > MAX_Speed) Driving_speed_cal = MAX_Speed;
	if(Driving_speed_cal < -MAX_Speed) Driving_speed_cal = -MAX_Speed;
	if((0 < Driving_speed_cal) & (Driving_speed_cal < MIN_Speed)) Driving_speed_cal = 0;
	if((0 > Driving_speed_cal) & (Driving_speed_cal > -MIN_Speed)) Driving_speed_cal = 0;

	switch(Steering_mode_cal) // options 'Front'; 'Rear'; '4_Wheel'; 'Carb'; 'Rotate'
	{
		case 1://#############################################################
			// steering limits
			if(Steering_direction_cal > 50) Steering_direction_cal = 50;
			if(Steering_direction_cal < -50) Steering_direction_cal = -50;

			// pre calc
			R_Turn = WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{
				angle_fl = 0;
				angle_fr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}

			//rear axle
			angle_rl = 0;
			angle_rr = 0;
			break;

		case 2:// rear #############################################################
			// steering limits
			if(Steering_direction_cal > 50) Steering_direction_cal = 50;
			if(Steering_direction_cal < -50) Steering_direction_cal = -50;

			// pre calc
			R_Turn = WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){
				angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel


				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){
				angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{
				angle_rl = 0;
				angle_rr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}

			//rear axle
			angle_fl = 0;
			angle_fr = 0;
			break;

		case 3: // 4 wheel #############################################################
			// steering limits
			if(Steering_direction_cal > 30) Steering_direction_cal = 30;
			if(Steering_direction_cal < -30) Steering_direction_cal = -30;

			// pre calc
			R_Turn = HALF_WHEEL_BASE / tan(Steering_direction_cal / RAD_TO_DEG); // to truni graduis converstion
			Speed_ratio = (2 * (copysign(R_Turn,1) + HALF_TRACK_WIDTH) * M_PI) / (2 * copysign(R_Turn,1) * M_PI);

			if((R_TRUN_MIN < R_Turn) && (R_Turn < R_TRUN_MAX)){ // trun right positive R
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel
				//angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // out wheel
				//angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				angle_rl = -angle_fl;
				angle_rr = -angle_fr;

				speed_fl = Driving_speed_cal * Speed_ratio;
				speed_fr = Driving_speed_cal / Speed_ratio;
				//speed_rl = Driving_speed_cal * Speed_ratio;
				//speed_rr = Driving_speed_cal / Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else if((- R_TRUN_MIN > R_Turn) && (R_Turn > -R_TRUN_MAX)){ // trun left positive R
				angle_fl = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				angle_fr = RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // out wheel
				//angle_rl = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn + HALF_TRACK_WIDTH)); // in wheel
				//angle_rr = - RAD_TO_DEG * atan(WHEEL_BASE /(R_Turn - HALF_TRACK_WIDTH)); // in wheel

				angle_rl = -angle_fl;
				angle_rr = -angle_fr;

				speed_fl = Driving_speed_cal / Speed_ratio;
				speed_fr = Driving_speed_cal * Speed_ratio;
				//speed_rl = Driving_speed_cal / Speed_ratio;
				//speed_rr = Driving_speed_cal * Speed_ratio;

				speed_rl = speed_fl;
				speed_rr = speed_fr;
			}
			else{// starigt driving
				angle_fl = 0;
				angle_fr = 0;
				angle_rl = 0;
				angle_rr = 0;

				speed_fl = Driving_speed_cal;
				speed_fr = Driving_speed_cal;
				speed_rl = Driving_speed_cal;
				speed_rr = Driving_speed_cal;
			}
			break;

		case 4:  //carb //#############################################################
			Steering_direction_cal = Steering_direction_cal * 2; // mach data from remote
			if(Steering_direction_cal > 190) Steering_direction_cal = 190;
			if(Steering_direction_cal < -190) Steering_direction_cal = -190;
			// steering
			angle_fl = Steering_direction_cal;
			angle_fr = Steering_direction_cal;
			angle_rl = Steering_direction_cal;
			angle_rr = Steering_direction_cal;
			 // speed
			 speed_fl = Driving_speed_cal;
			 speed_fr = Driving_speed_cal;
			 speed_rl = Driving_speed_cal;
			 speed_rr = Driving_speed_cal;
			break;

		case 5: // rotate #############################################################
			// steering agel for turng in place
			angle_fl = ROTATION_ANGEL + ROTATION_ANGEL_OFFSET;
			angle_fr = -ROTATION_ANGEL + ROTATION_ANGEL_OFFSET;
			angle_rl = -ROTATION_ANGEL + ROTATION_ANGEL_OFFSET;
			angle_rr = ROTATION_ANGEL + ROTATION_ANGEL_OFFSET;

			 // speed
			if (Steering_direction_cal > 10){
				speed_fl = ROTATION_SPEED;
				speed_fr = -ROTATION_SPEED;
				speed_rl = ROTATION_SPEED;
				speed_rr = -ROTATION_SPEED;
			}
			else if(Steering_direction_cal < -10){
				speed_fl = -ROTATION_SPEED;
				speed_fr = ROTATION_SPEED;
				speed_rl = -ROTATION_SPEED;
				speed_rr = ROTATION_SPEED;
			}
			else{
				speed_fl = 0;
				speed_fr = 0;
				speed_rl = 0;
				speed_rr = 0;
			}
			break;

		default:
			angle_fl = 0;
			angle_fr = 0;
			angle_rl = 0;
			angle_rr = 0;

			speed_fl = 0;
			speed_fr = 0;
			speed_rl = 0;
			speed_rr = 0;

	}
  // set gobal vars
  Steering_Angles[0] = angle_fl;
  Steering_Angles[1] = angle_fr;
  Steering_Angles[2] = angle_rl;
  Steering_Angles[3] = angle_rr;

  Speeds[0] = speed_fl;
  Speeds[1] = speed_fr;
  Speeds[2] = speed_rl;
  Speeds[3] = speed_rr;

}


void CAN_RX_ULTRASONIC_ISR(void) { // recide data
	XMC_CAN_MO_t* lmsgobjct_ptr_1 = CAN_NODE_0.lmobj_ptr[6]->mo_ptr;
	CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[6]);

	uint8_t data_RX[8];
	uint8_t i;
	for(i = 0 ; i < 8 ; i ++){
		 data_RX[i] = lmsgobjct_ptr_1->can_data_byte[i];
	}

	CAN_state_Ultrasonic = (data_RX[0 * 2 + 1] << 8) | data_RX[0 * 2];
	distance_C = (data_RX[1 * 2 + 1] << 8) | data_RX[1 * 2];
	distance_L = (data_RX[2 * 2 + 1] << 8) | data_RX[2 * 2];
	distance_R = (data_RX[3 * 2 + 1] << 8) | data_RX[3 * 2];

}

void CAN_RX_INVERTER_ISR(void) {
	WATCHDOG_Service();
	DIGITAL_IO_SetOutputLow(&WATCHDOG_LED_BLUE);

	CAN_RX_Inverter_count++;
	if(CAN_RX_Inverter_count == 4) CAN_RX_Inverter_Read_Data(); // read data after all for have reviced asseumes all 4 are working
}

void CAN_RX_Inverter_Read_Data(){
	uint8_t Motor_on_rover;
	for(Motor_on_rover = 0; Motor_on_rover < 4; Motor_on_rover ++){
		XMC_CAN_MO_t* lmsgobjct_ptr_1 = CAN_NODE_0.lmobj_ptr[Motor_on_rover + 2]->mo_ptr;
		CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[Motor_on_rover + 2]);

		uint8_t data_RX[8];
		uint8_t i;
		for(i = 0 ; i < 8 ; i ++){
			 data_RX[i] = lmsgobjct_ptr_1->can_data_byte[i];
		}

		uint8_t Motor_on_rover_RX = (data_RX[0 * 2 + 1] << 8) | data_RX[0 * 2];
		if(Motor_on_rover_RX == Motor_on_rover){
			DIGITAL_IO_SetOutputLow(&LED_CAN_ERROR);

			uint8_t i;
			int16_t Vaules_int16[3] = {0};
			for(i = 1 ; i < 4 ; i ++){
				 if(i == 1) Vaules_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
				 else if(i == 2) Vaules_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
				 else if(i == 3) Vaules_int16[i - 1] = (data_RX[i * 2 + 1] << 8) | data_RX[i * 2];
			 }

			Inveter_CAN_OK[Motor_on_rover] = Vaules_int16[0];
			Actual_Speeds[Motor_on_rover] = (float)Vaules_int16[1] / 1000;
			Actual_Distance[Motor_on_rover] = (float)Vaules_int16[2] / 1000;


			}
		else DIGITAL_IO_SetOutputHigh(&LED_CAN_ERROR);
	}

	speed_fl_act = Actual_Speeds[0];
	speed_fr_act = Actual_Speeds[1];
	speed_rl_act = Actual_Speeds[2];
	speed_rr_act = Actual_Speeds[3];

	distance_fl_act = Actual_Distance[0];
	distance_fr_act = Actual_Distance[1];
	distance_rl_act = Actual_Distance[2];
	distance_rr_act = Actual_Distance[3];

}

void Watchdog_Handler(void) // to do restates if can is not working a problem wenn power from the buck or plab supplie but ok like this
{
  WATCHDOG_ClearAlarm(); /* Clear the alarm event */
  DIGITAL_IO_SetOutputHigh(&WATCHDOG_LED_BLUE);
}

void CAN_reset_distance(uint8_t distance_to_reset[4]){
	uint8_t TX[8] ={0};
	uint8_t i;
	for(i = 0; i < 4; i++){	TX[i] = distance_to_reset[i];}

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[1], (uint8_t*)TX);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[1]); //Transmit the data of message object1
}

void CAN_send_Speeds(float Speed_data[4]){

	int16_t Speeds_int16[4] ={Speed_data[0] *1000, Speed_data[1] *1000, Speed_data[2] *1000, Speed_data[3] *1000}; // in int16_t mm/s converter back on inverter

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[0], (uint8_t*)Speeds_int16);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[0]); //Transmit the data of message object1

}


void INTERRUPT_TIMER_10us_ISR(void){ // every 1ms so 10kHz
	if(RC_Gear_state == 1)RC_gear_100us_counter ++;
	if(RC_AUX1_state == 1)RC_AUX1_100us_counter ++;
}

void Steering_set_Angles(float Angles_data[4]){
	float deg_PWM = 0.645; // deg to PWM singlan converstion factor

	// calcuation of PWM for angel on each wheel
	int i;
	for(i = 0 ; i < 4 ; i ++){
		Servo_PWMs[i] = Angles_data[i]*deg_PWM + NP[i];
		}

	//limitinbg max servor angel to 1.75 rot in each dirction
	for(i = 0 ; i < 4 ; i ++){
		if(Servo_PWMs[i] > 950) { Servo_PWMs[i] = 950;}
		if(Servo_PWMs[i] < 550) { Servo_PWMs[i] = 550;}
	}

	//seting PWM vaules
	PWM_SetDutyCycle(&PWM_servo_fl,Servo_PWMs[0]);
	PWM_SetDutyCycle(&PWM_servo_fr,Servo_PWMs[1]);
	PWM_SetDutyCycle(&PWM_servo_rl,Servo_PWMs[2]);
	PWM_SetDutyCycle(&PWM_servo_rr,Servo_PWMs[3]);

}

void RC_Recive(void){

	CAPTURE_Start(&CAPTURE_RC_Steering);
	CAPTURE_Start(&CAPTURE_RC_Speed);

	CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_RC_Steering, &captured_time_Steering);
	CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_RC_Speed, &captured_time_Speed);

	if(RC_Speed_Stop == 1){
		if((captured_time_Speed >= 1000000) && (captured_time_Speed <= 2000000)){
		  RC_Speed = ((double)captured_time_Speed - RC_NP_SPEED) * RC_Speed_ns_to_mps; // form +-0.6m/s
		  if((RC_Speed < 0.1) && (RC_Speed > -0.1)) RC_Speed = 0; // to have accutal 0
		  RC_no_Speed_data_counter = 0;
		}
		else{
			RC_no_Speed_data_counter ++;
		}
	}
	else RC_Speed = 0;

	if((captured_time_Steering >= 1000000) && (captured_time_Steering <= 2000000)){
	  RC_Steering = (((double)captured_time_Steering - RC_NP_STEERING) * RC_Steering_ns_to_deg); // form +-90m/s
	  if((RC_Steering < 2) && (RC_Steering > -2)) RC_Steering = 0; // to have accutal 0
	  RC_no_Steering_data_counter = 0;
	}
	else{
	  RC_no_Steering_data_counter ++;
	}

	if(RC_no_Speed_data_counter > 20 || RC_no_Steering_data_counter > 20 ){ // internall vaules not ok
	  DIGITAL_IO_SetOutputLow(&RC_no_data_LED);
	  RC_no_Speed_data_counter = 20;
	  RC_no_Steering_data_counter = 20;

	  //set gobal vars 0
	  Driving_speed = 0;
	  Steering_direction = 0;
	}
	else{ // vaules filternig
	  DIGITAL_IO_SetOutputHigh(&RC_no_data_LED);

	  //set gobal vars stanart
	  Driving_speed = RC_Speed;
	  Steering_direction = RC_Steering;
	}

	New_Input = 1; //goalb var to tell it that there in new data to calculate
}

void RC_Connected_ISR(void){

	if(RC_controll == 1){

		RC_Gear_state =  PIN_INTERRUPT_GetPinValue(&RC_Connected); // pin haig or low

		if(RC_Gear_state == 0){// do nothing during high

			RC_Recive(); // get speed and steering data

			uint32_t RC_Gear_on_time_100us = RC_gear_100us_counter;
			RC_gear_100us_counter = 0;

			if((80 < RC_Gear_on_time_100us) && (RC_Gear_on_time_100us < 150)) RC_Gear_duty= 0;
			else if ((150 < RC_Gear_on_time_100us) && (RC_Gear_on_time_100us < 210)) RC_Gear_duty = 1;
			else RC_Gear_duty = 2;

			if(RC_Gear_duty == 1 && RC_Gear_duty_OLD == 0){
				Steering_mode ++;
				if(Steering_mode > 5)Steering_mode = 0;
			}
		RC_Gear_duty_OLD = RC_Gear_duty;
		}
	}
}

void RC_AUX1_ISR(void){
	if(RC_controll == 1){
		RC_AUX1_state =  PIN_INTERRUPT_GetPinValue(&RC_AUX1); // pin haig or low

		if(RC_AUX1_state == 0){// do nothing during high
			uint32_t RC_AUX1_on_time_100us = RC_AUX1_100us_counter;
			RC_AUX1_100us_counter = 0;

			if((80 < RC_AUX1_on_time_100us) && (RC_AUX1_on_time_100us < 150)) RC_AUX1_duty= 0;
			else if ((150 < RC_AUX1_on_time_100us) && (RC_AUX1_on_time_100us < 210)) RC_AUX1_duty = 1;
			else RC_AUX1_duty = 2;


			if(RC_AUX1_duty == 1)RC_Speed_Stop = 1;
			else RC_Speed_Stop = 0;
		}
	}
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

	//send traget speeds to inverter
	CAN_send_Speeds(Speeds);
	//set angels PWM
	Steering_set_Angles(Steering_Angles);

	//
	if(reset_distance == 1){
		reset_distance = 0;
		uint8_t all[4] = {1,1,1,1};
		CAN_reset_distance(all);
	}


	DIGITAL_IO_SetOutputLow(&CALC_TIME_INDICATOR);
}


void Servo_NP_setting(void){
	  NP[0] = NPfl;
	  NP[1] = NPfr;
	  NP[2] = NPrl;
	  NP[3] = NPrr;
}

float average(float a[],int num_vaules) {
    float sum = 0;
    int i;
    for(i = 0; i < num_vaules; i++){
        sum += a[i];
    }
    return sum / num_vaules;
}

void mode_led(uint8_t var){
	if(CHECK_BIT(var,0)){
		DIGITAL_IO_SetOutputLow(&MODE_001);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_001);
	}

	if(CHECK_BIT(var,1)){
		DIGITAL_IO_SetOutputLow(&MODE_010);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_010);
	}

	if(CHECK_BIT(var,2)){
		DIGITAL_IO_SetOutputLow(&MODE_100);
	}
	else{
		DIGITAL_IO_SetOutputHigh(&MODE_100);
	}
}
