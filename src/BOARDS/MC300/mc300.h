/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief AT32UC3 MC300 extension board header file.
 *
 * This file contains definitions and services related to the features of the
 * MC300 board rev. A.
 *
 * To use this board, define EXT_BOARD=MC300.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 AT32UC3 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#ifndef _MC300_H_
#define _MC300_H_

#include "compiler.h"


#if BOARD == EVK1100


/*! \name PWM Connections of Actuators
 */
//! @{

#define PWM_PORT_NUMBER             1
#define PWM_PORT                    (AVR32_GPIO.port[PWM_PORT_NUMBER])

// PWM0.
#define PWM_XH_PIN_NUMBER           AVR32_PWM_0_PIN
#define PWM_XH                      (PWM_XH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_XH_PWM_FUNCTION         AVR32_PWM_0_FUNCTION
#define PWM_XH_PWM_CHANNEL          0

// PWM1.
#define PWM_XL_PIN_NUMBER           AVR32_PWM_1_PIN
#define PWM_XL                      (PWM_XL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_XL_PWM_FUNCTION         AVR32_PWM_1_FUNCTION
#define PWM_XL_PWM_CHANNEL          1

// PWM2.
#define PWM_YH_PIN_NUMBER           AVR32_PWM_2_PIN
#define PWM_YH                      (PWM_YH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_YH_PWM_FUNCTION         AVR32_PWM_2_FUNCTION
#define PWM_YH_PWM_CHANNEL          2

// PWM3.
#define PWM_YL_PIN_NUMBER           AVR32_PWM_3_PIN
#define PWM_YL                      (PWM_YL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_YL_PWM_FUNCTION         AVR32_PWM_3_FUNCTION
#define PWM_YL_PWM_CHANNEL          3

// PWM4.
#define PWM_ZH_PIN_NUMBER           AVR32_PWM_4_1_PIN
#define PWM_ZH                      (PWM_ZH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_ZH_PWM_FUNCTION         AVR32_PWM_4_1_FUNCTION
#define PWM_ZH_PWM_CHANNEL          4

// PWM5.
#define PWM_ZL_PIN_NUMBER           AVR32_PWM_5_1_PIN
#define PWM_ZL                      (PWM_ZL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_ZL_PWM_FUNCTION         AVR32_PWM_5_1_FUNCTION
#define PWM_ZL_PWM_CHANNEL          5

//! @}


/*! \name TC Connections of Actuators
 */
//! @{

#define TC_PORT_NUMBER              1
#define TC_PORT                     (AVR32_GPIO.port[TC_PORT_NUMBER])

// PWM0.
#define TC_XH_PIN_NUMBER            AVR32_TC_A0_0_0_PIN
#define TC_XH                       (TC_XH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_XH_TC_FUNCTION           AVR32_TC_A0_0_0_FUNCTION

// PWM1.
#define TC_XL_PIN_NUMBER            AVR32_TC_B0_0_0_PIN
#define TC_XL                       (TC_XL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_XL_TC_FUNCTION           AVR32_TC_B0_0_0_FUNCTION

#define TC_XHXL_TC_CHANNEL          0

// PWM2.
#define TC_YH_PIN_NUMBER            AVR32_TC_A1_0_0_PIN
#define TC_YH                       (TC_YH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_YH_TC_FUNCTION           AVR32_TC_A1_0_0_FUNCTION

// PWM3.
#define TC_YL_PIN_NUMBER            AVR32_TC_B1_0_0_PIN
#define TC_YL                       (TC_YL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_YL_TC_FUNCTION           AVR32_TC_B1_0_0_FUNCTION

#define TC_YHYL_TC_CHANNEL          1

// PWM4.
#define TC_ZH_PIN_NUMBER            AVR32_TC_A2_0_0_PIN
#define TC_ZH                       (TC_ZH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_ZH_TC_FUNCTION           AVR32_TC_A2_0_0_FUNCTION

// PWM5.
#define TC_ZL_PIN_NUMBER            AVR32_TC_B2_0_0_PIN
#define TC_ZL                       (TC_ZL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_ZL_TC_FUNCTION           AVR32_TC_B2_0_0_FUNCTION

#define TC_ZHZL_TC_CHANNEL          2

//! @}


/*! \name GPIO Connections of Hall-Effect Sensors
 */
//! @{

