/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Clock example for XMega.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file main.c
 *  @brief Clock example for XMega.
 *
 *  Software for the XMega clock tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-clock/
 *
 *	This file contains several examples:
 *		1) Enable the internal 32 MHz oscillator. Set symbol EXAMPLE = 1.
 *		   NOTE: You have to use at least -O1 as optimization to use this example.
 *		2) Enable the internal 32 MHz oscillator. Set symbol EXAMPLE = 2.
 *		   NOTE: You have to use at least -O1 as optimization to use this example.
 *		3) Enable a external 16 MHz crystal oscillator with a startup time of 16 clock cycles. Set symbol EXAMPLE = 3.
 *		   NOTE: You have to use at least -O1 as optimization to use this example.
 *		4) Enable the PLL to increase the clock of the internal 32 MHz oscillator to 48 MHz. Set symbol EXAMPLE = 4.
 *		   NOTE: You have to use at least -O1 as optimization to use this example.
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

int main(void)
{
#if(EXAMPLE == 1)
	/*
		Initialize the internal 32 MHz oscillator
	*/
	OSC.CTRL |= OSC_RC32MEN_bm;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

#elif(EXAMPLE == 2)
	/*
		Initialize the internal 2 MHz oscillator
	*/
	OSC.CTRL |= OSC_RC2MEN_bm;
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC2M_gc;
	
#elif(EXAMPLE == 3)
	/*
		Initialize the external crystal oscillator
	*/
	OSC_XOSCCTRL = OSC_XOSCSEL_XTAL_16KCLK_gc | OSC_FRQRANGE_12TO16_gc; 
	OSC.CTRL |= OSC_XOSCEN_bm; 
	while(!(OSC.STATUS & OSC_XOSCRDY_bm)); 
	CCP = CCP_IOREG_gc; CLK.CTRL = CLK_SCLKSEL_XOSC_gc;

#elif(EXAMPLE == 4)
	/*
		Initialize the PLL
			-> 48 MHz output frequency
			-> 32 MHz internal oscillator as input
	*/
	OSC.CTRL |= OSC_RC32MEN_bm;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	
	OSC.PLLCTRL = OSC_PLLSRC_RC32M_gc | 0x06;
	
	OSC.CTRL |= OSC_PLLEN_bm;
	while(!(OSC.STATUS & OSC_PLLRDY_bm));

	CCP = CCP_IOREG_gc; 
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;

#endif

	/*
		Configure the output for the system clock
			-> Port C.7
	*/
	PORTC.DIRSET = (0x01 << 0x07);
	PORTCFG.CLKEVOUT |= PORTCFG_CLKOUT_PC7_gc;

    while(1) 
    {
    }
}