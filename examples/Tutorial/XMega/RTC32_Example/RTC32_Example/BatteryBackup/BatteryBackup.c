/*
 * BatteryBackup.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Battery backup for XMega.

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

/** @file BatteryBackup.c
 *  @brief Driver for XMega battery backup.
 *
 *  @author Daniel Kampert
 */

#include "BatteryBackup.h"

BatteryStatus_t Battery_CheckStatus(void)
{
	// Check battery backup power
	if(VBAT.STATUS & VBAT_BBPWR_bm)
	{
		// No or empty backup battery detected
		return BATTERY_STATUS_NO_POWER;
	}
	else if(VBAT.STATUS & VBAT_BBBORF_bm)
	{
		// Backup system is powered from battery
		return BATTERY_STATUS_BBBOD;
	}
	else if(VBAT.STATUS & VBAT_BBPORF_bm)
	{
		// Battery power on detected
		return BATTERY_STATUS_BBPOR;
	}
	else
	{
		// Set this bit to enable the access to all backup registers
		VBAT.CTRL = VBAT_ACCEN_bm;

		// Check the external crystal oscillator
		if(VBAT.STATUS & VBAT_XOSCFAIL_bm)
		{
			return BATTERY_STATUS_XOSCFAIL;
		}
		else
		{
			return BATTERY_STATUS_OK;
		}
	}
}

void Battery_Init(void)
{
	// Enable access
	VBAT.CTRL = VBAT_ACCEN_bm;

	// Reset the battery backup system
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		asm volatile(	"movw r30,  %0"		"\n\t"
						"ldi  r16,  %2"     "\n\t"
						"out   %3, r16"     "\n\t"
						"st     Z,  %1"     "\n\t"
						::	
						"r" (&VBAT.CTRL),
						"r" (VBAT_RESET_bm),
						"M" (CCP_IOREG_gc),
						"i" (&CCP)
						: "r16", "r30", "r31"
		);
	}

	VBAT.CTRL |= VBAT_XOSCFDEN_bm | VBAT_XOSCEN_bm;
}

void Battery_WriteRegister(uint8_t Register, uint8_t Data)
{
	if(Register == 0)
	{
		VBAT.BACKUP0 = Data;
	}
	else
	{
		VBAT.BACKUP1 = Data;
	}
}

uint8_t Battery_ReadRegister(uint8_t Register)
{
	if(Register == 0)
	{
		return VBAT.BACKUP0;
	}
	else
	{
		return VBAT.BACKUP1;
	}
}