/*
 * keyboard.c
 *
 *  Created on: May 14, 2023
 *      Author: Yosi
 */

#include "main.h"

uint8_t tolower2(char c)
{
	uint8_t low;
	low = 'a'+ (c-'A');

	return low;
}

// convert ascii character to USB HID Keyboard scancode.
// realize that there is no difference between 'A'/'a' or '1'/'!', they
// have the same scancode.  in order to differentiate between these the SHIFT
// modifier has to be set (see kb_modifier_t).
// see kb_scancode_t for other scancodes that are not ascii.
// bit8 will be set if shift needs to be set (for upper case chars or '!' vs '1')
uint16_t ToHidKeyboardScancode(char c)
{
   uint16_t shift = 0;

   if ((c>='A')&&(c<='Z'))
   {
      c = tolower2(c);
      shift = 0x0200;
   }
   if ((c>='a')&&(c<='z'))
   {
      return(((c-='a')+4)|shift);
   }
   if ((c>='1')&&(c<='9'))
   {
      return((c-='0')+0x1D);
   }
   switch(c)
   {
      case '!':   return(0x21E);
      case '@':   return(0x21F);
      case '#':   return(0x220);
      case '$':   return(0x221);
      case '%':   return(0x222);
      case '^':   return(0x223);
      case '&':   return(0x224);
      case '*':   return(0x225);
      case '(':   return(0x226);
      case ')':   return(0x227);
      case '0':   return(0x27);
      case '\n':  return(0x28);  //enter
      case '\r':  return(0x28);  //enter
      case '\b':  return(0x2A);  //backspace
      case '\t':  return(0x2B);  //tab
      case ' ':   return(0x2C);  //space
      case '_':   return(0x22D);
      case '-':   return(0x2D);
      case '+':   return(0x22E);
      case '=':   return(0x2E);
      case '{':   return(0x22F);
      case '[':   return(0x2F);
      case '}':   return(0x230);
      case ']':   return(0x30);
      case '|':   return(0x231);
      case '\\':   return(0x31);
      case ':':   return(0x233);
      case ';':   return(0x33);
      case '"':   return(0x234);
      case '\'':   return(0x34);
      case '~':   return(0x235);
      case '`':   return(0x35);
      case '<':   return(0x236);
      case ',':   return(0x36);
      case '>':   return(0x237);
      case '.':   return(0x37);
      case '?':   return(0x238);
      case '/':   return(0x38);
   }
   return(0); // this should be error
}

