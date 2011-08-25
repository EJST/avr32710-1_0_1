/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief SVPWM configuration file.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
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


#ifndef _CONF_SVPWM_H_
#define _CONF_SVPWM_H_

#include <avr32/io.h>
#include "board.h"

#define FCPU_HZ                   42000000
#define FPBA_HZ                   (FCPU_HZ/2)

#define TC_CHANNEL    0
/*! \name USART Settings
 */
//! @{
#if BOARD == EVK1100
#  define STDIO_USART               (&AVR32_USART1)
#  define STDIO_USART_BAUDRATE      57600
#  define STDIO_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#  define STDIO_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#  define STDIO_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#  define STDIO_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#elif BOARD == EVK1101
#  define STDIO_USART               (&AVR32_USART1)
#  define STDIO_USART_BAUDRATE      57600
#  define STDIO_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#  define STDIO_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#  define STDIO_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#  define STDIO_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#endif
//! @}


// Here you have to define your control coefficients
// Kp for the proportionnal coef
// Ki for the integral coef
// Kd for the derivative coef

// Speed regulation coefficients
#define Kp_speed 10//1 MMT motor//8 MAXON moto
#define Ki_speed 3//10 MMT motor//8 MAXON motor
#define Kd_speed 1

// All PID coef are multiplied by 2^Kmul
// For exemple : kp = 1 => Kp = 1 * 2^K_scal = 1 * 2^4 = 16
// To get the right result you have to divide the number by 2^K_scal
// So execute a K_scal right shift
#define K_speed_scal 4// 4 MMT motor//5 MAXON motor
#define K_cur_scal 4
#define K_pos_scal 5


#define RESOLUTION 256
// Speed measurement
// K_SPEED = (60 * FCPU_HZ * 255)/(6* n * speed_max(rpm))
// 6: as we count every interrupt changes
// n : number of pairs of poles.
// P_TICK
#define K_SPEED                       4371428 // TECMOTION : FL42BLS01-001 with n = 4 , speed_max = 7000

#define FTICK_HZ                      15625 // 64µs
#define DELAY                         (FCPU_HZ/FTICK_HZ)

#define TOP_PWM                       250 /* f pwm = 60e6 /2 /4 /250 /2 = 15 kHz */
#define DEAD_TIME                     10

#define PRESCALER_TICK                3  //Main loop = P_TICK*PRESCALER_TICK //!< prescaler for Tick
#define PRESCALER_REGULATION_LOOP     1 //Regulation loop = P_TICK*PRESCALER_REGULATION_LOOP //!< prescaler for Tick

#define MAX_AMPLITUDE                 150//!< voltage to apply
#define MIN_AMPLITUDE                 10

#define n_SAMPLE                      8
#define n_SAMPLE_RESOLUTION           3

/*! \name Task Support Settings
 */
//! @{
#define SENSOR_TASK_SUPPORT
//! @}

#endif  // _CONF_SVPWM_H_
