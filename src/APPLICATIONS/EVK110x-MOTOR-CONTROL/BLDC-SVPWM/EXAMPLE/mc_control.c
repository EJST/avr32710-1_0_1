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
//_____  I N C L U D E S ___________________________________________________
#include "compiler.h"
#include "mc_control.h"
#include "mc_driver.h"
#include "conf_svpwm.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________
#define OPEN_LOOP    0
#define SPEED_LOOP   1
#define CURRENT_LOOP 2

//_____ D E C L A R A T I O N S ____________________________________________
extern volatile unsigned short tick;  //!< Tick reference state

volatile U8 duty_cycle = 0;           //!<Parameter to set PWM Duty Cycle after regulation calculation
volatile U8 loop_type = SPEED_LOOP;   //!< Define the type of regulation (OPEN_LOOP or CLOSE_LOOP)

// Speed regulation variables 
volatile S32 speed_error = 0;         //!<Error calculation
volatile S32 amplitude_error = 0;     //!<Error calculation
volatile S32 last_speed_error = 0;    //!<Variable for the last error
volatile S32 speed_integral = 0;      //!<Error calculation
volatile S32 speed_integ = 0;         //!<Error calculation
volatile S32 speed_derivative= 0;     //!<Error calculation
volatile S32 speed_der = 0;           //!<Error calculation
volatile S32 speed_proportional = 0;  //!<Error calculation
volatile U8 speed_reference = 150;    //!<Speed reference value

volatile mc_motor_state_t motor_state=STOP; //!<Current Motor State value

U8 mc_get_motor_measured_speed(void)
{

  if (motor_state==RUN)
  {
    if (mc_get_hall_period()==1)
    {
      return 0;
    }
    else
    {
      return (U8)((FCPU_HZ/(4*2*7000)*60*255*n_SAMPLE)/mc_get_hall_period());
    }
  }
  else 
  {
      return 0;
  }
}
void mc_set_reference_speed(U8 speed)
{
  speed_reference = speed;
}
U8 mc_get_reference_speed(void)
{
  return speed_reference;
}

U8 mc_control_speed(U8 speed_cmd)
{
  U8 Duty = 0;
  S32 increment = 0;

  // Error calculation
  speed_error = speed_cmd - mc_get_motor_measured_speed();

  // proportional term calculation
  speed_proportional = Kp_speed*speed_error;

  // integral term calculation
  speed_integral = speed_integral + speed_error;

  if(speed_integral >  255) speed_integral =  255;
  if(speed_integral < -255) speed_integral = -255;

  speed_integ = Ki_speed*speed_integral;

  speed_derivative = speed_error - last_speed_error;

  if(speed_derivative >  255) speed_derivative =  255;
  if(speed_derivative < -255) speed_derivative = -255;

  speed_der = Kd_speed*speed_derivative;

  last_speed_error = speed_error;
  
  // Duty Cycle calculation
  increment = speed_proportional + speed_integ;
  increment += speed_der;
  increment = increment >> K_speed_scal;

  // Variable saturation
  if(increment >= (S16)(MAX_AMPLITUDE)) Duty = MAX_AMPLITUDE;
  else
  {
    if(increment <= (S16)(0)) Duty =   1;
    else Duty = (U8)(increment);
  }
  // return Duty Cycle
  return (((Duty)));
}

volatile unsigned long g_regulation_period =0;

void mc_regulation_task()
{
 if (tick) {
      tick = 0;
      g_regulation_period++;
      if (g_regulation_period>=PRESCALER_REGULATION_LOOP)
      {
          g_regulation_period=0;    
          mc_set_amplitude(mc_control_speed(mc_get_reference_speed()));
      }
  }
}

void mc_set_open_loop(){loop_type = OPEN_LOOP;}

void mc_set_speed_loop(){loop_type = SPEED_LOOP;}

void mc_run()
{
   mc_run_motor();
   motor_state = RUN;
}

void mc_stop()
{
  mc_stop_motor();
  motor_state = STOP;
}

void mc_forward()
{
  mc_set_motor_direction(MC_CW);  
  mc_run();
}

void mc_backward()
{
  mc_set_motor_direction(MC_CCW);   
  mc_run();
}

U8 mc_get_direction(void) 
{
  return (U8)(mc_get_motor_direction());
}
U8 mc_get_run_stop(void)
{
  return (U8)(motor_state);
}
