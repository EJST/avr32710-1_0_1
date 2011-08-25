/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Motor Control file with PWM peripheral
 *
 * This file contains the driver for Motor Control.
 *   It uses the PWM interface
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

#include <avr32/io.h>
#include "flashc.h"
#include "pm.h"
#include "cycle_counter.h"
#include "tc.h"
#include "gpio.h"
#include "intc.h"
#include "hall_estimator.h"
#include "svpwm.h"
#include "pwm_drv.h"
#include "conf_svpwm.h"
#include "mc_driver.h"


//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________

//_____ D E C L A R A T I O N S ____________________________________________

volatile unsigned short tick;                             //!< tick reference flag
volatile U8   Prescaler_Main_Tick = PRESCALER_TICK;       //!< prescaler for tick generation
volatile U16   amplitude = 0;                             //!< amplitude value for SVPWM calculation
volatile mc_motor_direction_t mc_direction;               //!< current motor direction
volatile svpwm_options_t svpwm_options;                   //!< current svpwm value
volatile hall_estimator_options_t hall_estimator_options; //!< current hall estimator value
volatile pwm_drv_options_t pwm_drv_options;               //!< current pwm value


#if __GNUC__
__attribute__((__interrupt__)) void compare_int_handler( void )
#elif __ICCAVR32__
/* TC Interrupt  */
#pragma handler = AVR32_TC_IRQ_GROUP, 1
__interrupt void compare_int_handler( void )
#endif
{	
     // clear the interrupt flag
     AVR32_TC.channel[TC_CHANNEL].sr;
     hall_estimator_update_teta(&hall_estimator_options,svpwm_options.resolution);
     svpwm_options.teta = hall_estimator_options.teta;
     // If overflow
     if (svpwm_options.teta>svpwm_options.resolution)
     {
       svpwm_options.teta = svpwm_options.resolution;
     }   
     // Prepare the next svpwm calculation
     svpwm_options.sector_number = hall_estimator_options.sector_number;
     svpwm_options.amplitude = amplitude;
     // Launch svpwm calculation
     svpwm_calc(&svpwm_options,mc_direction);
     // Update duty cycle
     pwm_drv_duty_cycle(&pwm_drv_options,svpwm_options.PWM0, svpwm_options.PWM0+DEAD_TIME, svpwm_options.PWM1, svpwm_options.PWM1+DEAD_TIME, svpwm_options.PWM2, svpwm_options.PWM2+DEAD_TIME);
     // Generate the main Tick 
     Prescaler_Main_Tick -= 1;
     if (Prescaler_Main_Tick == 0)
     {
        Prescaler_Main_Tick = PRESCALER_TICK; 
        tick=1;
     }
}
//------------------------------------------------------------------------------
/*! \name Set amplitude
 */
//! @{
void mc_set_amplitude (U8 val)
{
  amplitude = val;
}
//------------------------------------------------------------------------------
/*! \name Get amplitude
 */
//! @{
U8 mc_get_amplitude (void)
{
  return amplitude;
}
//------------------------------------------------------------------------------
/*! \name Get Hall Period
 */
//! @{
U32 mc_get_hall_period(void)
{
  hall_estimator_update_speed(&hall_estimator_options);
  return hall_estimator_options.speed;
}

//------------------------------------------------------------------------------
/*! \name Set Motor Direction
 */
//! @{
void mc_set_motor_direction(mc_motor_direction_t direction)
{
  mc_direction = direction;
}

//------------------------------------------------------------------------------
/*! \name Get Motor Direction
 */
//! @{
mc_motor_direction_t mc_get_motor_direction(void)
{
  return mc_direction;
}
//------------------------------------------------------------------------------
/*! \name Set Duty Cycle
 */
//! @{
void mc_duty_cycle(U8 duty)
{
  pwm_drv_duty_cycle(&pwm_drv_options,duty,duty,duty,duty,duty,duty);
}
//------------------------------------------------------------------------------
/*! \name Run Motor
 */
//! @{
void mc_run_motor(void)
{ 
   // enable all GPIO alternate control 
   pwm_drv_enable_pwm_fct();
   hall_estimator_do_interrupt();
   Enable_global_interrupt();   
}
//------------------------------------------------------------------------------
/*! \name Stop Motor
 */
