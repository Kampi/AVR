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

/** @brief			Print a character string.
 *  @param String	Pointer to character string
 *  @return			Number of bytes
 */
static uint8_t __prints(const char* String)
{
	int ByteCounter = 0x00;

	while(*String != '\0')
	{
		putchar(*String++);
		ByteCounter++;
	}
	
	return ByteCounter;
}

/** @brief			Print a integer.
 *  @param Integer	Integer value
 *  @param Base		Value base
 *  @return			Number of bytes
 */
static uint8_t __printi(const int Integer, const int Base)
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
	
	return __prints(pBuffer);
}

/** @brief			Print a floating point value.
 *  @param Float	Float value
 *  @param Length	Length of value string
 *  @param Decimal	Decimal places
 *  @return			Number of bytes
 */
static uint8_t __printf(const double Float, const int Length, const int Decimal)
{
	int ByteCounter = 0x00;
	
	return ByteCounter;
}


int	printf(const char* __fmt, ...)
{
	int ByteCounter = 0x00;
	va_list argp;
	va_start(argp, __fmt);

	// Loop through the message string
	while(*__fmt != '\0')
	{
		if(*__fmt == '%')
		{
			// Skip '%'
			__fmt++;

			// Convert the value
			if(*__fmt == 'd')
			{
				int d = va_arg(argp, int);
				ByteCounter += __printi(d, 10);
			}
			else if(*__fmt == 'c')
			{
				int c = va_arg(argp, int);
				putchar(c);
				ByteCounter++;
			}
			else if(*__fmt == 's')
			{
				char* s = va_arg(argp, char*);
				ByteCounter += __prints(s);
			}
			else if(*__fmt == 'x')
			{
				int d = va_arg(argp, int);
				ByteCounter += __printi(d, 16);
			}
			else if(*__fmt == 'f')
			{
				double d = va_arg(argp, double);
				ByteCounter += __printf(d, 0, 0);
			}
		}
		else
		{
			putchar(*__fmt);
			ByteCounter++;
		}
		
		__fmt++;
	}

	va_end(argp);

	return ByteCounter;
}