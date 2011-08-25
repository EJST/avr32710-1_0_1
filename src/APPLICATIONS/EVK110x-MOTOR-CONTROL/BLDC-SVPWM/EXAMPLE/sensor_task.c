/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Motor Control Sensor Task service for AVR32 UC3.
 *
 * AVR32  Motor Control Sensor Task service.
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
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "conf_svpwm.h"
#include "conf_shell.h"
#include "sensor_task.h"
#include "intc.h"
#include "spi.h"
#include "dip204.h"
#include "gpio.h"
#include "adc.h"
#include "delay.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________

//_____ D E C L A R A T I O N S ____________________________________________
volatile int sensor_Cmd = DV_JOYSTICK_JS_NONE;              //!< Current Joystick Command

static volatile avr32_adc_t * adc= (volatile avr32_adc_t *) &AVR32_ADC;

void sensor_joystick_handler(void) 
{
  if(gpio_get_pin_value(GPIO_JOYSTICK_UP)==0)
  {
    sensor_Cmd = DV_JOYSTICK_JS_UP;
  }
  else
  {
    if(gpio_get_pin_value(GPIO_JOYSTICK_DOWN)==0)
    {
      sensor_Cmd = DV_JOYSTICK_JS_DOWN;
    }
    else
    {
        if(gpio_get_pin_value(GPIO_JOYSTICK_LEFT)==0)
        {
            sensor_Cmd = DV_JOYSTICK_JS_LEFT;
        }
        else
        {
            if(gpio_get_pin_value(GPIO_JOYSTICK_RIGHT)==0)
            {
              sensor_Cmd = DV_JOYSTICK_JS_RIGHT;
            }
        }
    }
  }
}

int sensor_potentiometer_value(void)
{
   int i_current_val;
   
   /* enable channel for sensor */
   adc_enable( adc, ADC_POTENTIOMETER_CHANNEL );
   /* start conversion */
   adc_start( adc );
   /* get value for sensor */
   i_current_val = adc_get_value( adc, ADC_POTENTIOMETER_CHANNEL ) * 100 / ADC_MAX_VALUE;
   /* Disable channel for sensor */
   adc_disable( adc, ADC_POTENTIOMETER_CHANNEL );
   
   return i_current_val; 
}

void sensor_task_init(void)
{

    // Display Initialization
    static const gpio_map_t DIP204_SPI_GPIO_MAP =
  {
    {DIP204_SPI_SCK_PIN,  DIP204_SPI_SCK_FUNCTION },  // SPI Clock.
    {DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION},  // MISO.
    {DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION},  // MOSI.
    {DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION}   // Chip Select NPCS.
  };

  // add the spi options driver structure for the LCD DIP204
  spi_options_t spiOptions =
  {
    .reg          = DIP204_SPI_NPCS,
    .baudrate     = 1000000,
    .bits         = 8,
    .spck_delay   = 0,
    .trans_delay  = 0,
    .stay_act     = 1,
    .spi_mode     = 0,
    .modfdis      = 1
  };

  // Assign I/Os to SPI
  gpio_enable_module(DIP204_SPI_GPIO_MAP,
                    sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));

  // Initialize as master
  spi_initMaster(DIP204_SPI, &spiOptions);

  // Set selection mode: variable_ps, pcs_decode, delay
  spi_selectionMode(DIP204_SPI, 0, 0, 0);

  // Enable SPI
  spi_enable(DIP204_SPI);

  // setup chip registers
  spi_setupChipReg(DIP204_SPI, &spiOptions, FOSC0);

  // Initialize the delay driver.
  delay_init(FCPU_HZ);

  // initialize LCD
  dip204_init(backlight_PWM, TRUE);

  // Display default message.
  dip204_set_cursor_position(1,1);
  dip204_write_string("SVPWM with AVR32 UC3");
  dip204_set_cursor_position(1,2);
  dip204_write_string("STATE:");
  dip204_set_cursor_position(1,3);
  dip204_write_string("SPEED:");
  dip204_set_cursor_position(1,4);
  dip204_write_string("DIR:");
  dip204_hide_cursor();


  // Joystick Initialization
  static const gpio_map_t JOYSTICK_GPIO_MAP =
  {
      {GPIO_JOYSTICK_LEFT,  0},
      {GPIO_JOYSTICK_RIGHT, 0},  
      {GPIO_JOYSTICK_UP,  0},
      {GPIO_JOYSTICK_DOWN,  0}
  };  
  
  /*set pins as input/output function*/
  gpio_enable_gpio(JOYSTICK_GPIO_MAP,
                   sizeof(JOYSTICK_GPIO_MAP) / sizeof(JOYSTICK_GPIO_MAP[0]));
                   
    // Potentiometer Initialization
  gpio_enable_module_pin( ADC_POTENTIOMETER_PIN , ADC_POTENTIOMETER_FUNCTION );
}

void sensor_task(void)
{
   char chaine[13];
    // Acknowledge new inputs
   sensor_joystick_handler();
    switch (sensor_Cmd)
    {
       case DV_JOYSTICK_JS_UP:
          mci_run();
          break;
       case DV_JOYSTICK_JS_DOWN:
          mci_stop();
          break;
       case DV_JOYSTICK_JS_LEFT:
          mci_stop();
          mci_forward();
          break;
       case DV_JOYSTICK_JS_RIGHT:
          mci_stop();
          mci_backward();
          break;
       default:
          break;
    }
    // Get Potentiometer values
    mci_set_speed(sensor_potentiometer_value());

    // Display Motor State
    dip204_set_cursor_position(7,2);
    dip204_write_string("            ");
    dip204_set_cursor_position(7,2);    
    switch (mci_get_run_stop())
    {
      case 0:
        dip204_write_string("<<STOP>>");
        break;
      case 1:
        dip204_write_string("<<RUN>>");
        break;
     }
  
    // Speed Value
    dip204_set_cursor_position(7,3);
    dip204_write_string("            ");    
    dip204_set_cursor_position(7,3);    
    sprintf(chaine,"%d",mci_get_measured_speed());
    dip204_write_string(chaine);
  
    // Current Direction Value
    dip204_set_cursor_position(7,4);
    dip204_write_string("            ");
    dip204_set_cursor_position(7,4);    
    switch (mci_get_direction())
    {
      case 0:
        dip204_write_string("<<FORWARD>>");
        break;
      case 1:
        dip204_write_string("<<BACKWARD>>");
        break;
     }  
     
     sensor_Cmd = DV_JOYSTICK_JS_NONE;
}

