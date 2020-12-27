/*
 * main.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: IR remote receiver example for XMega.

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
 *  @brief IR remote receiver example for XMega.
 *
 *  Software for the XMega IR remote example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Interfaces/IR-Remote/NEC_IR.h"
#include "Services/DisplayManager/DisplayManager.h"

char DisplayBuffer[32];
IR_Message_t Message;

int main(void)
{
	/*
		Initialize the system clock
	*/
	SysClock_Init();

	/*
		Initialize the IR receiver
	*/
	IR_Init();

	/*
		Initialize the display manager
	*/
	DisplayManager_Init();
	DisplayManager_Clear();
	DisplayManager_DrawString(20, 0, "IR remote example");

	while(1)
	{
		if(IR_GetMessage(&Message))
		{
			DisplayManager_ClearLine(2);

			if(!Message.IsRepeat)
			{
				DisplayManager_DrawString(0, 8, "Button pressed!");

				// Lets do some actions according to the pressed button
				switch(Message.Data.Value)
				{
					case IR_REMOTE_KEY_1:
					{
						DisplayManager_SwitchBacklight(true);

						break;
					}
					case IR_REMOTE_KEY_9:
					{
						DisplayManager_SwitchBacklight(false);

						break;
					}
					// Ignore all other values
					default:
					{
						break;
					}
				}

				// Print the received data
				sprintf(DisplayBuffer, "Data: 0x%lX", Message.Data.Value);
				DisplayManager_DrawString(0, 16, DisplayBuffer);
			}
			else
			{
				DisplayManager_DrawString(0, 16, "Repeat");
			}
		}
	}

	return 0;
}