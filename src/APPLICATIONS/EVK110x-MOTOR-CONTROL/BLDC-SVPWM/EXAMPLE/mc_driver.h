/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Motor Control file.
 *
 * This file contains the definition for Motor Control.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:            AVR32 - Using the AVR32 FAT
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 ***************************************************************************/

/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _MC_DRIVER_H_
#define _MC_DRIVER_H_
#include "compiler.h"

extern volatile U8 hall_state;

//! Motor Step Value.
typedef enum {
  HS_001=1,
  HS_010=2,
  HS_011=3,
  HS_100=4,
  HS_101=5,
  HS_110=6,
  HS_ERR=0
} mc_motor_step_t;

//! Motor Direction value.
typedef enum {
  MC_CW=0,
  MC_CCW=1
} mc_motor_direction_t;

/*! \brief Get HALL Period
 */
extern U32 mc_get_hall_period(void);
/*! \brief Set Motor Direction
 * \param direction Motor direction to reach.
 */
extern void mc_set_motor_direction(mc_motor_direction_t direction);
/*! \brief Get Motor Direction 
 * \retval mc_motor_direction_t Motor Direction
 */
extern mc_motor_direction_t mc_get_motor_direction(void);
/*! \brief Set Duty Cycle
 * \param duty Duty Cycle Value.
 */
extern void mc_duty_cycle(U8 duty);
/*! \brief Run Motor
 */
extern void mc_run_motor(void);
/*! \brief Init Motor
 */
extern void mc_init_motor(void);
/*! \brief Stop Motor
 */
extern void mc_stop_motor(void);
/*! \brief Set Amplitude
 * \param val Value of Amplitude
 */
extern void mc_set_amplitude (U8 val);
/*! \brief Set Speed Reference Value
 * \param speed Speed Value to reach
 */
extern void mc_set_speed (U8 speed);
/*! \brief Get Amplitude
 * \retval U8 amplitude value
 */
extern U8 mc_get_amplitude (void);
#endif
