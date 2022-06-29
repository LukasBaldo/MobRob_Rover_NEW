#ifndef GLOBAS
#define GLOBAS

#include <stdint.h> // need for data types


extern float V_DC_link;
extern float omega_ele_rads;

extern float CAN_speed_ref;
extern uint8_t CAN_new_meassage;// if 1 recived can data
extern uint8_t CAN_no_com_counter;
// distacne vaule back over can

extern int distance_180deg_ele_count;
extern float Speed_act ;
extern float distance ;



#endif /* GLOBAS */
