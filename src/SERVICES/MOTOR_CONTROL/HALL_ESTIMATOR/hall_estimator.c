/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief HALL ESTIMATOR for AVR32 UC3.
 *
 * AVR32 HALL ESTIMATOR module.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a TC module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

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
#include "board.h"
#include "compiler.h"
#include "intc.h"
#include "gpio.h"
#include "cycle_counter.h"
#include "hall_estimator.h"
#include "conf_svpwm.h"

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________

#define ADVANCE         0  //!<  Advance in angle rotor algorithm/ hardware dependant - so it's constant */

//_____ D E C L A R A T I O N S ____________________________________________

U8 volatile hall_state = 0;          //!<  current hall state value
volatile U32 filt_hall_period = 1;   //!< filtered hall period
volatile U32 average=0;              //!<  average hall period
volatile U32 hall_ti;                //!<  last hall period value
volatile U32 count = 0;              //!<  average hall period

volatile U32 tab[n_SAMPLE];         //!<  filtered hall period buffer
volatile U32 i=0;                   //!<  index of filtered hall period buffer

volatile U8   Hall_Sector = HS_011 ;      //!< current Hall sector
volatile U8   Mem_Hall_Sector = HS_011 ;  //!< future Hall sector
volatile U8   Sensor_Counter_Up = 0;      //!< measure of the time
volatile U8   Sensor_Counter_Down = 1;    //!< current counter for svpwm
volatile U8   Top_Counter = 1;            //!< current top counter
volatile U8   Mem_Top_Counter = 1;        //!< memorization at top
volatile U16  Mem_Top_CounterX8 = 1;      //!< for filter
volatile U8   Delay_For_Change = 1;       //!< delay

#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
__interrupt
#endif
static void hall_int_handler( void )
{	
   U32 hall_value;
   U32 hall_tj;
   U32 hall_period;
   
  if (gpio_get_pin_interrupt_flag(HALL_1_PIN)) 
  {
   
   // determine hall period
   hall_tj= Get_sys_count();
   hall_period = hall_tj - hall_ti;
   hall_ti = hall_tj; // arm for next period   

   //Filtering
   average -= tab[i];   
   tab[i]   = hall_period;
   average += tab[i];
   filt_hall_period = average;
   i++;
   if (i==n_SAMPLE) i=0;
   
    gpio_clear_pin_interrupt_flag(HALL_1_PIN);
  }
  
  if (gpio_get_pin_interrupt_flag(HALL_2_PIN)) 
  {   
    gpio_clear_pin_interrupt_flag(HALL_2_PIN);
  }  
  
  if (gpio_get_pin_interrupt_flag(HALL_3_PIN)) 
  {
    gpio_clear_pin_interrupt_flag(HALL_3_PIN);
  }

    
   if ( Sensor_Counter_Up > 3 )
   {

      if (Delay_For_Change != 0)
      {
         // Last Delay not finished : Force Change_Sector 
         Top_Counter = Mem_Top_Counter;
         if (Top_Counter == 0) Top_Counter++; // to avoid "divide by 0" 

         Sensor_Counter_Down = Mem_Top_Counter;
         Hall_Sector = Mem_Hall_Sector;
         // end Change_Sector 
      }

      // Filter and memorize the Conuter_Up in the Top_Counter 
      Mem_Top_CounterX8 = (((U16)7 * Mem_Top_CounterX8) + (U16)8 * Sensor_Counter_Up) >>3;
      Mem_Top_Counter = Mem_Top_CounterX8 >> 3;

      Sensor_Counter_Up = 0; // reset the Counter_Up 

      // Get the status of the 3 Hall sensors 
      hall_value = HALL_PORT.pvr & (1<<HALL_3 | 1<<HALL_2 | 1<<HALL_1); 

      switch (hall_value) {
         case (0<<HALL_3 | 1<<HALL_2 | 0<<HALL_1):
            hall_state = HS_010;
            break;
         case (1<<HALL_3 | 1<<HALL_2 | 0<<HALL_1):
            hall_state = HS_110;
            break;
         case (1<<HALL_3 | 0<<HALL_2 | 0<<HALL_1):
            hall_state = HS_100;
            break;
         case (1<<HALL_3 | 0<<HALL_2 | 1<<HALL_1):
            hall_state = HS_101;
            break;
         case (0<<HALL_3 | 0<<HALL_2 | 1<<HALL_1):
            hall_state = HS_001;
            break;
         case (0<<HALL_3 | 1<<HALL_2 | 1<<HALL_1):
            hall_state = HS_011;
            break;
         default:
            hall_state = HS_ERR;
       }
        Mem_Hall_Sector = hall_state;

      // arm the Delay_For_Change 
      Delay_For_Change = Mem_Top_Counter - ADVANCE;

   } // else false interrupt 
}
void hall_estimator_do_interrupt(void)
{
   U32 hall_value;
    
   if ( Sensor_Counter_Up > 3 )
   {

      if (Delay_For_Change != 0)
      {
         /* Last Delay not finished : Force Change_Sector */
         Top_Counter = Mem_Top_Counter;
         if (Top_Counter == 0) Top_Counter++; /* to avoid "divide by 0" */

         Sensor_Counter_Down = Mem_Top_Counter;
         Hall_Sector = Mem_Hall_Sector;
         /* end Change_Sector */
      }

      /* Filter and memorize the Conuter_Up in the Top_Counter */
      Mem_Top_CounterX8 = (((U16)7 * Mem_Top_CounterX8) + (U16)8 * Sensor_Counter_Up) >>3;
      Mem_Top_Counter = Mem_Top_CounterX8 >> 3;

      Sensor_Counter_Up = 0; /* reset the Counter_Up */

      /* Get the status of the 3 Hall sensors */
      hall_value = HALL_PORT.pvr & (1<<HALL_3 | 1<<HALL_2 | 1<<HALL_1); // get hall sensor position

      switch (hall_value) {
         case (0<<HALL_3 | 1<<HALL_2 | 0<<HALL_1):
            hall_state = HS_010;
            break;
         case (1<<HALL_3 | 1<<HALL_2 | 0<<HALL_1):
            hall_state = HS_110;
            break;
         case (1<<HALL_3 | 0<<HALL_2 | 0<<HALL_1):
            hall_state = HS_100;
            break;
         case (1<<HALL_3 | 0<<HALL_2 | 1<<HALL_1):
            hall_state = HS_101;
            break;
         case (0<<HALL_3 | 0<<HALL_2 | 1<<HALL_1):
            hall_state = HS_001;
            break;
         case (0<<HALL_3 | 1<<HALL_2 | 1<<HALL_1):
            hall_state = HS_011;
            break;
         default:
            hall_state = HS_ERR;
       }
        Mem_Hall_Sector = hall_state;

      /* arm the Delay_For_Change */
      Delay_For_Change = Mem_Top_Counter - ADVANCE;

   } /* else false interrupt */  
}
//------------------------------------------------------------------------------
/*! \name Initialization function
 */
