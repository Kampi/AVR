/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: AC example for XMega.

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
 *  @brief AC example for XMega.
 *
 *  Software for the XMega AC tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-ac/
 *
 *	This file contains several examples:
 *		1) Voltage limit detection with output on comparator 0. Set symbol EXAMPLE = 1.
 *		2) Same as example 1, but with interrupts. Set symbol EXAMPLE = 2.
 *		3) Window mode example with DAC as voltage input for comparator 0. Set symbol EXAMPLE = 3.
 *		   This example use the outside window interrupt from the analog comparator to toggle an LED.
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	#if(EXAMPLE == 1)
		/*
			Initialize the AC
				-> ACA
				-> Comparator 0
				-> Large hysteresis
				-> Positive input: Pin 4
				-> Negative input: Pin 5
				-> Set A.7 as output for the comparator
		*/
		ACA.AC0CTRL |= AC_HYSMODE1_bm | AC_ENABLE_bm;
		ACA.AC0MUXCTRL |= AC_MUXPOS_PIN4_gc | AC_MUXNEG_PIN5_gc;
		ACA.CTRLA |= AC_AC0OUT_bm;
		PORTA.DIR |= PIN7_bm;
		
		while(!(ACA.STATUS & AC_AC0STATE_bm));
	
	#elif(EXAMPLE == 2)
		/*
			Initialize the LED
				-> PR0 (LED0 active low)
		*/
		PORTR.DIR |= (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);
	
		/*
			Initialize the AC
				-> ACA
				-> Comparator 0
				-> Both edges interrupt (low level)
				-> Large hysteresis
				-> Positive input: Pin 4
				-> Negative input: Pin 5
				-> Set A.7 as output for the comparator
		*/
		ACA.AC0CTRL |= AC_INTMODE_BOTHEDGES_gc | AC_INTLVL_LO_gc | AC_HYSMODE1_bm | AC_ENABLE_bm;
		ACA.AC0MUXCTRL |= AC_MUXPOS_PIN4_gc | AC_MUXNEG_PIN5_gc;
		ACA.CTRLA |= AC_AC0OUT_bm;
		PORTA.DIR |= PIN7_bm;
		
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();	
		
	#elif(EXAMPLE == 3)
		/*
			Initialize the LED
				-> PR0 (LED active low)
		*/
		PORTR.DIR |= (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);
	
		/*
			Initialize the DAC as voltage reference
				-> 1 V reference
				-> 12-bit
				-> Internal channel 0
				-> Set the output voltage to 1 V
		*/
		DACB.CTRLC &= ~(DAC_REFSEL1_bm | DAC_REFSEL0_bm | DAC_LEFTADJ_bm);
		DACB.CTRLB &= ~(DAC_CH1TRIG_bm | DAC_CH0TRIG_bm | DAC_CHSEL1_bm | DAC_CHSEL0_bm);
		DACB.CTRLA |= (DAC_CH0EN_bm | DAC_ENABLE_bm | DAC_IDOEN_bm);
   
		while(!DACB.STATUS & DAC_CH0DRE_bm);
		DACB.CH0DATA = 0xFFF;
	   
		/*
			Initialize the AC
				-> ACA
				-> Comparator 0 & 1
				-> Large hysteresis
				-> Window mode
				-> Upper limit of window: Pin 5
				-> Lower limit of window: DACB
				-> Outside window interrupt (low level)
		*/
		ACA.AC0CTRL |= AC_ENABLE_bm | AC_HYSMODE1_bm;
		ACA.AC0MUXCTRL |= AC_MUXPOS_PIN4_gc | AC_MUXNEG_PIN5_gc;
		
		ACA.AC1CTRL |= AC_ENABLE_bm | AC_HYSMODE1_bm;
		ACA.AC1MUXCTRL |= AC_MUXPOS_PIN4_gc | AC_MUXNEG_DAC_gc;
		
		ACA.WINCTRL |= AC_WEN_bm | AC_WINTMODE1_bm | AC_WINTMODE0_bm | AC_WINTLVL_LO_gc;
		
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

ISR(ACA_AC0_vect)
{
	PORTR.OUTTGL = (0x01 << 0x00);
}

ISR(ACA_ACW_vect)
{
	PORTR.OUTTGL = (0x01 << 0x00);
}

