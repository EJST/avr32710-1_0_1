/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief BLDC SVPWM Motor Control application for AVR32 UC3.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USART module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/*! \page License
 * Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
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
/*!
 * \mainpage
 * \section section1 Description
 *   This application is an S.V.P.W.M generation which control a BLDC Motor
 *   through a real-time motor control application. \n
 *   It runs on an EVK1100 board \n
 *   The main source files are :
 *
 *          - \ref bldc_svpwm_example.c  Main loop
 *          - \ref mc_control.c          Regulation Loop
 *          - \ref mc_driver.c           Low Level Layer
 *          - \ref sensor_task.c         Display Task
 *
 * \section section2 Configuration
 * The configuration of the application is stored in different files :
 *
 *          - \ref conf_svpwm.h               Contains the configuration of the application
 *          - \ref conf_shell.h               Configuration of the shell
 *
 * \section section3 Main Peripheral Use
 *   - CPU Cycle Counter is used for speed measurement
 *   - One Timer-Counter is used to generate the tick reference
 *   - Six PWM channels are used to control motor
 *   - Three GPIO are used for hall sensors acquisition
 *   - USART is used for communication with the shell
 *
 * \section section4 User Manual
 *   - See the associated application note available at http://www.atmel.com
 *
 * \subsection subsection1 List of Material
 *   - EVK1100 (UC3A)
 *
 * \section section5 Supported Compiler
 *  - AVR32-GCC 4.2.2 - atmel 1.0.1 with AVR32 GNU TOOLCHAIN 1.3.4
 *  - IAR C/C++ Compiler for Atmel AVR32 2.21B/W32 (2.21.2.3)
 */

#include <stddef.h>
#include <stdio.h>

#include <avr32/io.h>
#include "flashc.h"
#include "pm.h"
#include "gpio.h"
#include "cycle_counter.h"
#include "intc.h"
#include "mc_driver.h"
#include "mc_control.h"
#include "board.h"
#if __GNUC__ && __AVR32__
  #include "nlao_cpu.h"
  #include "nlao_usart.h"
#elif __ICCAVR32__
  #include "usart.h"
#endif

#include "conf_svpwm.h"

#ifdef SENSOR_TASK_SUPPORT
  #include "sensor_task.h"
#else
  #include "ushell_task.h"
#endif


extern volatile unsigned short tick;

/*! \brief Initializes MCU exceptions.
 */
static void init_exceptions(void)
{
#if __GNUC__ && __AVR32__
  // Import the Exception Vector Base Address.
  extern void _evba;

  // Load the Exception Vector Base Address in the corresponding system
  // register.
  Set_system_register(AVR32_EVBA, (int)&_evba);
#endif

  // Enable exceptions globally.
  Enable_global_exception();
}


/*! \brief Initializes the HSB bus matrix.
 */
static void init_hmatrix(void)
{
  // For the internal-flash HMATRIX slave, use last master as default.
  union
  {
    unsigned long                 scfg;
    avr32_hmatrix_scfg_t          SCFG;
  } u_avr32_hmatrix_scfg = {AVR32_HMATRIX.scfg[AVR32_HMATRIX_SLAVE_FLASH]};
  u_avr32_hmatrix_scfg.SCFG.defmstr_type = AVR32_HMATRIX_DEFMSTR_TYPE_LAST_DEFAULT;
  AVR32_HMATRIX.scfg[AVR32_HMATRIX_SLAVE_FLASH] = u_avr32_hmatrix_scfg.scfg;
}


/*! \brief Initializes the MCU system clocks.
 */
