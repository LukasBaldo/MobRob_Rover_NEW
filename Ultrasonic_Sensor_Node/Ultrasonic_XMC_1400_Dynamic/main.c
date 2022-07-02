/*
 * main.c
 *
 *  Created on: 2022 Jul 01 21:38:44
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)

uint8_t CAN_OK = 0;
CAN_NODE_STATUS_t init_status;


uint16_t RESET_IN_10us_AFTER_SUCSSES = 10;
uint16_t RESET_IN_10us_AFTER_ECHO_LOW_FAIL = 500;
#define MAX_RESET 2500 // 40Hz
uint16_t MIN_RESET = 100; // 250Hz

#define NUM_SENSORS  0b100
#define ECHO_TIME_MIN 10 // limit 1.7cm
#define ECHO_TIME_MAX 2400 // limit 408cm
#define ECHO_TIME_10us_TO_DISTANCE_cm 0.17

volatile uint8_t echo_state = 0;
volatile uint8_t wait_echo = 0;
volatile uint8_t echo_count_control = 0;
volatile uint8_t reset_done = 1;

volatile uint32_t t_10us_count = 0;
volatile uint32_t reset_at = MAX_RESET;
volatile uint32_t echo_t_10us = 0;
volatile uint32_t echo_time = 0;

volatile uint16_t distance_C = 0; //in cm
volatile uint16_t distance_L = 0; //in cm
volatile uint16_t distance_R = 0; //in cm


//diagnostics
volatile uint32_t restet_count = 0;
volatile uint32_t sample_count = 0;
volatile uint32_t sample_rate_Hz = 0;
volatile uint32_t failed_count = 0;
volatile uint32_t echo_not_low_count = 0;
volatile uint32_t echo_not_low_count_1 = 0;
volatile uint32_t limit_fail_num = 0;
volatile uint32_t limit_fail_count = 0;



enum SENSORS{
	CENTER = 0b001,
	LEFT = 0b010,
	RIGHT = 0b100};
volatile enum SENSORS Now_Sensor = CENTER;

void reset();
void CAN_send_values();
bool within_MAX_MIN_check_int(int x, int MAX, int MIN);
uint16_t saturation_to_MIN_MAX(uint16_t value, uint16_t MAX, uint16_t MIN);

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

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {


  }
}


void ISR_10us_TIMER(){
	if(reset_done == 1){ // make sure its finished
		t_10us_count ++;
		if (echo_count_control == 1) echo_t_10us ++;

		if(t_10us_count == 1)BUS_IO_Write(&BUS_IO_0,Now_Sensor);  // set trigger high
		else if (t_10us_count == 11) BUS_IO_Write(&BUS_IO_0,0b000); // set tigger low

		else if (t_10us_count == 20) {
				switch(Now_Sensor){
					case CENTER:
						echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_C);
						break;
					case LEFT:
						echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_L);
						break;
					case RIGHT:
						echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_R);
						break;
				}
				if(echo_state != 0) {
					echo_not_low_count ++ ;
					reset_at = t_10us_count + RESET_IN_10us_AFTER_ECHO_LOW_FAIL; // reset(); // check if echo low
					reset_at = saturation_to_MIN_MAX(reset_at,MAX_RESET,MIN_RESET);
				}
				else wait_echo = 1;
			}

		else if (t_10us_count > reset_at) reset();//no ech in expüecte time error start again // 25000us 40Hz
	}
}

void reset(){
	reset_done = 0;

	// set values values back
	t_10us_count = 0;
	wait_echo = 0;
	echo_count_control = 0;
	echo_t_10us = 0;
	reset_at = MAX_RESET;
	//distance = 0;

	//switch to next sensor
	Now_Sensor = Now_Sensor << 1;
	if(Now_Sensor > NUM_SENSORS){
		Now_Sensor = CENTER;
		CAN_send_values(); //send results
	}
	// sampelr rater counter
	restet_count++;

	reset_done = 1;
}

void ISR_ECHO_C(void){
	echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_C);
	if(wait_echo == 1 && echo_state == 1) echo_count_control = 1;
	else if(echo_state == 0 && echo_count_control == 1){
		echo_time = echo_t_10us;
		if(within_MAX_MIN_check_int(echo_time, ECHO_TIME_MAX, ECHO_TIME_MIN)){
			distance_C = echo_time * ECHO_TIME_10us_TO_DISTANCE_cm;
			sample_count++; // sampelr rater counter
		}
		else limit_fail_count ++;
		 reset_at = t_10us_count + RESET_IN_10us_AFTER_SUCSSES;
		 reset_at = saturation_to_MIN_MAX(reset_at,MAX_RESET,MIN_RESET);
	}
}

void ISR_ECHO_L(void){
	echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_L);
	if(wait_echo == 1 && echo_state == 1) echo_count_control = 1;
	else if(echo_state == 0 && echo_count_control == 1){
		echo_time = echo_t_10us;
		if(within_MAX_MIN_check_int(echo_time, ECHO_TIME_MAX, ECHO_TIME_MIN)){
			distance_L = echo_time * ECHO_TIME_10us_TO_DISTANCE_cm;
			sample_count++; // sampelr rater counter
		}
		else limit_fail_count ++;
		 reset_at = t_10us_count + RESET_IN_10us_AFTER_SUCSSES;
		 reset_at = saturation_to_MIN_MAX(reset_at,MAX_RESET,MIN_RESET);
	}
}

void ISR_ECHO_R(void){
	echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_R);
	if(wait_echo == 1 && echo_state == 1) echo_count_control = 1;
	else if(echo_state == 0 && echo_count_control == 1){
		echo_time = echo_t_10us;
		if(within_MAX_MIN_check_int(echo_time, ECHO_TIME_MAX, ECHO_TIME_MIN)){
			distance_R = echo_time * ECHO_TIME_10us_TO_DISTANCE_cm;
			sample_count++; // sampelr rater counter
		}
		else limit_fail_count ++;
		 reset_at = t_10us_count + RESET_IN_10us_AFTER_SUCSSES;
		 reset_at = saturation_to_MIN_MAX(reset_at,MAX_RESET,MIN_RESET);
	}
}


void ISR_1s_TIMER(){ //for diagnostics
	sample_rate_Hz = sample_count;
	if(restet_count > sample_count) failed_count = restet_count - sample_count;
	else failed_count = 0;
	echo_not_low_count_1 = echo_not_low_count;
	limit_fail_num = limit_fail_count;
	sample_count = 0;
	restet_count = 0;
	limit_fail_count = 0;
	echo_not_low_count = 0;
}


bool within_MAX_MIN_check_int(int x, int MAX, int MIN){
	if((x - MIN) * (x - MAX) <= 0) return 1;
	else return 0;
}

uint16_t saturation_to_MIN_MAX(uint16_t value, uint16_t MAX, uint16_t MIN){
	if(value < MIN) return  MIN;
	else if(value > MAX) return MAX;
	else return value;
}

void CAN_send_values(){
	uint16_t RX_Data[4] ={CAN_OK, distance_C, distance_L, distance_R}; // in int16_t mm/s converter back on inverter

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[0], (uint8_t*)RX_Data);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[0]); //Transmit the data of message object1

	// reset
	distance_C = 0;
	//distance_L = 0;
	//distance_R = 0;
}
