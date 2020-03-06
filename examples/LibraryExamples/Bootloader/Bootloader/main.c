/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Bootloader example for XMega.

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
 *  @brief Bootloader example for XMega.
 *
 *  Software for the XMega bootloader example from
 *  https://www.kampis-elektroecke.de/2020/03/bootloader-fuer-avr-xmega/
 *
 *  @author Daniel Kampert
 */

#include "Bootloader/Bootloader.h"

int main(void)
{
	// Initialize the bootloader
	Bootloader_Init();

	// Enter the bootloader mode and wait for a program
	if(Bootloader_Enter() == TRUE)
	{
		Bootloader_Exit();
	}
	else
	{
	}
	
    while(1)
    {
    }

	return 0;
}