/*
 * BatteryBackup.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega battery backup system

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
 *  @brief Driver for XMega battery backup system.
 *
 *  This file contains the prototypes and definitions for the XMega battery backup driver.
 *
 *  @author Daniel Kampert
 */

#include "XMega/BatteryBackup/BatteryBackup.h"

void BatteryBackup_Reset(void)
{
	// Enable R/W access
	VBAT.CTRL = VBAT_ACCEN_bm;

	// Perform reset
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		asm volatile(	"movw r30,  %0"		"\n\t"
						"ldi  r16,  %2"     "\n\t"
						"out   %3, r16"     "\n\t"
						"st     Z,  %1"     "\n\t"
						::	"r" (&VBAT.CTRL), 
							"r" (VBAT_RESET_bm), 
							"M" (CCP_IOREG_gc), 
							"i" (&CCP)
						: "r16", "r30", "r31"
					);
	}
}

void BatteryBackup_ConfigXOSC(const BatteryBackup_Clock_t Clock, const Bool_t HighESR)
{	
	BatteryBackup_Reset();
	ExtClock_32kLowPowerEnable();

	if(Clock == BATTERYBACKUP_CLOCK_1KHZ)
	{
		VBAT.CTRL |= VBAT_XOSCSEL_bm;
	}
	else
	{
		VBAT.CTRL &= ~VBAT_XOSCSEL_bm;
	}
	
	// Enable failure detection
	VBAT.CTRL |= VBAT_XOSCFDEN_bm;
	
	BatteryBackup_EnableExtOsc();
	
	// Set high ESR mode
	if(HighESR)
	{
		VBAT.CTRL |= VBAT_HIGHESR_bm;
	}
	else
	{
		VBAT.CTRL &= ~VBAT_HIGHESR_bm;
	}
	
	// Wait some time
	for(uint16_t i = 0; i < 0xFFFF; i++);
}

void BatteryBackup_WriteBackup(const BatteryBackupBackupRegister_t Register, const uint8_t Data)
{
	if(Register == BATTERYBACKUP_REGISTER0)
	{
		VBAT.BACKUP0 = Data;
	}
	else if(Register == BATTERYBACKUP_REGISTER1)
	{
		VBAT.BACKUP1 = Data;
	}
}

const uint8_t BatteryBackup_ReadBackup(const BatteryBackupBackupRegister_t Register)
{
	// ToDo
	
	return 0;
}

const BatteryBackupStatus_t BatteryBackup_Check(const Bool_t FirstStart)
{
	if(VBAT.STATUS & VBAT_BBPWR_bm)
	{
		return BATTERYBACKUP_STATUS_NO_POWER;
	}
	else 
	{
		if(VBAT.STATUS & VBAT_BBPORF_bm) 
		{
			if(FirstStart)
			{
				return BATTERYBACKUP_STATUS_INIT;
			}
			else
			{
				return BATTERYBACKUP_STATUS_BBPOR;
			}
		}
		else if(VBAT.STATUS & VBAT_BBBORF_bm)
		{
			return BATTERYBACKUP_STATUS_BBBOD;
		}
		else 
		{
			VBAT.CTRL = VBAT_ACCEN_bm;
			if(VBAT.STATUS & VBAT_XOSCFAIL_bm)
			{
				return BATTERYBACKUP_STATUS_XOSCFAIL;
			}
			else
			{
				return BATTERYBACKUP_STATUS_OK;
			}
		}
	}
}