/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Example application for the bootloader application.

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
 *  @brief Example application for the bootloader application.
 *		   NOTE: This application gets mapped at start address 0x10000.
 *
 *  Software for the XMega bootloader example from
 *  https://www.kampis-elektroecke.de/2020/03/xmega-bootloader
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/GPIO/GPIO.h"

int main(void)
{
	/*
		Configure LED0
			-> PORTR.0 
			-> Output
	*/
	GPIO_SetDirection(&PORTR, 0, GPIO_DIRECTION_OUT);
	
	while(1) 
	{
		GPIO_Toggle(&PORTR, 0);
		for(uint16_t i = 0x00; i < 0xFFFF; i++);
	}
}