#define HALL_PORT_NUMBER            0
#define HALL_PORT                   (AVR32_GPIO.port[HALL_PORT_NUMBER])
#define HALL_GPIO_IRQ               AVR32_GPIO_IRQ_0

#define HALL_1                      (AVR32_PIN_PA09 - HALL_PORT_NUMBER * 0x20)
#define HALL_2                      (AVR32_PIN_PA29 - HALL_PORT_NUMBER * 0x20)
#define HALL_3                      (AVR32_PIN_PA30 - HALL_PORT_NUMBER * 0x20)
#define HALL_1_PIN                  AVR32_PIN_PA09
#define HALL_2_PIN                  AVR32_PIN_PA29
#define HALL_3_PIN                  AVR32_PIN_PA30

//! @}


/*! \name GPIO and TC Connections of the Quadrature Decoder
 */
//! @{

#define QDEC0A_GPIO_PIN             AVR32_PIN_PB27
#define QDEC0A_GPIO_IRQ             AVR32_GPIO_IRQ_0

#define QDEC0B_GPIO_PIN             AVR32_PIN_PB28
#define QDEC0B_GPIO_IRQ             AVR32_GPIO_IRQ_0

//! @}


#elif BOARD == EVK1101


/*! \name PWM Connections of Actuators
 */
//! @{

#define PWM_PORT_NUMBER             0
#define PWM_PORT                    (AVR32_GPIO.port[PWM_PORT_NUMBER])

// PWM0.
#define PWM_XH_PIN_NUMBER           AVR32_PWM_0_0_PIN
#define PWM_XH                      (PWM_XH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_XH_PWM_FUNCTION         AVR32_PWM_0_0_FUNCTION
#define PWM_XH_PWM_CHANNEL          0

// PWM1.
#define PWM_XL_PIN_NUMBER           AVR32_PWM_1_0_PIN
#define PWM_XL                      (PWM_XL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_XL_PWM_FUNCTION         AVR32_PWM_1_0_FUNCTION
#define PWM_XL_PWM_CHANNEL          1

// PWM2.
#define PWM_YH_PIN_NUMBER           AVR32_PWM_2_0_PIN
#define PWM_YH                      (PWM_YH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_YH_PWM_FUNCTION         AVR32_PWM_2_0_FUNCTION
#define PWM_YH_PWM_CHANNEL          2

// PWM3.
#define PWM_YL_PIN_NUMBER           AVR32_PWM_6_0_PIN
#define PWM_YL                      (PWM_YL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_YL_PWM_FUNCTION         AVR32_PWM_6_0_FUNCTION
#define PWM_YL_PWM_CHANNEL          6

// PWM4.
#define PWM_ZH_PIN_NUMBER           AVR32_PWM_4_1_PIN
#define PWM_ZH                      (PWM_ZH_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_ZH_PWM_FUNCTION         AVR32_PWM_4_1_FUNCTION
#define PWM_ZH_PWM_CHANNEL          4

// PWM5.
#define PWM_ZL_PIN_NUMBER           AVR32_PWM_5_1_PIN
#define PWM_ZL                      (PWM_ZL_PIN_NUMBER - PWM_PORT_NUMBER * 0x20)
#define PWM_ZL_PWM_FUNCTION         AVR32_PWM_5_1_FUNCTION
#define PWM_ZL_PWM_CHANNEL          5

//! @}


/*! \name TC Connections of Actuators
 */
//! @{

#define TC_PORT_NUMBER              0
#define TC_PORT                     (AVR32_GPIO.port[TC_PORT_NUMBER])

// PWM0.
#define TC_XH_PIN_NUMBER            AVR32_TC_A0_0_1_PIN
#define TC_XH                       (TC_XH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_XH_TC_FUNCTION           AVR32_TC_A0_0_1_FUNCTION

// PWM1.
#define TC_XL_PIN_NUMBER            AVR32_TC_B0_0_1_PIN
#define TC_XL                       (TC_XL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_XL_TC_FUNCTION           AVR32_TC_B0_0_1_FUNCTION

#define TC_XHXL_TC_CHANNEL          0

// PWM2.
#define TC_YH_PIN_NUMBER            AVR32_TC_A1_0_0_PIN
#define TC_YH                       (TC_YH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_YH_TC_FUNCTION           AVR32_TC_A1_0_0_FUNCTION

