/*
 * main.c
 *
 *  Created on: 2022 Jul 01 21:38:44
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

uint8_t CAN_OK = 0;
CAN_NODE_STATUS_t init_status;


volatile uint8_t echo_state = 0;
volatile uint8_t wait_echo = 0;
volatile uint8_t echo_count_control = 0;

volatile uint32_t t_10us_count = 0;
volatile uint32_t echo_t_10us = 0;
volatile uint32_t echo_time = 0;
volatile uint16_t distance = 0;

#define NUM_SENSORS  3
#define ECHO_TIME_MIN 20
#define ECHO_TIME_MAX 2355
#define ECHO_TIME_10us_TO_DISTANCE_cm 0.17

enum SENSORS{
	CENTER = 0b001,
	LEFT = 0b010,
	RIGHT = 0b100};
volatile enum SENSORS Now_Sensor = CENTER;

void reset();
void CAN_send_vaules(uint16_t distance_C_CAN, uint16_t distance_L_CAN, uint16_t distance_R_CAN );
bool within_MAX_MIN_check_int(int x, int MAX, int MIN);

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
	t_10us_count ++;
	if (echo_count_control == 1) echo_t_10us ++;

	if(t_10us_count == 1)BUS_IO_Write(&BUS_IO_0,Now_Sensor);  // set trigger high

	else if (t_10us_count == 5) {
			if(PIN_INTERRUPT_GetPinValue(&ECHO_0) != 0) reset(); // check if echo low
			else wait_echo = 1;
	}

	else if (t_10us_count == 11) BUS_IO_Write(&BUS_IO_0,0b000); // set tigger low




	else if (t_10us_count > 2500) reset();//no ech in expüecte time error start again

}

void reset(){
	t_10us_count = 0;
	wait_echo = 0;
	echo_count_control = 0;
	echo_t_10us = 0;
	Now_Sensor = Now_Sensor << 1;
	if(Now_Sensor > NUM_SENSORS) Now_Sensor = CENTER;
}

void ISR_ECHO_0(void){
	echo_state = PIN_INTERRUPT_GetPinValue(&ECHO_0);

	if(wait_echo == 1 && echo_state == 1) echo_count_control = 1;

	else if(echo_state == 0 && echo_count_control == 1){
		echo_time = echo_t_10us;
		//echo_count_control = 0; not need
		if(within_MAX_MIN_check_int(echo_time, ECHO_TIME_MAX, ECHO_TIME_MIN)){

			distance = echo_time * ECHO_TIME_10us_TO_DISTANCE_cm;

			CAN_send_vaules(distance,0,0);
		}

		reset();
	}

}


bool within_MAX_MIN_check_int(int x, int MAX, int MIN)
{
	if((x - MIN) * (x - MAX) <= 0) return 1;
	else return 0;
}

void CAN_send_vaules(uint16_t distance_C_CAN, uint16_t distance_L_CAN, uint16_t distance_R_CAN ){

	uint16_t RX_Data[4] ={CAN_OK, distance_C_CAN, distance_L_CAN, distance_R_CAN}; // in int16_t mm/s converter back on inverter

	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[0], (uint8_t*)RX_Data);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[0]); //Transmit the data of message object1
}
