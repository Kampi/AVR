/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: CRCSCAN example for ATtiny406.

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
 *  @brief CRCSCAN example for ATtiny406.
 *
 *  Software for the ATtiny406 CRCSCAN tutorial from
 *  https://www.kampis-elektroecke.de/avr/avr8/attiny406-crcscan/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define GRUEN			0x00
#define ROT				0x01

int main(void)
{
	/*
		Initialize the LEDs
			-> Output
			-> PC0 and PC1 (RED and GREEN active high)
	*/
	PORTC.DIR = (0x01 << ROT) | (0x01 << GRUEN);
	PORTC.OUTCLR = (0x01 << ROT) | (0x01 << GRUEN);

	/*
		Initialize the CRC module
			-> Flash memory as source
			-> Enable NMI
	*/
	CRCSCAN.CTRLB = CRCSCAN_SRC_FLASH_gc;
	CRCSCAN.CTRLA = CRCSCAN_NMIEN_bm | CRCSCAN_ENABLE_bm;

	if(CRCSCAN.STATUS & CRCSCAN_OK_bm) 
	{
		PORTC.OUTCLR = (0x01 << ROT);
		PORTC.OUTSET = (0x01 << GRUEN);
	}

	while(1) 
	{

	}

	return 0;
}

ISR(CRCSCAN_NMI_vect)
{
	// Disable green LED and enable red LED if the CRC fails
	PORTC.OUTCLR = (0x01 << GRUEN);
	PORTC.OUTSET = (0x01 << ROT);
}