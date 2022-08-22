#ifndef GLOBAS
#define GLOBAS

//#include <stdint.h> // need for data types


//###############################################
//###########	NEED TO BE SET	#################
//###############################################
#define MOTOR_NUM 6   //for trque dirction and omegqparam
#define MOTOR_ON_ROVER 1// 0 front left 1 front right 2 back left 3 back right

volatile extern float V_DC_link;

volatile extern int distance_180deg_ele_count;

volatile extern float angle_phi;
volatile extern float omega_mech_rps;


#endif /* GLOBAS */
