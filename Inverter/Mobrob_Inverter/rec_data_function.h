#ifndef REC_DATA_FUNCTION
#define REC_DATA_FUNCTION

#include <stdint.h> // need for data types


void f_rec_data(float data_point_var1, float data_point_var2,float* x_ref, float start_step_from, float step_to, uint8_t rec_every_x_5kHz, uint32_t setting_time_cylcys);
void rec_step_from_0(float data_point_var1, float data_point_var2, float data_point_var3, float* x_ref, float step_to, uint8_t rec_every_x_5kHz);

#endif /* REC_DATA_FUNCTION */
