#ifndef GLOBAS
#define GLOBAS

//#include <stdint.h> // need for data types


//###############################################
//###########	NEED TO BE SET	#################
//###############################################
#define MOTOR_NUM 10 // for trque dirction motro 1 differtn form rest so far
#define MOTOR_ON_ROVER 0// 0 front left 1 front right 2 back left 3 back right
#define INVERTER_NUM 0 //witch board for ADC_TO_DCLINK factor version 2 boards: B3 right rear rover = 3, B2 left rear rover = 2, B5 left front rover = 0, B4 right front rover = 1,  version one borads: B10, = 4 B11 = 5,



volatile extern float V_DC_link;
volatile extern float omega_ele_rads;

volatile extern int distance_180deg_ele_count;

#endif /* GLOBAS */
