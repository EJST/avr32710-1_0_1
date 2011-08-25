/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Motor Control Shell service for AVR32 UC3.
 *
 * AVR32 Motor Control Shell service.
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
#ifndef _USHELL_TASK_H_
#define _USHELL_TASK_H_

//_____ I N C L U D E S ____________________________________________________

//_____ M A C R O S ________________________________________________________


// Commands shell
#define CMD_NONE           0x00
#define CMD_HELP           0x01
#define CMD_RUN            0x02
#define CMD_STOP           0x03
#define CMD_FORWARD        0x04
#define CMD_BACKWARD       0x05
#define CMD_SET_SPEED      0x06
#define CMD_GET_ID         0x07
#define CMD_GET_STATUS0    0x08
#define CMD_GET_STATUS1    0x09


// Special char values
#define CR                 0x0D
#define LF                 0x0A
#define CTRL_Q             0x11
#define CTRL_C             0x03
#define BKSPACE_CHAR       0x08
#define ABORT_CHAR         CTRL_C
#define QUIT_APPEND        CTRL_Q

// String values for commands
#define STR_RUN            "ru"
#define STR_STOP           "st"
#define STR_HELP           "help"
#define STR_FORWARD        "fw"
#define STR_BACKWARD       "bw"
#define STR_SET_SPEED      "ss"
#define STR_GET_ID         "gi"
#define STR_GET_STATUS0    "g0"
#define STR_GET_STATUS1    "g1"

// String messages
#define MSG_PROMPT         "\r>"
#define MSG_WELCOME        "\rATMEL Motor Control Interface"
#define MSG_ER_CMD_NOT_FOUND  "Unknown Command"
#define MSG_HELP    "\r\
ru : run\r\
st : stop\r\
fw : forward\r\
bw : backward\r\
ss : set speed\r\
gi : get id\
"

//_____ D E C L A R A T I O N S ____________________________________________

extern void   build_cmd(void);
extern void   parse_cmd(void);
extern U8     mystrncmp(U8 *str1,U8 *str2,U8 i);
extern void   print_msg(U8 *str);
extern void   ushell_task_init(void);
extern void   ushell_task(void);

#endif /* _USHELL_TASK_H_ */

