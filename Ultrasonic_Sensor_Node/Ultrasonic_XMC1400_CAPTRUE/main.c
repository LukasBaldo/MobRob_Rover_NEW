/*
 * main.c
 *
 *  Created on: 2022 May 31 16:22:54
 *  Author: lukas
 */
#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)

#define ECHO_TIME_ns_TO_DISTANCE_cm 0.000017
#define ECHO_TIME_MIN 200000
#define ECHO_TIME_MAX 20000000

uint8_t CAN_OK = 0;
CAN_NODE_STATUS_t init_status;

enum SENSORS{
	CENTER = 0,
	LEFT = 1,
	RIGHT = 2};
volatile enum SENSORS Sensor = CENTER;

volatile uint16_t t_100us_count = 0;

uint32_t captured_time = 0;

uint8_t flag_C = 0;
uint8_t flag_L = 0;
uint8_t flag_R = 0;

uint16_t distance_C = 0;
uint32_t echo_time_C = 0;

uint16_t distance_L = 0;
uint32_t echo_time_L = 0;

uint16_t distance_R = 0;
uint32_t echo_time_R = 0;

void Capture_read();
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

void TIMER_100us_ISR(){
	t_100us_count ++;

	if(t_100us_count == 1){
		switch(Sensor)
		{
			case CENTER:
				CAPTURE_Start(&CAPTURE_C);
				BUS_IO_Write(&BUS_IO_TRIGGER,0b001);
				break;
			case LEFT:
				CAPTURE_Start(&CAPTURE_L);
				BUS_IO_Write(&BUS_IO_TRIGGER,0b010);
				break;
			case RIGHT:
				CAPTURE_Start(&CAPTURE_R);
				BUS_IO_Write(&BUS_IO_TRIGGER,0b100);
				break;
			default:
				break;
		}
	}
	else if (t_100us_count == 2) {
		 BUS_IO_Write(&BUS_IO_TRIGGER,0b000);
	}
	else if (t_100us_count == 250) { // runnig at 35 Hz no probelm with long distasnce anymore
		Capture_read();
		t_100us_count = 0;

		Sensor++;
		if(Sensor > 2){
			Sensor = 0;
			CAN_send_vaules(distance_C, distance_L, distance_R );
		}
	}
}

void Capture_read(){
	switch(Sensor)
	{
		case CENTER:
			if(CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_C, &captured_time) == CAPTURE_STATUS_SUCCESS){
				echo_time_C = captured_time;
				flag_C = 0;

				if(within_MAX_MIN_check_int(echo_time_C, ECHO_TIME_MAX, ECHO_TIME_MIN))	distance_C = echo_time_C * ECHO_TIME_ns_TO_DISTANCE_cm;
				else distance_C = 0;
			}
			else{
				flag_C = 1;
				distance_C = 0;
			}
			break;
			CAPTURE_Stop(&CAPTURE_C);
		case LEFT:
			if(CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_L, &captured_time) == CAPTURE_STATUS_SUCCESS){
					echo_time_L = captured_time;
					flag_L = 0;

					if(within_MAX_MIN_check_int(echo_time_L, ECHO_TIME_MAX, ECHO_TIME_MIN))	distance_L = echo_time_L * ECHO_TIME_ns_TO_DISTANCE_cm;
					else distance_L = 0;
				}
				else{
					flag_L = 1;
					distance_L = 0;
				}
			break;
			CAPTURE_Stop(&CAPTURE_L);
		case RIGHT:
			if(CAPTURE_GetCapturedTimeInNanoSec(&CAPTURE_R, &captured_time) == CAPTURE_STATUS_SUCCESS){
					echo_time_R = captured_time;
					flag_R = 0;

					if(within_MAX_MIN_check_int(echo_time_R, ECHO_TIME_MAX, ECHO_TIME_MIN))	distance_R = echo_time_R * ECHO_TIME_ns_TO_DISTANCE_cm;
					else distance_R = 0;
				}
				else{
					flag_R = 1;
					distance_R = 0;
				}
			CAPTURE_Stop(&CAPTURE_R);
			break;
		default:
			break;
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

