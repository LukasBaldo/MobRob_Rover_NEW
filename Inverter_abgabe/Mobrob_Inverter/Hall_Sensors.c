/*
 * Hall_Sensors.c
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */
#include "Hall_Sensors.h"

#define TIME_180DEG_TO_OMEGA_MECH 454.5455
#define TIME_OMEGA_0 5000 //time since last hall before omgea set to 0 in 100us

// Angle correction factor to align Current to Hall Sensor vuales form 14/04
float offset_pos = -27.0;				//Offset to reduce Hall Sensor Offset in CW direction
float offset_neg = -20.0;			//Offset to reduce Hall Sensor Offset in CCW direction


// Declaration of Lookup Table
int8_t lookup_table[] = {0,0,0,0,0,0,0,0,0,0,0,-1,0,1,0,0,0,0,0,1,0,0,-1,0,0,1,-1,0,0,0,0,0,0,0,0,0,0,-1,1,0,0,-1,0,0,1,0,0,0,0,0,1,0,-1,0,0,0,0,0,0,0,0,0,0,0};
//access via the index in the  form of the 6 bits (3 previous hall states and 3 actual  hall states)
//for each of the 2^6 possibilities of sequence of hall events  it has been thought about the waveforms and thus in which direction the motor has to turn!

volatile int8_t sector = 0;
volatile int8_t direction = 0;              // direction +1/-1 -> CW and CCW 0 invalid
volatile uint16_t enc_val = 0;          // old and new values of all Hall Sensors
volatile uint32_t time_since_A = 0;	// time between to edges Hall A
volatile uint32_t time_since_B = 0; // time between to edges Hall B
volatile uint32_t time_since_C = 0; // time between to edges Hall C
volatile uint32_t time_180deg = 1000;        // time which last triggered HAL sensor needed for 180°
volatile char last_Hall_trig;			//last triggered Hall Sensor A/B/C

uint32_t time_180deg_cal = 1000;
float angle_in_sector = 30;// in deg // 30 defalut becasue in the middel of the 60°


volatile float omega_mech_rps_temp = 0; //SK	//since dividing through the incremental (counted up) value of time_180deg can lead to ridiculously high velocities a plausability check is performed before temp is stored to the value



//########################
// INTERRUPTS
//########################

// 100us Timer for speed measurement
void TimeCounterISR(void){
	time_since_A ++;
	time_since_B ++;
	time_since_C ++;
}

// Hall Sensors
void HallSensor_A_ISR(void){
	HallSensor();
	time_180deg = time_since_A;				// Actual velocity value calculated with Hall A
	time_since_A = 0;						// measurement of speed -> time between change of Hall A
	last_Hall_trig = 'A';

	//Distance measurement
	if(direction == 1 ) distance_180deg_ele_count ++;
	else if(direction == -1 ) distance_180deg_ele_count --;
}
void HallSensor_B_ISR(void){
	HallSensor();
	time_180deg = time_since_B;
	time_since_B = 0;
	last_Hall_trig = 'B';
}
void HallSensor_C_ISR(void){
	HallSensor();
	time_180deg = time_since_C;
	time_since_C = 0;
	last_Hall_trig = 'C';
}


//########################
// FUNCTIONS
//########################


// Hall init -> readout of actual Hall pattern to determine the motor position
void Hall_init(void){

	  uint8_t Hall_Initial =  ((((PIN_INTERRUPT_GetPinValue(&Hall_A_ISR) << 1) | PIN_INTERRUPT_GetPinValue(&Hall_B_ISR)) << 1) | PIN_INTERRUPT_GetPinValue(&Hall_C_ISR));

	 if(Hall_Initial == 0b100){sector = 0;}
	 else if(Hall_Initial == 0b110){sector = 1;}
	 else if(Hall_Initial == 0b010){sector = 2;}
	 else if(Hall_Initial == 0b011){sector = 3;}
	 else if(Hall_Initial == 0b001){sector = 4;}
	 else if(Hall_Initial == 0b101){sector = 5;}
}

// Function to determine direction and current Sector
void HallSensor(void){
	enc_val = enc_val << 3; //shift old values and make space for new ones
	uint8_t Hall_status =((((PIN_INTERRUPT_GetPinValue(&Hall_A_ISR) << 1) | PIN_INTERRUPT_GetPinValue(&Hall_B_ISR)) << 1) | PIN_INTERRUPT_GetPinValue(&Hall_C_ISR));
	enc_val = enc_val | Hall_status; //read Sensor values and store them
	direction = lookup_table[enc_val & 0b111111];  //direction due to look up table, look at actual and prev measurement

	if(Hall_status == 0b100){sector = 0;}
		 else if(Hall_status == 0b110){sector = 1;}
		 else if(Hall_status == 0b010){sector = 2;}
		 else if(Hall_status == 0b011){sector = 3;}
		 else if(Hall_status == 0b001){sector = 4;}
		 else if(Hall_status == 0b101){sector = 5;}

}

void Hall_postion_speed_estimation(){ // called ervery 200us by mian befor control_calc()
		uint32_t t; // Actual time since last angle update via Hall Sensor
		if(last_Hall_trig == 'A') t = time_since_A;
		else if(last_Hall_trig == 'B') t = time_since_B;
		else if(last_Hall_trig == 'C') t = time_since_C;
		time_180deg_cal = time_180deg;
		// not fixing sector for calc

		if(time_180deg_cal > 2500) time_180deg_cal = 2500;
		else if(time_180deg_cal < 2) time_180deg_cal = 2;

		if(t < TIME_OMEGA_0){
			angle_in_sector = (180.0 / (float)time_180deg_cal) * t;
			if(angle_in_sector > 60) angle_in_sector = 60;
		}
		else{
			angle_in_sector = 30; // standig still
		}

		// CW
		if(direction==1)
		{
			angle_phi = (sector*60.0)+angle_in_sector;
			angle_phi=angle_phi+offset_pos;
			if(angle_phi > 360.0){angle_phi=angle_phi-360.0;}

			omega_mech_rps_temp = TIME_180DEG_TO_OMEGA_MECH / (float)time_180deg_cal;
		}
		// CCW
		else{
			angle_phi = (sector*60.0)-angle_in_sector;
			angle_phi = angle_phi+60.0;
			if(angle_phi < 0){angle_phi=angle_phi+360.0;}
			angle_phi=angle_phi+offset_neg;
			if(angle_phi > 360.0){angle_phi=angle_phi-360.0;}

			omega_mech_rps_temp = -TIME_180DEG_TO_OMEGA_MECH / (float)time_180deg_cal;
		}

		if(omega_mech_rps_temp<200 && omega_mech_rps_temp>(-200)) //plausibility check to avoid large peaks due to small time_180deg --> caused problems
		{
			omega_mech_rps = omega_mech_rps_temp;
		}

		if(t > TIME_OMEGA_0 || direction == 0){// set omega to 0 if no more hall detected or seqcane niether forward or backward
			omega_mech_rps = 0;
		}
}

