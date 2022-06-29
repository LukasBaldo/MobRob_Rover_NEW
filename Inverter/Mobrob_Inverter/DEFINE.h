#ifndef DEFINE
#define DEFINE

// NEED to be set
//motor
#define MOTOR_NUM 2 // for trque dirction motro 1 differtn form rest so far
#define MOTOR_ON_ROVER 0// 0 front left 1 front right 2 back left 3 back right
//#define INVERTER_NUM 0 //witch board for ADC_TO_DCLINK factor version 2 boards: B3 right rear rover = 3, B2 left rear rover = 2, B5 left front rover = 0, B4 right front rover = 1,  version one borads: B10, = 4 B11 = 5,

#define MAX_Speed_CAN 2
#define CAN_NO_COM_TH 250

#define PPZ 11
#define WHEEL_R 0.0675 //0.135/2 straigt vaule other thing casued problems

#define PI 3.14159265

#endif /* DEFINE */
