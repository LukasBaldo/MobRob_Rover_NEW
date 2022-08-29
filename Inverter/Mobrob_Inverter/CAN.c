// CAN Communication
#include "DEFINE_CONST.h"
#include <DAVE.h>
#include "Globals.h"
#include "CAN.h"

#define MAX_Speed_CAN 2

int16_t Speeds_int16_r[4];

float CAN_speed_ref;
uint8_t CAN_new_meassage = 0;// if 1 recived can data
uint8_t CAN_no_com_counter = 0;

// distacne vaule back over can
float Speed_act = 0;
float distance = 0;

void CAN_RX_MO2_ISR(void){
		XMC_CAN_MO_t* lmsgobjct_ptr_1 = CAN_NODE_0.lmobj_ptr[1]->mo_ptr;
		CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[1]); // reset for incremental encoder

		uint8_t data_r[8];
		 int i;
		 for(i = 0 ; i < 8 ; i ++){
			 data_r[i] = lmsgobjct_ptr_1->can_data_byte[i];
		 }

		 for(i = 0 ; i < 4 ; i ++){
			 Speeds_int16_r[i] = (data_r[i * 2 + 1] << 8) | data_r[i * 2];
			 }

		CAN_speed_ref = (float)Speeds_int16_r[MOTOR_ON_ROVER] / 1000; // converting back from mm/s to m/s

		if(MOTOR_NUM == 6)CAN_speed_ref = -CAN_speed_ref; // exeption for motor 6

		if(CAN_speed_ref > MAX_Speed_CAN) CAN_speed_ref = MAX_Speed_CAN;
		if(CAN_speed_ref < -MAX_Speed_CAN) CAN_speed_ref = -MAX_Speed_CAN;

		CAN_new_meassage = 1;
		CAN_no_com_counter = 0;
}

//CAN RX reste distance
void CAN_RX_MO7_ISR(void){
	XMC_CAN_MO_t* lmsgobjct_ptr = CAN_NODE_0.lmobj_ptr[6]->mo_ptr;
	CAN_NODE_MO_Receive((void*) CAN_NODE_0.lmobj_ptr[6]); // reset for incremental encoder

	uint8_t data_r[4];
	 int i;
	 for(i = 0 ; i < 4 ; i ++){
		 data_r[i] = lmsgobjct_ptr->can_data_byte[i];
	 }

	 if(data_r[MOTOR_ON_ROVER] == 1) distance_180deg_ele_count = 0;
}

void CAN_TX_ISR(void){
	int16_t Inverter_state[4] = {0};

	Inverter_state[0] = MOTOR_ON_ROVER; // for identification apart form CAN ID

	//if(CAN_no_com_counter > CAN_NO_COM_TH) Inverter_state[0] = 0;
	//else Inverter_state[1] = 1;


	Inverter_state[1] = V_DC_link * 100;  // *100 send  2 deciaml places

	Speed_act = omega_mech_rps * 2 * PI * WHEEL_R;
	if(MOTOR_NUM == 6) Speed_act = -Speed_act;
	Inverter_state[2] = Speed_act * 1000; // to mm/sconverstoion

	distance = (((float)distance_180deg_ele_count / 2) / PPZ) * PI * 2 * WHEEL_R;
	if(MOTOR_NUM == 6) distance = -distance;
	Inverter_state[3] = distance * 1000; // to mm

	uint8_t CAN_MO = MOTOR_ON_ROVER + 2;
	CAN_NODE_MO_UpdateData((void*) CAN_NODE_0.lmobj_ptr[CAN_MO], (uint8_t*)Inverter_state);
	CAN_NODE_MO_Transmit((void*) CAN_NODE_0.lmobj_ptr[CAN_MO]); //Transmit the data of message object1
}
