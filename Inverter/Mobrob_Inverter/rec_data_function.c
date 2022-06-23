/*
 * rec_data_function.c
 *
 *  Created on: 23 Jun 2022
 *      Author: lukas
 */

#include "rec_data_function.h"
//data rec
uint8_t rec_data_contorl = 0;// 0 noiting 1 record
#define REC_DATA_NUM_POINTS 1000
uint16_t rec_data_index = 0;
float rec_data[REC_DATA_NUM_POINTS] = {0};
float rec_data2[REC_DATA_NUM_POINTS] = {0};



uint8_t reset_rec = 0;
uint8_t start_rec = 0;
uint32_t start_rec_copunt = 0;


void f_rec_data(float data_point_var1, float data_point_var2,float* x_ref, float start_step_from, float step_to){
	  if(start_rec  == 1){
		  start_rec_copunt ++;

		  if(start_rec_copunt == 1){
			  *x_ref = start_step_from;
		  }

		  if(start_rec_copunt ==10000){
			  rec_data_contorl = 1;
		  }


		  if(start_rec_copunt == 10099){ // step
			  *x_ref = step_to;
		  }

		  if(start_rec_copunt == 20000){
			  *x_ref = 0;
			  start_rec = 0;
		  }
	  }


	if(rec_data_contorl == 1){
		  if(rec_data_index < REC_DATA_NUM_POINTS){
				rec_data[rec_data_index] = data_point_var1;
				rec_data2[rec_data_index] = data_point_var2;
				rec_data_index ++;
		  }
	}

	if(reset_rec == 1 && start_rec == 0){
		rec_data_contorl = 0;
		reset_rec = 0;
		start_rec_copunt = 0;
		rec_data_index = 0;
	}
}
