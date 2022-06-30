/*
 * PI_Controller.c
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */
#include "PI_Controller.h"

//#########################
//###### 	SET		#######
//#########################


//Vuales 14/04 with maurzio
//PID_param omega_param = {.P = 0.004, .I = 0.004, .D = 0.0, .MaxLimit =  1, .MinLimit = -1, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // limits of 2 are derived from T = K_t * I = 0.21 Nm/A * 2A = 1.47A --> 2A//PID_param Iq_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Iq_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Id_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};


// crretn best on motor 10 iq 50Hz wbw
PI_param Iq_param = {.P = 0.28, .I = 232.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};
PI_param Id_param = {.P = 0.28, .I = 232.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};

// max ested iq 100Hz wbw
//PID_param Iq_param = {.P = 0.56, .I = 465.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Id_param = {.P = 0.56, .I = 465.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};


PI_param omega_param = {.P = 0.2, .I = 0.08, .MaxLimit =  2, .MinLimit = -2, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};
void omega_param_selctor(uint8_t Motor_num){
	 switch(Motor_num)
	     {
	         case 6: // M6 wbw 3Hz
	        	  omega_param.P = 0.2859;
	        	  omega_param.I = 0.1289;
	        	break;
	         case 10: //M10 3Hz wbw
	        	 omega_param.P = 0.3949;
	        	 omega_param.I = 0.1619;
	        	 //PI_param omega_param = {.P = 0.3949, .I = 0.1619, .MaxLimit =  2, .MinLimit = -2, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; //M10 3Hz wbw
	        	 break;
	    }
}

#define T_CYC 0.0002 // = 1.0 /5000.0 Hz; //Tcyc sample correspond to control update rate to PI controler for I

// PID Controller
float PI_Controller(float SetPoint,float ActValue,PI_param *param){

	float Deviation = SetPoint - ActValue;
	float temp1, temp2;

	 temp1 = param->P*Deviation;
	 temp2 = temp1 + param->I * T_CYC * Deviation + param->Output_z;

	 if (temp2 >= param->MaxLimit){
		 temp2 = param->MaxLimit;
	 }
	 else if (temp2 < param->MinLimit){
		 temp2 = param->MinLimit;
	 }

	 param->Output_z = temp2 - temp1;

	 param->Output = temp2;

	return param->Output;

}

