/*
 * SysClock.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega system clock.

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

/** @file Arch/XMega/ClockManagement/SysClock.c
 *  @brief Driver for Atmel AVR XMega clock system. 
 *
 *   This file contains the implementation of the Atmel AVR XMega clock system.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "Arch/XMega/ClockManagement/SysClock.h"

uint32_t SysClock_GetClockPer4(void)
{
	uint32_t Clock = SysClock_GetClock();
	 
	switch(CLK.PSCTRL >> 0x02)
	{
		case(CLK_PSADIV_1_gc):
		{
			return Clock;
		}
		case(CLK_PSADIV_2_gc):
		{
			return Clock >> 0x01;
		}
		case(CLK_PSADIV_4_gc):
		{
			return Clock >> 0x02;
		}
		case(CLK_PSADIV_8_gc):
		{
			return Clock >> 0x03;
		}
		case(CLK_PSADIV_16_gc):
		{
			return Clock >> 0x04;
		}
		case(CLK_PSADIV_32_gc):
		{
			return Clock >> 0x05;
		}
		case(CLK_PSADIV_64_gc):
		{
			return Clock >> 0x06;
		}
		case(CLK_PSADIV_128_gc):
		{
			return Clock >> 0x07;
		}
		case(CLK_PSADIV_256_gc):
		{
			return Clock >> 0x08;
		}
		case(CLK_PSADIV_512_gc):
		{
			return Clock >> 0x09;
		}
		default:
		{
			// Invalid case
			return 0;
		}
	}
}
 
uint32_t SysClock_GetClockPer2(void)
{
	uint32_t Clock = SysClock_GetClockPer4();

	switch((CLK.PSCTRL & 0x03))
	{
		case(CLK_PSBCDIV_4_1_gc):
		{
			return Clock >> 0x02;
		}
		case(CLK_PSBCDIV_2_2_gc):
		{
			return Clock >> 0x01;
		}
		default:
		{
			return Clock;
		}
	}
}
 
uint32_t SysClock_GetClockPer(void)
{
	uint32_t Clock = SysClock_GetClockPer2();

	switch(CLK.PSCTRL & 0x01)
	{
		case(CLK_PSBCDIV_1_1_gc):
		{
			return Clock;
		}
		case(CLK_PSBCDIV_1_2_gc):
		{
			return Clock >> 0x01;
		}
		default:
		{
			return 0;
		}
	 }
 }