#pragma once
#include <lib/math.h>
#include <lib/string.h>
#include <lib/stdiox.h>
#include <lib/signal.h>
#include <lib/converters.h>
#include <headers/macros.h>

char output_string[80];


uint32_t d2nbns(uint32_t a, uint32_t b)
{

   for(int i = 0; i < sizeof(output_string); i++)
      output_string[i] = '\0';

   int system, number,x,y;
   
   system=a;
   number=b;

   /*
   xprintf("SYSTEM: %d\n", system);
   xprintf("NUMBER: %d\n\n", number);
   */

   uint8_t string_counter = 0;

   while(number>=1)
   {
      y=number%system;
      number=number/system;

      if(y < 10)
         y = y + 48;

      if(y >= 10)
         y = y + 55;


      xprintf("%c", y);


//      output_string[string_counter] = y;
      string_counter++;

   }

   //reverse_string(output_string);


}