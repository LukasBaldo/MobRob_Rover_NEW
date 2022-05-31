/*
 * main.c
 *
 *  Created on: 2022 May 24 16:56:28
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)


// ################### CONSTANTS################
#define ECHO_TIME_MIN 20
#define ECHO_TIME_MAX 2355
#define ECHO_TIME_10us_TO_DISTANCE_cm 0.17

//#################### FUNCTIONS ################
float average(float a[],int num_vaules);
void CAN_send_vaules(uint16_t distance_C_CAN, uint16_t distance_L_CAN, uint16_t distance_R_CAN );
void calc();

// ################## VARS ######################
uint8_t CAN_OK = 0;
CAN_NODE_STATUS_t init_status;

volatile uint16_t t_10us_count = 0;

volatile uint8_t pin_state_C = 0;
volatile uint32_t echo_time_C = 0;
volatile uint32_t echo_count_C = 0;
volatile uint16_t distance_C = 0;

volatile uint8_t pin_state_L = 0;
volatile uint32_t echo_time_L = 0;
volatile uint32_t echo_count_L = 0;
volatile uint16_t distance_L = 0;


volatile uint8_t pin_state_R = 0;
volatile uint32_t echo_time_R = 0;
volatile uint32_t echo_count_R = 0;
volatile uint16_t distance_R = 0;

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


void TIMER_10us_ISR(){
	t_10us_count ++;
	if (pin_state_C == 1) echo_count_C ++;
	if (pin_state_L == 1) echo_count_L ++;
	if (pin_state_R == 1) echo_count_R ++;

	if(t_10us_count == 1){
		 BUS_IO_Write(&BUS_IO_TRIGGER,0b111);
	}
	else if (t_10us_count == 11) {
		 BUS_IO_Write(&BUS_IO_TRIGGER,0b000);
	}
	else if (t_10us_count > 2500) {
		t_10us_count = 0;
		calc();
		CAN_send_vaules(distance_C, distance_L, distance_R );
	}

}



void ECHO_L_ISR(void){
	pin_state_L = PIN_INTERRUPT_GetPinValue(&ECHO_L);

	if(pin_state_L == 0){
		echo_time_L = echo_count_L;
		echo_count_L = 0;
	}

}

void ECHO_C_ISR(void){
	pin_state_C = PIN_INTERRUPT_GetPinValue(&ECHO_C);

	if(pin_state_C == 0){
		echo_time_C = echo_count_C;
		echo_count_C = 0;
	}

}

void ECHO_R_ISR(void){
	pin_state_R = PIN_INTERRUPT_GetPinValue(&ECHO_R);

	if(pin_state_R == 0){
		echo_time_R = echo_count_R;
		echo_count_R = 0;
	}

}

void calc(){
	if(echo_time_L < ECHO_TIME_MIN) echo_time_L = ECHO_TIME_MIN;
	else if(echo_time_L > ECHO_TIME_MAX) echo_time_L = ECHO_TIME_MAX;
	distance_L = echo_time_L * ECHO_TIME_10us_TO_DISTANCE_cm;

	if(echo_time_C < ECHO_TIME_MIN) echo_time_C = ECHO_TIME_MIN;
	else if(echo_time_C > ECHO_TIME_MAX) echo_time_C = ECHO_TIME_MAX;
	distance_C = echo_time_C * ECHO_TIME_10us_TO_DISTANCE_cm;

	if(echo_time_R < ECHO_TIME_MIN) echo_time_R = ECHO_TIME_MIN;
	else if(echo_time_R > ECHO_TIME_MAX) echo_time_R = ECHO_TIME_MAX;
	distance_R = echo_time_R * ECHO_TIME_10us_TO_DISTANCE_cm;
}

void CAN_send_vaules(uint16_t distance_C_CAN, uint16_t distance_L_CAN, uint16_t distance_R_CAN ){

	uint16_t RX_Data[4] ={CAN_OK, distance_C_CAN, distance_L_CAN, distance_R_CAN}; // in int16_t mm/s converter back on inverter

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[0], (uint8_t*)RX_Data);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[0]); //Transmit the data of message object1
}


float average(float a[],int num_vaules) {
    float sum = 0;
    int i;
    for(i = 0; i < num_vaules; i++){
        sum += a[i];
    }
    return sum / num_vaules;
}
