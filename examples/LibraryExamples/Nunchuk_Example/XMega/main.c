/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Nunchuk example for XMega.

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
 *  @brief Nunchuk example for XMega.
 *
 *  Software for the XMega Nunchuk example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/Nunchuk/Nunchuk.h"
#include "Services/DisplayManager/DisplayManager.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &NUNCHUK_INTERFACE,
	.Bitrate = NUNCHUK_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

Nunchuk_Data_t NewData;

uint8_t x;
uint8_t y;

uint8_t NeutralX;
uint8_t NeutralY;

int main(void)
{
	/*
		Initialize the system clock
	*/
	SysClock_Init();

	/*
		Initialize the display manager
	*/
	DisplayManager_Init();

	/*
		Initialize the Nunchuck driver
	*/
	Nunchuk_Init(&Config_I2CM);

	// Get the neutral position
	Nunchuk_Read(&NewData);
	NeutralX = NewData.Joy_X;
	NeutralY = NewData.Joy_Y;

    while(1)
    {
		Nunchuk_Read(&NewData);

		// Clear the screen when the "C" button is pressed
		if(NewData.Button_C)
		{
			x = 0x00;
			y = 0x00;
			DisplayManager_Clear();
		}		
		else
		{
			if(NewData.Joy_X > NeutralX)
			{
				if(x < (DISPLAYMANAGER_LCD_WIDTH - 0x01))
				{
					x++;
				}
			}
			else if(NewData.Joy_X < NeutralX)
			{
				if(x)
				{
					x--;
				}
			}

			if(NewData.Joy_Y < NeutralY)
			{
				if(y < (DISPLAYMANAGER_LCD_HEIGHT - 0x01))
				{
					y++;
				}
			}
			else if(NewData.Joy_Y > NeutralY)
			{
				if(y > 0)
				{
					y--;
				}
			}

			// Only draw when the "Z" button isn´t pressed
			if(!NewData.Button_Z)
			{
				DisplayManager_DrawPixel(x, y, PIXELMASK_SET);
			}
		}
		
		_delay_ms(50);
    }
}