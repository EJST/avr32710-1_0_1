/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SVPWM service for AVR32 UC3.
 *
 * AVR32 SVPWM service.
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


#include <avr32/io.h>
#include "compiler.h"
#include "hall_estimator.h"
#include "svpwm.h"
#include "dsp.h"


void svpwm_calc(volatile svpwm_options_t *svpwm_options,U8 dir)
{	
        volatile U32 dx,dy,div;
        volatile U32 T;
        
        div = dsp32_op_div(svpwm_options->teta,svpwm_options->resolution);
        dy = dsp32_op_sin(dsp32_op_mul(DSP32_Q(1./3.),div));
        dy = (((U64)(svpwm_options->amplitude))*((U64)(dy)))>>DSP32_QB;
        
        
        div = dsp32_op_div(svpwm_options->resolution-svpwm_options->teta,svpwm_options->resolution);
        dx = dsp32_op_sin(dsp32_op_mul(DSP32_Q(1./3.),div));
	dx = (((U64)(svpwm_options->amplitude))*((U64)(dx)))>>DSP32_QB;
        	
        T = svpwm_options->max_frequency / 2;
	if (dir==0)
	{
	    switch(svpwm_options->sector_number)
	    {
	    case HS_001 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 + dy/2) ; break ;
	     case HS_101 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 + dy/2) ; break ;
	     case HS_100 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 - dy/2) ; break ;
	     case HS_110 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 - dy/2) ; break ;
	     case HS_010 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 - dy/2) ; break ;
	     case HS_011 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 + dy/2) ; break ;
	     default :
	    	svpwm_options->PWM0 = (unsigned short int) (T) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T) ; break ;
	
	    }
	}
	else
	{
	    switch(svpwm_options->sector_number)
	    {
	    case HS_110 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 + dy/2) ; break ;
	     case HS_010 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 + dy/2) ; break ;
	     case HS_011 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 - dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 - dy/2) ; break ;
	     case HS_001 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 + dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T - dx/2 - dy/2) ; break ;
	     case HS_101 :
	    	svpwm_options->PWM0 = (unsigned short int) (T + dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 - dy/2) ; break ;
	     case HS_100 :
	    	svpwm_options->PWM0 = (unsigned short int) (T - dx/2 + dy/2) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T - dx/2 - dy/2) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T + dx/2 + dy/2) ; break ;
	     default :
	    	svpwm_options->PWM0 = (unsigned short int) (T) ;
	    	svpwm_options->PWM1 = (unsigned short int) (T) ;
	    	svpwm_options->PWM2 = (unsigned short int) (T) ; break ;
	
	    }		
	}

}
