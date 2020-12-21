/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: GPIO example for XMega.

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file main.c
 *  @brief GPIO example for XMega.
 *
 *	This file contains several examples:
 *		1) Configure pin R.0 as output and clear it to turn on a LED. Set symbol EXAMPLE = 1.
 *		2) Configure pin R.0 as output and pin E.5 as input Set the output of R.0 to the state of E.5. Set symbol EXAMPLE = 2.
 *		3) Same as example 1 but with invert mode enabled. Set symbol EXAMPLE = 3.
 *		4) Multi pin configuration. Set symbol EXAMPLE = 4.
 *		5) Enable the clock output on port C. Set symbol EXAMPLE = 5.
 *		6) Interrupt example for GPIO. Use pin E.5 as input with interrupts to toggle the LED on R.0. Set symbol EXAMPLE = 6.
 *
 *  Software for the XMega GPIO tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-io/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	#if(EXAMPLE == 1)
		/*
			Initialize the LED
				-> Output
				-> PR0 (LED0 active low)
		*/
		PORTR.DIRSET = (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);

	#elif(EXAMPLE == 2)
		/*
			Initialize the LED
				-> Output
				-> PR0 (LED0 active low)
		*/
		PORTR.DIRSET = (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);
		
		/*
			Initialize the Button
				-> Input
				-> PE5 (Button1 active low)
		*/
		PORTE.DIRCLR = (0x01 << 0x00);
		
		while(1)
		{
			if((PORTE.IN & (0x01 << 0x05)) >> 0x05)
			{
				PORTR.OUTSET = (0x01 << 0x00);
			}
			else
			{
				PORTR.OUTCLR = (0x01 << 0x00);
			}
			
			PORTR.OUT
		}
		
	#elif(EXAMPLE == 3)
		/*
			Initialize the LED
				-> Output
				-> PR0 (LED0 active low)
				-> Invert pin R.0
		*/
		PORTR.DIRSET = (0x01 << 0x00);
		PORTR.PIN0CTRL |= PORT_INVEN_bm;
		
		// Set pin R.0 (NOTE: You need set, because of the invert mode)
		PORTR.OUTSET = (0x01 << 0x00);
		
	#elif(EXAMPLE == 4)
		/*
			Configure pin 1, 4 and 7 with one instruction
				-> Enable invert mode
		*/
		PORTCFG.MPCMASK = (0x01 << 0x07) | (0x01 << 0x04) | (0x01 << 0x01);
		PORTC.PIN1CTRL = PORT_INVEN_bm;

	#elif(EXAMPLE == 5)
		/*
			Initialize the clock output
				-> Output
				-> Enable clock output
		*/
		PORTC.DIRSET = (0x01 << 0x07);
		PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_PC7_gc;
	
	#elif(EXAMPLE == 6)
		/*
			Initialize the LED
				-> Output
				-> PR0 (LED0 active low)
		*/
		PORTR.DIRSET = (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);
		
		/*
			Initialize the Button
				-> Input
				-> PE5 (Button1 active low)
		*/
		PORTE.DIRCLR = (0x01 << 0x00);

		/*
			Configure interrupts for PE5
				-> INT0
				-> Falling edge (low level)
		*/
		PORTE.PIN5CTRL |= PORT_ISC_FALLING_gc;
		PORTE.INT0MASK |= (0x01 << 0x05);
		PORTE.INTCTRL = PORT_INT0LVL_LO_gc;

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();

	#endif

    while(1) 
    {
    }
}

ISR(PORTE_INT0_vect)
{
	PORTR.OUTTGL = (0x01 << 0x00);
}