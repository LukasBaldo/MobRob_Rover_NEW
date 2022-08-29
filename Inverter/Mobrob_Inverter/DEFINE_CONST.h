#ifndef DEFINE_CONST
#define DEFINE_CONST


#define IQ_REF_MAX 5 // max current alwous by sat check


// Define often used vaules to decrease computational effort
#define PI 3.14159265
#define TwoThird 0.66666667
#define TwoPiThird 2.0943951
#define FourPiThird 4.1887902
#define Pi180 0.01745329

//for angel to PWM_SVM app
#define ANGLE_ONE_DEGREE (46603U)

// Define Chip select Pins for Current measurement via SPI Interface
#define ChipSelect_U 0b110
#define ChipSelect_V 0b101
#define ChipSelect_W 0b011


// MOTOR COnststs
//#define J 0.0023064 //SK J = 1/2 * m_wheel * r_wheel^2 = 1/2*1.2kg*(62mm)^2
#define K_T 0.21 //SK torque constant in Nm/A
#define K_inv (15.7/1.732) // K_inv = V / m = V_DC / 2 * 2/sqrt(3) = V_DC/sqrt(3)
#define PPZ 11
#define WHEEL_R 0.0675 //0.135/2 straigt vaule other thing casued problems


#endif /* DEFINE_CONST */
