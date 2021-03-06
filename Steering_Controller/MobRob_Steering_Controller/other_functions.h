#ifndef other_functions    /* This is an "include guard" */
#define other_functions   /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/
#include <stdbool.h>
#include <math.h>

bool within_MAX_MIN(float value, float max, float min);
float distance_trajectory_obstacel(float a, float b, float o_x, float o_y);
float pythagoras(float a, float b);
float vector_abs_value(float a[2]);
float Exp_moving_average(float new_value, float value, float ALPHA);
float average(float a[],int num_values);
float vector_projection_abs(float a[2], float b[2]);
float dot_product(float v[2], float u[2]);

#endif /* other_functions */
