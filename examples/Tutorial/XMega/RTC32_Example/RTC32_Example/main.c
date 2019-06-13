/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: RTC32 example for XMega.

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
 *  @brief RTC32 example for XMega.
 *		   NOTE: You have to enable the brown out detection
 *					-> Set BODPD to "BOD enabled continously"
 *					-> Set BODACT to "BOD enabled continously"
 *
 *  Software for the XMega RTC32 tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-rtc32/ 
 *
 *  @author Daniel Kampert
 */

#include "BatteryBackup/BatteryBackup.h"
#include "RTC32/RTC32.h"

int main(void)
{
	/*
		Initialize the LED outputs
			-> PR0 (LED active low)
			-> PR1 (LED active low)
	*/
	PORTR.DIR |= (0x01 << 0x01) | (0x01 << 0x00);
	PORTR.OUTCLR = (0x01 << 0x01) | (0x01 << 0x00);
	
	// Check the status of the battery backup system
	BatteryStatus_t Status = Battery_CheckStatus();
	switch(Status)
	{
		case(BATTERY_STATUS_OK):
		{
			uint8_t Count = RTC32_GetCount();
			
			// Enable interrupts if no error
			RTC32_EnableInterrupts();
			
			/*
				Set a breakpoint here and toggle the main supply while you have connected
				an battery to the backup system. You will see that the counter still
				counts even with no supply voltage.
			*/
			break;
		}
		case(BATTERY_STATUS_NO_POWER):
		case(BATTERY_STATUS_BBPOR):
		case(BATTERY_STATUS_BBBOD):
		case(BATTERY_STATUS_XOSCFAIL):
		default:
		{			
			/*
				Initialize battery backup an error
					-> Enable failure detection
					-> 1 Hz output
					-> No high ESR mode
			*/
			Battery_Init();
			
			/*
				Initialize the RTC32
					-> 10 seconds periode
					-> 4 second compare
					-> Compare interrupt (low level)
					-> Overflow interrupt (low level)
			*/
			RTC32_Init();
			
			break;
		}
	}

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