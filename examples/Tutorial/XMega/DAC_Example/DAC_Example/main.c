/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: DAC example for XMega.

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
 *  @brief DAC example for XMega.
 *
 *  Software for the XMega DAC tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-dac/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>

int main(void)
{
	/*
		Initialize the DAC
			-> 1 V reference
			-> 12-bit
			-> Channel 0
	*/
   DACB.CTRLC &= ~(DAC_REFSEL1_bm | DAC_REFSEL0_bm | DAC_LEFTADJ_bm);
   DACB.CTRLB &= ~(DAC_CH1TRIG_bm | DAC_CH0TRIG_bm);
   DACB.CTRLA |= (DAC_CH0EN_bm | DAC_ENABLE_bm);
   
   while(!DACB.STATUS & DAC_CH0DRE_bm);
   DACB.CH0DATA = 0x100;

   while(1)
   {
   }
}

