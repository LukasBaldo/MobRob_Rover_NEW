
/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2014-06-16:
 *     - Initial version<br>
 * 2015-08-28:
 *     - Added CLOCK_XMC1_Init conditionally
 * 2018-08-08:
 *     - Add creation of projectData.bak file
 * 2019-01-30:
 *     - Fix creation of projectData.bak file
 * 2019-04-29:
 *     - Make DAVE_Init() weak, the user can reimplement the function
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "DAVE.h"

/***********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/*******************************************************************************
 * @brief This function initializes the APPs Init Functions.
 *
 * @param[in]  None
 *
 * @return  DAVE_STATUS_t <BR>
 ******************************************************************************/
__WEAK DAVE_STATUS_t DAVE_Init(void)
{
  DAVE_STATUS_t init_status;
  
  init_status = DAVE_STATUS_SUCCESS;
     /** @Initialization of APPs Init Functions */
     init_status = (DAVE_STATUS_t)CLOCK_XMC1_Init(&CLOCK_XMC1_0);

  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of CAN_NODE APP instance CAN_NODE_0 */
	 init_status = (DAVE_STATUS_t)CAN_NODE_Init(&CAN_NODE_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance CAN_RX_ULTRASONIC */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&CAN_RX_ULTRASONIC); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PWM APP instance PWM_servo_fl */
	 init_status = (DAVE_STATUS_t)PWM_Init(&PWM_servo_fl); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PWM APP instance PWM_servo_fr */
	 init_status = (DAVE_STATUS_t)PWM_Init(&PWM_servo_fr); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PWM APP instance PWM_servo_rl */
	 init_status = (DAVE_STATUS_t)PWM_Init(&PWM_servo_rl); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PWM APP instance PWM_servo_rr */
	 init_status = (DAVE_STATUS_t)PWM_Init(&PWM_servo_rr); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance TIMER_10us */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&TIMER_10us); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance INTERRUPT_TIMER_10us */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&INTERRUPT_TIMER_10us); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of CAPTURE APP instance CAPTURE_RC_Steering */
	 init_status = (DAVE_STATUS_t)CAPTURE_Init(&CAPTURE_RC_Steering); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of CAPTURE APP instance CAPTURE_RC_Speed */
	 init_status = (DAVE_STATUS_t)CAPTURE_Init(&CAPTURE_RC_Speed); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance RC_no_data_LED */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&RC_no_data_LED); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance TIMER_CONTROL */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&TIMER_CONTROL); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance INTERRUPT_TIMER_CONTROL */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&INTERRUPT_TIMER_CONTROL); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PIN_INTERRUPT APP instance RC_Connected */
	 init_status = (DAVE_STATUS_t)PIN_INTERRUPT_Init(&RC_Connected); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance CALC_TIME_INDICATOR */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&CALC_TIME_INDICATOR); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance MODE_001 */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&MODE_001); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance MODE_010 */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&MODE_010); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance MODE_100 */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&MODE_100); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of WATCHDOG APP instance WATCHDOG_0 */
	 init_status = (DAVE_STATUS_t)WATCHDOG_Init(&WATCHDOG_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance WATCHDOG_LED_BLUE */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&WATCHDOG_LED_BLUE); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance CAN_RX_INVERTER */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&CAN_RX_INVERTER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of PIN_INTERRUPT APP instance RC_AUX1 */
	 init_status = (DAVE_STATUS_t)PIN_INTERRUPT_Init(&RC_AUX1); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance LED_CAN_ERROR */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LED_CAN_ERROR); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of DIGITAL_IO APP instance LED_COLLISION_AVOID */
	 init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LED_COLLISION_AVOID); 
   }  
  return init_status;
} /**  End of function DAVE_Init */

