/*
 * main.c
 *
 *  Created on: 2022 Jun 15 15:50:45
 *  Author: lukas
 */




#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

uint8_t UH = 0;
uint8_t UL = 0;

uint8_t VH = 0;
uint8_t VL = 0;

uint8_t WH = 0;
uint8_t WL = 0;

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if (status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	  if(UH == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_5);
	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_5);

	  if(UL == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_0);
	  	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_0);


	  if(VH == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_4);
	  	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_4);

	  if(VL == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_2);
	  	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_2);


	  if(WH == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_3);
	  	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_3);

	  if(WL == 1) DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_1);
	  	  else DIGITAL_IO_SetOutputLow(&DIGITAL_IO_1);

  }
}