//! @{
void hall_estimator_init(volatile hall_estimator_options_t *hall_estimator_options)
{
    gpio_enable_pin_pull_up(HALL_1_PIN );	// HALL_1_PIN
    gpio_enable_pin_pull_up(HALL_2_PIN );	// HALL_2_PIN
    gpio_enable_pin_pull_up(HALL_3_PIN );	// HALL_3_PIN    
}

//------------------------------------------------------------------------------
/*! \name Interrupt intitialization function
 */
//! @{
void hall_estimator_init_interrupt(volatile hall_estimator_options_t *hall_estimator_options)
{
    gpio_enable_pin_interrupt(HALL_1_PIN , GPIO_PIN_CHANGE);	// HALL_1_PIN
    gpio_enable_pin_interrupt(HALL_2_PIN , GPIO_PIN_CHANGE);	// HALL_2_PIN  
    gpio_enable_pin_interrupt(HALL_3_PIN , GPIO_PIN_CHANGE);	// HALL_3_PIN  
    INTC_register_interrupt(&hall_int_handler, HALL_GPIO_IRQ+HALL_1_PIN/8, AVR32_INTC_INT0);  
    INTC_register_interrupt(&hall_int_handler, HALL_GPIO_IRQ+HALL_2_PIN/8, AVR32_INTC_INT0);
    INTC_register_interrupt(&hall_int_handler, HALL_GPIO_IRQ+HALL_3_PIN/8, AVR32_INTC_INT0);	
}

//------------------------------------------------------------------------------
/*! \name Start function
 */
//! @{
void hall_estimator_start(volatile hall_estimator_options_t *hall_estimator_options)
{
   hall_ti = Get_sys_count();
}

//------------------------------------------------------------------------------
/*! \name Estimator update teta / sector number
 */
//! @{
void hall_estimator_update_teta(volatile hall_estimator_options_t *hall_estimator_options,U32 resolution)
{

   if (Delay_For_Change != 0)
   {

      Delay_For_Change --;
      if (Delay_For_Change == 0)
      {
         // Change_Sector 
         Top_Counter = Mem_Top_Counter;
         if (Top_Counter == 0) Top_Counter++; // to avoid "divide by 0"
         Sensor_Counter_Down = Mem_Top_Counter;
         Hall_Sector = Mem_Hall_Sector;
         // end Change_Sector 
      }
   }

   // increment Sensor_Counter_Up 
   if ( Sensor_Counter_Up < 255 )
   {
      Sensor_Counter_Up += 1;
   }

   // decrement Sensor_Counter_Down 
   if (Sensor_Counter_Down != 0) Sensor_Counter_Down -= 1;
  
   // Update teta value 
   hall_estimator_options->teta = ((U16)(Top_Counter - Sensor_Counter_Down) * resolution) / Top_Counter;
   // Update sector number value 
   hall_estimator_options->sector_number = hall_state;

}
//------------------------------------------------------------------------------
/*! \name Estimator update speed
 */
//! @{
void hall_estimator_update_speed(volatile hall_estimator_options_t *hall_estimator_options)
{
   // Update rotor speed evaluation 
    hall_estimator_options->speed = filt_hall_period;
}
//@}
