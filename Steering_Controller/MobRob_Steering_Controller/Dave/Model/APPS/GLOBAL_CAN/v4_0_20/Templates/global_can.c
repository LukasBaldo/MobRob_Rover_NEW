/**
 * @file global_can.c
 * @date 2015-06-30
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * GLOBAL_CAN v4.0.20 - Configures global resources of MultiCAN module.
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
 ************************************************************************************************************************
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial version<br>
 *
 * 2015-05-19:
 *     - XMC_ASSERT debug statement is added in all API's.<br>
 *
 * 2015-06-18:
 *     - LLD version change is removed.<br>
 *
 * 2015-06-30:
 *     - GLOBAL_CAN_Enable() and GLOBAL_CAN_Disable() APIs are removed.<br>
 *
 * 2018-06-21:
 *     - Change GLOBAL_CAN_Init() to use  XMC_CAN_InitEx() which avoids using fractional divider
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "global_can.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/* API to retrieve the version of the GLOBAL_CAN APP  */
DAVE_APP_VERSION_t GLOBAL_CAN_GetAppVersion()
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t)GLOBAL_CAN_MAJOR_VERSION;
  version.minor = (uint8_t)GLOBAL_CAN_MINOR_VERSION;
  version.patch = (uint8_t)GLOBAL_CAN_PATCH_VERSION;

  return (version);
}

/*  Function to initialize the CAN Peripheral module clock.  */
GLOBAL_CAN_STATUS_t GLOBAL_CAN_Init(GLOBAL_CAN_t *handle)
{
  GLOBAL_CAN_STATUS_t status = GLOBAL_CAN_STATUS_SUCCESS;

  XMC_ASSERT("GLOBAL_CAN_Init: handle null", handle != NULL);

  if (handle->init_status != true)
  {
#if defined(MULTICAN_PLUS)
    XMC_CAN_InitEx(handle->canglobal_ptr, (XMC_CAN_CANCLKSRC_t)handle->can_clock_src, handle->can_frequency);
#else
    XMC_CAN_InitEx(handle->canglobal_ptr, XMC_CAN_CANCLKSRC_FPERI, handle->can_frequency);
#endif
    handle->init_status = true;
    status = GLOBAL_CAN_STATUS_SUCCESS;
  }
  return (status);

}

