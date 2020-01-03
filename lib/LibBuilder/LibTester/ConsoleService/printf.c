/*
 * printf.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: printf implementation for AVR microcontroller.

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

/** @file Common/Services/ConsoleService/printf.c
 *  @brief printf implementation for AVR microcontroller.
 *
 *  @author Daniel Kampert
 */

#include "Common/Services/ConsoleService/ConsoleService.h"

static uint8_t _prints(const char* String)
{
	int ByteCounter = 0x00;

	while(*String != '\0')
	{
		putchar(*String++);
		ByteCounter++;
	}
	
	return ByteCounter;
}

static uint8_t _printi(const int Integer, const int Base)
{
	char* pBuffer;
	int Integer_Temp = Integer;

	// Reserve a transmit buffer. Size depends on the type of 'Number' + array end
	char Buffer[sizeof(Integer) + 2] = {0x00};

	// Set the end of a char array
	Buffer[sizeof(Buffer) - 1] = '\0';

	// Pointer to the transmit buffer. Set the pointer to the last element before the
	// array end, because the transformation starts with the ones
	pBuffer = &Buffer[sizeof(Buffer) - 2];
	
	do
	{
		*--pBuffer = Representation[Integer_Temp % Base];
		Integer_Temp /= Base;
	}while(Integer_Temp != 0);
	
	return _prints(pBuffer);
}

int	printf(const char* _fmt, ...)
{
	int ByteCounter = 0x00;
	va_list argp;
	va_start(argp, _fmt);

	// Loop through the message string
	while(*_fmt != '\0')
	{
		if(*_fmt == '%')
		{
			// Skip '%'
			_fmt++;

			// Convert the value
			if(*_fmt == 'd')
			{
				int d = va_arg(argp, int);
				ByteCounter += _printi(d, 10);
			}
			else if(*_fmt == 'c')
			{
				int c = va_arg(argp, int);
				putchar(c);
				ByteCounter++;
			}
			else if(*_fmt == 's')
			{
				char* s = va_arg(argp, char*);
				ByteCounter += _prints(s);
			}
			else if(*_fmt == 'x')
			{
				int d = va_arg(argp, int);
				ByteCounter += _printi(d, 16);
			}
		}
		else
		{
			putchar(*_fmt);
			ByteCounter++;
		}
		
		_fmt++;
	}

	va_end(argp);

	return ByteCounter;
}