#ifndef CAN_STUFF
#define CAN_STUFF

#include <stdint.h> // need for data types

#include "DEFINE_CONST.h"
#include <DAVE.h>
#include "Globals.h"

#define CAN_NO_COM_TH 250

extern float CAN_speed_ref;
extern uint8_t CAN_new_meassage;// if 1 recived can data
extern uint8_t CAN_no_com_counter;
// distacne vaule back over can

void CAN_send(void);






#endif /* CAN_STUFF */