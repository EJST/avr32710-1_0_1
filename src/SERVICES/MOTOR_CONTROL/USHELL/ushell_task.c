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

//_____  I N C L U D E S ___________________________________________________
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#if __GNUC__ && __AVR32__
  #include "nlao_cpu.h"
  #include "nlao_usart.h"
  #include "usart.h"
#elif __ICCAVR32__
  #include "usart.h"
#endif
#include "conf_svpwm.h"
#include "conf_shell.h"
#include "ushell_task.h"
#include "ascii.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________
void convert_param1(void);
void print_hex16(U16 value);

//_____ D E C L A R A T I O N S ____________________________________________

volatile static U8 cmd;
volatile static U8 cmd_type;
volatile static U16 par_str1[8];
volatile static U16 par_str2[8];
volatile static U16 param1;
volatile static U16 param2;
volatile static U8 cmd_str[26];
volatile static U8 i_str=0;

U8 str_run[]=STR_RUN;
U8 str_stop[]=STR_STOP;
U8 str_help[]=STR_HELP;
U8 str_forward[]=STR_FORWARD;
U8 str_backward[]=STR_BACKWARD;
U8 str_set_speed[]=STR_SET_SPEED;
U8 str_get_id[]=STR_GET_ID;
U8 str_get_status0[]=STR_GET_STATUS0;
U8 str_get_status1[]=STR_GET_STATUS1;

U8 msg_prompt[]=MSG_PROMPT;
U8 msg_welcome[]=MSG_WELCOME;
U8 msg_help[]=MSG_HELP;
U8 msg_er_cmd_not_found[]=MSG_ER_CMD_NOT_FOUND;


void ushell_task_init(void)
{
   printf("\n");
   printf("%s",msg_welcome);
   printf("\n");
   printf("%s",msg_prompt);
   cmd=FALSE;
   cmd_type=CMD_NONE;
}

void ushell_task(void)
{
   U8 status = 0;

   if(cmd==FALSE)
   {
      build_cmd();
   }
   else
   {
      switch (cmd_type)
      {
         case CMD_HELP:
            printf("%s",msg_help);
            break;
         case CMD_RUN:
            mci_run();
            break;
         case CMD_STOP:
            mci_stop();
            break;
         case CMD_FORWARD:
            mci_forward();
            break;
         case CMD_BACKWARD:
            mci_backward();
            break;
         case CMD_SET_SPEED:
            convert_param1();
            mci_set_speed(param1);
            break;
         case CMD_GET_ID:
            print_hex16(BOARD_ID);
            putchar(' ');
            print_hex16(SOFT_ID);
            putchar(' ');
            print_hex16(REV_ID);
            break;
         case CMD_GET_STATUS0:
            status = (mci_get_direction()<<3)|(mci_get_run_stop()<<2);
            print_hex16(status);
            putchar(' ');
            print_hex16(mci_get_measured_speed());
            putchar(' ');
            print_hex16(mci_get_measured_current());
            break;
         case CMD_GET_STATUS1:
            print_hex16(0xDEA);
            putchar(' ');
            print_hex16(0x123);
            break;
         default:    //Unknown command
            printf("%s",msg_er_cmd_not_found);
      }
      cmd_type=CMD_NONE;
      cmd=FALSE;
      printf("%s",msg_prompt);
   }
}

void build_cmd(void)
{
U8 c;
   if (usart_test_hit(STDIO_USART))    //Something new of  the UART ?
   {
      c=getchar();
      switch (c)
      {
         case CR:
            cmd_str[i_str]=0;  //Add NULL char
            parse_cmd();    //Decode the command
            i_str=0;
            break;
         case ABORT_CHAR:    //^c abort cmd
            i_str=0;
            printf("\r#");
            break;
         case BKSPACE_CHAR:   //backspace
            if(i_str>0)
            {
               putchar(c);
               putchar(' ');
               putchar(c);
            }
            if(i_str>=1)
            {
               i_str--;
            }
            break;

         default:
            cmd_str[i_str++]=c;  //append to cmd line
            break;
      }
   }
}


void parse_cmd(void)
{
   U8 i=0;
   U8 j;

   //Get command type
   if (cmd_str[i]!=' ')
   {
    for(i=0;cmd_str[i]!=' ' && i<=i_str;i++);
    cmd=TRUE;
   }

   //Decode command type

   if ( !strncmp((const char *)cmd_str,(const char *)str_run,i-1))
   {  cmd_type=CMD_RUN; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_stop,i))
   {  cmd_type=CMD_STOP; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_help,i-1))
   {  cmd_type=CMD_HELP; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_forward,i-1))
   {  cmd_type=CMD_FORWARD; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_backward,i-1))
   {  cmd_type=CMD_BACKWARD; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_set_speed,i))
   {  cmd_type=CMD_SET_SPEED; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_get_id,i-1))
   {  cmd_type=CMD_GET_ID; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_get_status0,i-1))
   {  cmd_type=CMD_GET_STATUS0; }
   else if ( !strncmp((const char *)cmd_str,(const char *)str_get_status1,i-1))
   {  cmd_type=CMD_GET_STATUS1; }
   else
   {
      if(i_str)
      {
        printf("%s",msg_er_cmd_not_found);
      }
      printf("%s",msg_prompt);
      cmd=FALSE;
      return;
   }

  //Get first arg (if any)
   if(++i<i_str)
   {
      j=0;
      for(;(cmd_str[i]!=' ')&&(i<i_str);i++)
      {
         LSB(par_str1[j])=cmd_str[i];
         MSB(par_str1[j])=0;
         j++;
      }
      LSB(par_str1[j])=0;MSB(par_str1[j])=0;
   }
   else   {  return; }

   //Get second arg (if any)
   if(++i<i_str)
   {
      j=0;
      for(;(cmd_str[i]!=' ')&&(i<i_str);i++)
      {
         LSB(par_str2[j])=cmd_str[i];
         MSB(par_str2[j])=0;
         j++;
      }
      LSB(par_str2[j])=0;MSB(par_str2[j])=0;
   }
   else   { return; }

}

void convert_param1(void)
{
   U8 i = 0;
   param1 = 0;
   while ( par_str1[i] != 0 )
   {
      param1 = param1 << 4;
      param1 = param1 + ascii_to_bin(par_str1[i]);
      i++;
   }
}

void convert_param2(void)
{
   U8 i = 0;
   param2 = 0;
   while ( par_str2[i] != 0 )
   {
      param2 = param2 << 4;
      param2 = param2 + ascii_to_bin(par_str2[i]);
      i++;
   }
}

void print_hex16(U16 value)
{
   U8 c;
   U8 d4;
   U8 d3;
   U8 d2;
   U8 d1;

   d4 = (U16)(value >> 12) & 0x0F;
   d3 = (U16)(value >> 8) & 0x0F;
   d2 = (U16)(value >> 4) & 0x0F;
   d1 = (U16)(value) & 0x0F;

   if (d4 != 0)
   {
      c = bin_to_ascii(d4);
      putchar(c);

      c = bin_to_ascii(d3);
      putchar(c);

      c = bin_to_ascii(d2);
      putchar(c);
   }
   else
   {
      if (d3 != 0)
      {
         c = bin_to_ascii(d3);
         putchar(c);

         c = bin_to_ascii(d2);
         putchar(c);
      }
      else
      {
         if (d2 != 0)
         {
            c = bin_to_ascii(d2);
            putchar(c);
         }
      }
   }

   c = bin_to_ascii(d1);
   putchar(c);
}