// PWM3.
#define TC_YL_PIN_NUMBER            AVR32_TC_B1_0_0_PIN
#define TC_YL                       (TC_YL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_YL_TC_FUNCTION           AVR32_TC_B1_0_0_FUNCTION

#define TC_YHYL_TC_CHANNEL          1

// PWM4.
#define TC_ZH_PIN_NUMBER            AVR32_TC_A2_0_0_PIN
#define TC_ZH                       (TC_ZH_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_ZH_TC_FUNCTION           AVR32_TC_A2_0_0_FUNCTION

// PWM5.
#define TC_ZL_PIN_NUMBER            AVR32_TC_B2_0_0_PIN
#define TC_ZL                       (TC_ZL_PIN_NUMBER - TC_PORT_NUMBER * 0x20)
#define TC_ZL_TC_FUNCTION           AVR32_TC_B2_0_0_FUNCTION

#define TC_ZHZL_TC_CHANNEL          2

//! @}


/*! \name GPIO Connections of Hall-Effect Sensors
 */
//! @{

#define HALL_PORT_NUMBER            1
#define HALL_PORT                   (AVR32_GPIO.port[HALL_PORT_NUMBER])
#define HALL_GPIO_IRQ               AVR32_GPIO_IRQ_0

#define HALL_1                      (AVR32_PIN_PB00 - HALL_PORT_NUMBER * 0x20)
#define HALL_2                      (AVR32_PIN_PB01 - HALL_PORT_NUMBER * 0x20)
#define HALL_3                      (AVR32_PIN_PB10 - HALL_PORT_NUMBER * 0x20)
#define HALL_1_PIN                  AVR32_PIN_PB00
#define HALL_2_PIN                  AVR32_PIN_PB01
#define HALL_3_PIN                  AVR32_PIN_PB10

//! @}


/*! \name GPIO and TC Connections of the Quadrature Decoder
 */
//! @{

#define QDEC0A_GPIO_PIN             AVR32_PIN_PA20
#define QDEC0A_GPIO_IRQ             AVR32_GPIO_IRQ_0

#define QDEC0A_TC_PIN               AVR32_TC_CLK0_0_1_PIN
#define QDEC0A_TC_FUNCTION          AVR32_TC_CLK0_0_1_FUNCTION
#define QDEC0A_TC_CHANNEL           0
#define QDEC0A_TC_CLK_SRC_CHANNEL   TC_CLOCK_SOURCE_XC0
#define QDEC0A_TC_EXT_CLK_CHANNEL   TC_CH0_EXT_CLK0_SRC_TCLK0
#define QDEC0A_TC_IRQ_CHANNEL       AVR32_TC_IRQ0

#define QDEC0B_GPIO_PIN             AVR32_PIN_PA09
#define QDEC0B_GPIO_IRQ             AVR32_GPIO_IRQ_0

#define QDEC0B_TC_PIN               AVR32_TC_CLK1_0_0_PIN
#define QDEC0B_TC_FUNCTION          AVR32_TC_CLK1_0_0_FUNCTION
#define QDEC0B_TC_CHANNEL           1
#define QDEC0B_TC_CLK_SRC_CHANNEL   TC_CLOCK_SOURCE_XC1
#define QDEC0B_TC_EXT_CLK_CHANNEL   TC_CH1_EXT_CLK1_SRC_TCLK1
#define QDEC0B_TC_IRQ_CHANNEL       AVR32_TC_IRQ1

//! @}


/*! \name ADC Connections of Current Measurement
 */
//! @{

// PA4.
#define CURRENT_IA_ADC_CHANNEL      1
#define CURRENT_IA_ADC_PIN          AVR32_ADC_AD_1_PIN
#define CURRENT_IA_ADC_FUNCTION     AVR32_ADC_AD_1_FUNCTION

// PA5.
#define CURRENT_IB_ADC_CHANNEL      2
#define CURRENT_IB_ADC_PIN          AVR32_ADC_AD_2_PIN
#define CURRENT_IB_ADC_FUNCTION     AVR32_ADC_AD_2_FUNCTION

// PA6.
#define CURRENT_IC_ADC_CHANNEL      3
#define CURRENT_IC_ADC_PIN          AVR32_ADC_AD_3_PIN
#define CURRENT_IC_ADC_FUNCTION     AVR32_ADC_AD_3_FUNCTION

//! @}


#else
#  error AVR32 base board not compatible with the MC300 extension board
#endif


#endif  // _MC300_H_