//! @{
void mc_stop_motor(void)
{
  Disable_global_interrupt();  
  mc_duty_cycle(svpwm_options.max_frequency / 2);
  hall_estimator_options.speed = 0;  
  pwm_drv_disable_pwm_fct();   
}
//------------------------------------------------------------------------------
/*! \name Set Speed value
 */
//! @{
void mc_set_speed(U8 speed)
{
   pwm_drv_options.max_pwm_value = speed; 
   svpwm_options.max_frequency = speed;    
}
//------------------------------------------------------------------------------
/*! \name Init Tick reference counter
 */
//! @{
void mc_init_tc(void)
{
    volatile avr32_tc_t *tc = &AVR32_TC;

  // Options for waveform genration.
  static const tc_waveform_opt_t WAVEFORM_OPT =
  {
    .channel  = TC_CHANNEL,                        // Channel selection.

    .bswtrg   = TC_EVT_EFFECT_NOOP,                // Software trigger effect on TIOB.
    .beevt    = TC_EVT_EFFECT_NOOP,                // External event effect on TIOB.
    .bcpc     = TC_EVT_EFFECT_NOOP,                // RC compare effect on TIOB.
    .bcpb     = TC_EVT_EFFECT_NOOP,                // RB compare effect on TIOB.

    .aswtrg   = TC_EVT_EFFECT_NOOP,                // Software trigger effect on TIOA.
    .aeevt    = TC_EVT_EFFECT_NOOP,                // External event effect on TIOA.
    .acpc     = TC_EVT_EFFECT_NOOP,                // RC compare effect on TIOA: toggle.
    .acpa     = TC_EVT_EFFECT_NOOP,                // RA compare effect on TIOA: toggle (other possibilities are none, set and clear).

    .wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,// Waveform selection: Up mode with automatic trigger(reset) on RC compare.
    .enetrg   = FALSE,                             // External event trigger enable.
    .eevt     = 0,                                 // External event selection.
    .eevtedg  = TC_SEL_NO_EDGE,                    // External event edge selection.
    .cpcdis   = FALSE,                             // Counter disable when RC compare.
    .cpcstop  = FALSE,                             // Counter clock stopped with RC compare.

    .burst    = FALSE,                             // Burst signal selection.
    .clki     = FALSE,                             // Clock inversion.
    .tcclks   = TC_CLOCK_SOURCE_TC2                // Internal source clock 2 - connected to PBA/4
  };

  static const tc_interrupt_t TC_INTERRUPT =
  {
    .etrgs = 0,
    .ldrbs = 0,
    .ldras = 0,
    .cpcs  = 1,
    .cpbs  = 0,
    .cpas  = 0,
    .lovrs = 0,
    .covfs = 0
  };
  INTC_register_interrupt(&compare_int_handler, AVR32_TC_IRQ0, AVR32_INTC_INT1);

  // Initialize the timer/counter.
  tc_init_waveform(tc, &WAVEFORM_OPT);         // Initialize the timer/counter waveform.

  // Set the compare triggers.
  // Remember TC counter is 16-bits, so counting second is not possible.
  // We configure it to count ms.
  // We want: (1/(FPBA/4)) * RC = 1000 Hz => RC = (FPBA/4) / 1000 = 3000 to get an interrupt every 1ms
  tc_write_rc(tc, TC_CHANNEL, (FPBA_HZ/4)/FTICK_HZ);  // Set RC value.

  tc_configure_interrupts(tc, TC_CHANNEL, &TC_INTERRUPT);

  // Start the timer/counter.
  tc_start(tc, TC_CHANNEL);                    // And start the timer/counter.

}
//------------------------------------------------------------------------------
/*! \name Global Low level initialization
 */
//! @{
void mc_init_motor(void)
{   
  /* initialise the cpu count to generate a tick */
   tick = 0;
   mc_init_tc();
   pwm_drv_options.max_pwm_value = TOP_PWM;
   
   pwm_drv_init(&pwm_drv_options);
   pwm_drv_enable_pwm_channel();

   svpwm_options.resolution    = FTICK_HZ/((RESOLUTION)-1);
   svpwm_options.max_frequency = TOP_PWM;
   
   hall_estimator_options.speed = 0;
   hall_estimator_options.teta = 0;
   hall_estimator_init(&hall_estimator_options);    

   hall_estimator_init_interrupt(&hall_estimator_options);
   mc_set_amplitude (MIN_AMPLITUDE);   
   mc_set_motor_direction(MC_CW);
}
//@}