static void init_sys_clocks(void)
{
volatile avr32_pm_t* pm = &AVR32_PM;

  /* Switch the main clock to OSC0 */
  pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);
  /* Setup PLL0 on OSC0 */
  pm_pll_setup(pm,  // volatile avr32_pm_t* pm
               0,   // unsigned int pll
               6,   // unsigned int mul
               1,   // unsigned int div, Sel Osc0/PLL0 or Osc1/Pll1
               0,   // unsigned int osc
               16); // unsigned int lockcount
  pm_pll_set_option(pm, 0, 1, 1, 0);//60MHz

  /* Enable PLL0 */
  pm_pll_enable(pm,0);
  /* Wait for PLL0 locked */
  pm_wait_for_pll0_locked(pm) ;
  /* set divider to 4 for PBA bus */
  pm_cksel(pm,1,0,0,0,0,0);
  /* switch to clock */
  pm_switch_to_clock(pm, AVR32_PM_MCCTRL_MCSEL_PLL0);

#if __GNUC__ && __AVR32__
  // Give the used PBA clock frequency to Newlib, so it can work properly.
  set_cpu_hz(FPBA_HZ);
#endif
}

/*! \brief Initializes STDIO.
 */
static void init_stdio(void)
{
#if __GNUC__ && __AVR32__

  static const gpio_map_t STDIO_USART_GPIO_MAP =
  {
    {STDIO_USART_RX_PIN, STDIO_USART_RX_FUNCTION},
    {STDIO_USART_TX_PIN, STDIO_USART_TX_FUNCTION}
  };

  // Initialize the USART used for STDIO.
  set_usart_base((void *)STDIO_USART);
  gpio_enable_module(STDIO_USART_GPIO_MAP,
                     sizeof(STDIO_USART_GPIO_MAP) / sizeof(STDIO_USART_GPIO_MAP[0]));
  usart_init(STDIO_USART_BAUDRATE);

#elif __ICCAVR32__

  static const gpio_map_t STDIO_USART_GPIO_MAP =
  {
    {STDIO_USART_RX_PIN, STDIO_USART_RX_FUNCTION},
    {STDIO_USART_TX_PIN, STDIO_USART_TX_FUNCTION}
  };

  static const usart_options_t STDIO_USART_OPTIONS =
  {
    .baudrate     = STDIO_USART_BAUDRATE,
    .charlength   = 8,
    .paritytype   = USART_NO_PARITY,
    .stopbits     = USART_1_STOPBIT,
    .channelmode  = USART_NORMAL_CHMODE
  };

  // Initialize the USART used for STDIO.
  extern volatile avr32_usart_t *volatile stdio_usart_base;
  stdio_usart_base = STDIO_USART;
  gpio_enable_module(STDIO_USART_GPIO_MAP,
                     sizeof(STDIO_USART_GPIO_MAP) / sizeof(STDIO_USART_GPIO_MAP[0]));
  usart_init_rs232(STDIO_USART, &STDIO_USART_OPTIONS, FPBA_HZ);

#endif
}


/*! \brief Initializes MCU interrupts.
 */
static void init_interrupts(void)
{
  // Initialize interrupt handling.
  INTC_init_interrupts();

  // Enable interrupts globally.
  Enable_global_interrupt();
}


/*! \brief Low-level initialization routine called during startup, before the
 *         \ref main function.
 */
#if __GNUC__ && __AVR32__
int _init_startup(void)
#elif __ICCAVR32__
int __low_level_init(void)
#endif
{
  init_exceptions();
  init_hmatrix();
  init_sys_clocks();
  init_stdio();
  init_interrupts();

  // EWAVR32: Request initialization of data segments.
  // GCC: Don't-care value.
  return 1;
}


int main(void)
{
  // Configure standard I/O streams as unbuffered.
#if __GNUC__ && __AVR32__
  setbuf(stdin, NULL);
#endif
  setbuf(stdout, NULL);

   // Low level initialization routine for motor control
   mc_init_motor();
   // Force motor in stop position
   mc_stop_motor();
   // Set speed loop for regulation
   mc_set_speed_loop();
   // Initialize Task Support
#ifdef SENSOR_TASK_SUPPORT
   sensor_task_init();
#else
    ushell_task_init();
#endif

   while(1)
   {
      // Regulation_task
      mc_regulation_task();
      // Call Task routine
      #ifdef SENSOR_TASK_SUPPORT
         sensor_task();
      #else
          ushell_task();
      #endif
   }
}
