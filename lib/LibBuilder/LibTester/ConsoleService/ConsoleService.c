/*
 * ConsoleService.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: USART Console service for AVR microcontroller.

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

/** @file Common/Services/ConsoleService/ConsoleService.c
 *  @brief USART Console service for AVR microcontroller.
 * 
 *  This file contains the implementation for the AVR USART console.
 *
 *  @author Daniel Kampert
 */

#include "Common/Services/ConsoleService/ConsoleService.h"

/** @brief	USART interface definition for the console service.
 *			NOTE: The values are chosen for a clock speed of 2 MHz or 32 MHz.
 */
static USART_Config_t _Config_USART = {
	.Device = &CONCAT(CONSOLE_STDIO),
	.DeviceMode = USART_MODE_ASYNCH,
	.Direction = USART_DIRECTION_BOTH,
	.Baudrate = 115200,
	.Parity = USART_PARITY_NONE,
	.Stop = USART_STOP_1,
	.Size = USART_SIZE_8,
	.BSCALE = -7,
	.EnableDoubleSpeed = FALSE,
};

static int USART_Put(char c, FILE* stream)
{
	USART_SendChar(_Config_USART.Device, c);
	
	return 0;
}

static int USART_Get(FILE* stream)
{
	return USART_GetChar(_Config_USART.Device);
}

static FILE USART_stdio = FDEV_SETUP_STREAM(USART_Put, USART_Get, _FDEV_SETUP_RW);

void ConsoleService_Init(void)
{
	USART_Init(&_Config_USART);

	stdout = &USART_stdio;
	stdin = &USART_stdio;
}