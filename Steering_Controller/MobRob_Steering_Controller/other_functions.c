/*
 * other_functions.c
 *
 *  Created on: 13 Jun 2022
 *      Author: lukas
 */

#include "other_functions.h"
/* Always include the header file that declares something
                     * in the C file that defines it. This makes sure that the
                     * declaration and definition are always in-sync.  Put this
                     * header first in foo.c to ensure the header is self-contained.
                     */


/**
 * This is the function definition.
 * It is the actual body of the function which was declared elsewhere.
 */
bool within_MAX_MIN(float num, float max, float min){
	if((num-min)*(num-max) <= 0) return 1;
	else return 0;
}


float distance_trajectory_obstacel(float a, float b, float o_x, float o_y){
	return fabs(a * o_x +  b* o_y) / sqrt(pow(a,2) + pow(b,2));
}

float pythagoras(float a, float b){
	return sqrt(pow(a,2) + pow(b,2));
}

float vector_abs_value(float a[2]){
	return sqrt(pow(a[0],2) + pow(a[1],2));
}

float Exp_moving_average(float new_value, float value, float ALPHA){
	return ((new_value * ALPHA) + ((1 - ALPHA) * value));
}

float average(float a[],int num_values) {
    float sum = 0;
    int i;
    for(i = 0; i < num_values; i++){
        sum += a[i];
    }
    return sum / num_values;
}


float vector_projection_abs(float a[2], float b[2]){
	float factor = dot_product(a,b) / dot_product(b,b);
	float a1[2] = {factor * b[0], factor * b[0]};
	return vector_abs_value(a1);
}

float dot_product(float v[2], float u[2]){
	float result = 0.0;
    for (int i = 0; i < 2; i++)
        result += v[i]*u[i];
    return result;
}
