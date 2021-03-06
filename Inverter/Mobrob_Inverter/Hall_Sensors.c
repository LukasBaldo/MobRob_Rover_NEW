/*
 * Hall_Sensors.c
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */
#include "Hall_Sensors.h"
#include <DAVE.h>
#include "Globals.h"

// Declaration of Lookup Table
int8_t lookup_table[] = {0,0,0,0,0,0,0,0,0,0,0,-1,0,1,0,0,0,0,0,1,0,0,-1,0,0,1,-1,0,0,0,0,0,0,0,0,0,0,-1,1,0,0,-1,0,0,1,0,0,0,0,0,1,0,-1,0,0,0,0,0,0,0,0,0,0,0};
//access via the index in the  form of the 6 bits (3 previous hall states and 3 actual  hall states)
//for each of the 2^6 possibilities of sequence of hall events  it has been thought about the waveforms and thus in which direction the motor has to turn!

volatile int8_t sector = 0;
volatile int8_t direction = 0;              // direction +1/-1 -> CW and CCW
volatile uint16_t enc_val = 0;          // old and new values of all Hall Sensors
volatile uint32_t time_since_A = 0;	// time between to edges Hall A
volatile uint32_t time_since_B = 0; // time between to edges Hall B
volatile uint32_t time_since_C = 0; // time between to edges Hall C
volatile uint32_t time_180deg = 1000;        // time which last triggered HAL sensor needed for 180°
volatile char last_Hall_trig;			//last triggered Hall Sensor A/B/C


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

