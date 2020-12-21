/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: RTC example for XMega.

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
 *  @brief RTC example for XMega.
 *
 *  Software for the XMega RTC tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-rtc/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	/*
		Initialize the LED
			-> Output
			-> PR0 (LED0 active low)
	*/
	PORTR.DIRSET = (0x01 << 0x00);
	PORTR.OUTCLR = (0x01 << 0x00);
	
	/*
		Configure oscillator
			-> Use an external 32 kHz oscillator
			-> Divide the clock to 1024 kHz
	*/
	OSC.XOSCCTRL = OSC_X32KLPM_bm | OSC_XOSCSEL_32KHz_gc;
	OSC.CTRL = OSC_XOSCEN_bm;
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	CLK.RTCCTRL = CLK_RTCSRC_TOSC_gc | CLK_RTCEN_bm;
	
	/*
		Configure RTC
			-> Overflow each second
			-> Overflow interrupts (low priority)
	*/
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	while(RTC.STATUS & RTC_SYNCBUSY_bm);
	RTC.PER = 1023;
	while(RTC.STATUS & RTC_SYNCBUSY_bm);
	RTC.CNT = 0;
	while(RTC.STATUS & RTC_SYNCBUSY_bm);
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;
	
	/*
		Enable interrupts
			-> Priority low
	*/
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();

    while(1) 
    {
    }
}

ISR(RTC_OVF_vect)
{
	PORTR.DIRTGL = (0x01 << 0x00);
}

