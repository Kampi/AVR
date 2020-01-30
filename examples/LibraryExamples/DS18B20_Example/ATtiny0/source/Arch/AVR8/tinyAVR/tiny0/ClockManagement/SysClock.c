/*
 * SysClock.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR tiny0 system clock.

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

/** @file Arch/AVR8/tinyAVR/tiny0/ClockManagement/SysClock.h
 *  @brief Driver for Atmel AVR tiny0 clock system. 
 *
 *  This contains the implementation of the tiny0 clock system.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "Arch/AVR8/tinyAVR/tiny0/ClockManagement/SysClock.h"

uint32_t SysClock_GetClockPer(void)
{
	uint32_t SysClock = SysClock_GetClock();

	if(CLKCTRL.MCLKCTRLB & CLKCTRL_PEN_bm)
	{
		uint8_t Prescaler = ((CLKCTRL.MCLKCTRLB >> 0x01) & 0x0F) + 0x01;
		if(Prescaler > 0x06)
		{
			switch(Prescaler)
			{
				case(CLKCTRL_PDIV_6X_gc):
				{
					return SysClock / 0x06;
					break;
				}
				case(CLKCTRL_PDIV_10X_gc):
				{
					return SysClock / 0xA;
					break;
				}
				case(CLKCTRL_PDIV_12X_gc):
				{
					return SysClock / 0x0C;
					break;
				}
				case(CLKCTRL_PDIV_24X_gc):
				{
					return SysClock / 0x18;
					break;
				}
				case(CLKCTRL_PDIV_48X_gc):
				{
					return SysClock / 0x30;
					break;
				}
			}
		}
		else
		{
			return SysClock >> Prescaler;
		}
	}

	return SysClock;
}