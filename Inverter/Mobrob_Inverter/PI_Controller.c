/*
 * PI_Controller.c
 *
 *  Created on: 30 Jun 2022
 *      Author: lukas
 */
#include "PI_Controller.h"

//#############################
//###### 	SET VAULES	#######
//#############################


//Vuales 14/04 with maurzio
//PID_param omega_param = {.P = 0.004, .I = 0.004, .D = 0.0, .MaxLimit =  1, .MinLimit = -1, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // limits of 2 are derived from T = K_t * I = 0.21 Nm/A * 2A = 1.47A --> 2A//PID_param Iq_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Iq_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Id_param = {.P = 0.05, .I = 25.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};


// crretn best on motor 10 iq 50Hz wbw
//PI_param Iq_param = {.P = 0.28, .I = 232.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};
//PI_param Id_param = {.P = 0.28, .I = 232.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};

// M10 iq 75Hz wbw
PI_param Iq_param = {.P = 0.4782, .I = 329.799, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
PI_param Id_param = {.P = 0.4782, .I = 329.799, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};


// max ested iq 100Hz wbw
//PID_param Iq_param = {.P = 0.56, .I = 465.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0}; // Voltage limit to 1/2 of DC link.
//PID_param Id_param = {.P = 0.56, .I = 465.0, .D = 0.0, .MaxLimit =  100, .MinLimit = -100, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};


PI_param omega_param = {.P = 0.2, .I = 0.08, .MaxLimit =  2, .MinLimit = -2, .Output = 0.0, .Deviation_old = 0.0, .Deviation_old2 = 0.0};

void omega_param_selctor(uint8_t Motor_num){
	 switch(Motor_num)
	     {
	 	 	 case 1:
	 	 		// M1 wbw 3Hz iq 75Hz wbw
				//omega_param.P = 0.3651;
				//omega_param.I = 0.1387;

	 	 		// M1 wbw 2Hz iq 75Hz wbw
				omega_param.P = 0.2434;
				omega_param.I = 0.0925;


	 	 		 break;
	 	 	 case 2:
	 	 		omega_param.MaxLimit = 0.5;
				omega_param.MinLimit =- 0.5;

				// gets its own vaules
				Iq_param.P = 0.7421;
				Iq_param.I = 482.868;

				Id_param.P = 0.7421;
				Id_param.I = 482.868;


				//hand tunded
				omega_param.P = 0.025;
				omega_param.I = 0.035;

				 // M2 wbw 1Hz iq 75Hz wbw own iq
				//omega_param.P = 0.0173;
				//omega_param.I = 0.0916;

	 	 		 // M2 wbw 1Hz iq 75Hz wbw
				//omega_param.P = 0.01001;
				//omega_param.I = 0.018;

	 	 		 // M2 wbw 3Hz iq 75Hz wbw
				//omega_param.P = 0.2646;
				//omega_param.I = 0.0538;

				//M2 wbw 2Hz iq 75Hz wbw 0.06 to 0.09 char
				//omega_param.P = 0.1764; //0.179;
				//omega_param.I = 0.0359;

				  //M2 wbw 1Hz iq 75Hz wbw 0.06 to 0.09 char
				// omega_param.P = 0.0882; //0.179;
				 //omega_param.I = 0.0179;

				//omega_param.P = 0.01; //0.179;
				//omega_param.I = 0.01;

	 	 		//M2 wbw 0.6Hz iq 75Hz wbw 0.06 to 0.09 char
	 	 		//omega_param.P = 0.0108;
	 	 		//omega_param.I = 0.00529;

				 break;
	 	 	 case 3:
	 	 		 // M3 3Hz iq 75Hz
	 	 		omega_param.P = 0.2346;
				omega_param.I = 0.1153;
				break;
	         case 6:
	        	 // M6 wbw 3Hz
	        	//omega_param.P = 0.1; // start outweise ustabel          // 0.2859; calc value
	        	//omega_param.I = 0.1289;

	        	 // M6 wbw 3Hz iq 75Hz 0.12 0.16 char
	        	omega_param.P = 0.2906; // start outweise ustabel          // 0.2859; calc value
	        	omega_param.I = 0.1182;
	        	break;
	         case 10: //M10 3Hz wbw
	        	 //omega_param.P = 0.3949;
				 //omega_param.I = 0.1619;

				 //M10 3Hz wbw iq 75Hz wbw
				/// omega_param.P = 0.3567;
				 //omega_param.I = 0.1481;

				 //M10 5Hz wbw iq 75Hz wbw
				 //omega_param.P = 0.5946;
				 //omega_param.I = 0.2468;

				 //M10 3Hz wbw iq 75Hz wbw char rfro  0.13 to 0.18 T_ref
				 omega_param.P = 0.3667;
				 omega_param.I = 0.1841;
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

