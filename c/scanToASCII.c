/* scanToASCII.h
 *
 * Copyright (c) 2013 Jack Wu <jack.wu@live.ca>
 *
 * This file is part of bkernel.
 *
 * bkernel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bkernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar. If not, see <http://www.gnu.org/licenses/>.
 */

#include <scanToASCII.h>


static  int     state; /* the state of the keyboard */

/*  Normal table to translate scan code  */

unsigned char   kbcode[] = { 0,
          27,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
         '0',  '-',  '=', '\b', '\t',  'q',  'w',  'e',  'r',  't',
         'y',  'u',  'i',  'o',  'p',  '[',  ']', '\n',    0,  'a',
         's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', '\'',
         '`',    0, '\\',  'z',  'x',  'c',  'v',  'b',  'n',  'm',
         ',',  '.',  '/',    0,    0,    0,  ' ' };

/* captialized ascii code table to tranlate scan code */
unsigned char   kbshift[] = { 0,
           0,  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',
         ')',  '_',  '+', '\b', '\t',  'Q',  'W',  'E',  'R',  'T',
         'Y',  'U',  'I',  'O',  'P',  '{',  '}', '\n',    0,  'A',
         'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  '"',
         '~',    0,  '|',  'Z',  'X',  'C',  'V',  'B',  'N',  'M',
         '<',  '>',  '?',    0,    0,    0,  ' ' };

/* extended ascii code table to translate scan code */
unsigned char   kbctl[] = { 0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,   31,    0, '\b', '\t',   17,   23,    5,   18,   20,
          25,   21,    9,   15,   16,   27,   29, '\n',    0,    1,
          19,    4,    6,    7,    8,   10,   11,   12,    0,    0,
           0,    0,   28,   26,   24,    3,   22,    2,   14,   13 };



static int extchar(unsigned char code)
{
        state &= ~EXTENDED;
}

unsigned int kbtoa(unsigned char code)
{
  	unsigned int    ch;
  
  	if (state & EXTENDED)
    		return extchar(code);
  
	if (code & KEY_UP) 
	{
    		switch (code & 0x7f) 
		{
   	 		case LSHIFT:
    			case RSHIFT:
      				state &= ~INSHIFT;
      				break;
    			
			case CAPSL:
      				//printf("Capslock off detected\n");
      				state &= ~CAPSLOCK;
      				break;
    			
			case LCTL:
      				state &= ~INCTL;
      				break;
    			
			case LMETA:
      				state &= ~INMETA;
      				break;
    		}
    
    		return NOCHAR;
  	}
  
  
  	/* check for special keys */
  	switch (code) 
	{
  		case LSHIFT:
  		case RSHIFT:
    			state |= INSHIFT;
    			//printf("shift detected!\n");
    			return NOCHAR;
  
		case CAPSL:
    			state |= CAPSLOCK;
    			//printf("Capslock ON detected!\n");
    			return NOCHAR;
  
		case LCTL:
    			state |= INCTL;
    			return NOCHAR;
  		
		case LMETA:
    			state |= INMETA;
    			return NOCHAR;
  		
		case EXTESC:
    			state |= EXTENDED;
    			return NOCHAR;
  	}
  
  	ch = NOCHAR;
  
  	if (code < sizeof(kbcode))
	{
    		if ( state & CAPSLOCK )
     			ch = kbshift[code];
	  	else
	    		ch = kbcode[code];
  	}

  	if (state & INSHIFT) 
	{
    		if (code >= sizeof(kbshift))
      			return NOCHAR;
    		
		if ( state & CAPSLOCK )
      			ch = kbcode[code];
    		else
      			ch = kbshift[code];
  	}
  
	if (state & INCTL) 
	{
    		if (code >= sizeof(kbctl))
      			return NOCHAR;
    		
		ch = kbctl[code];
  	}

 	if (state & INMETA)
    		ch += 0x80;
  
	return ch;
}

/*
main() {
  kbtoa(LSHIFT);
  printf("45 = %c\n", kbtoa(45));
  kbtoa(LSHIFT | KEY_UP);
  printf("45 = %c\n", kbtoa(45));
}
*/
