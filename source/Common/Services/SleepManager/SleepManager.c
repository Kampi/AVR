/*
 * SleepManager.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Sleep manager service,

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

/** @file Common/Services/SleepManager/SleepManager.c
 *  @brief AVR Sleep manager services.
 * 
 *  This file contains the implementation for the AVR sleep manager service.
 *
 *  @author Daniel Kampert
 */

#include "Common/Services/SleepManager/SleepManager.h"

static uint8_t __Sleep_Locks[SLEEP_MODES + 1];

void SleepManager_Init(void)
{
	for(uint8_t i = 0x00; i < SLEEP_MODES; i++)
	{
		__Sleep_Locks[i] = 0x00;
	}
	 
	__Sleep_Locks[SLEEP_MODES] = 0x01;
}

const SleepMgr_Modes_t SleepManager_GetSleepMode(void)
{
	SleepMgr_Modes_t Mode;
	uint8_t* Temp_Ptr = __Sleep_Locks;
	
	while(!(*Temp_Ptr))
	{
		Temp_Ptr++;
		Mode = (SleepMgr_Modes_t)(Mode + 1);
	}
	
	return Mode;
}

void SleepManager_Lock(const SleepMgr_Modes_t Mode)
{
	if(__Sleep_Locks[Mode] != 0xFF)
	{
		__Sleep_Locks[Mode]++;
	}
}

void SleepManager_Unlock(const SleepMgr_Modes_t Mode)
{
	if(__Sleep_Locks[Mode] != 0x00)
	{
		__Sleep_Locks[Mode]--;
	}
}

void SleepManager_EnterSleep(void)
{
	DisableGlobalInterrupts();
	 
	switch(SleepManager_GetSleepMode())
	{
		case SLEEPMGR_ACTIVE:
		{
			EnableGlobalInterrupts();
			return;
		}
		case SLEEPMGR_IDLE:
		{
			EnableSleep(SLEEPMODE_IDLE);
			return;
		}
		case SLEEPMGR_PDOWN:
		{
			EnableSleep(SLEEPMODE_PDOWN);
			return;
		}
		case SLEEPMGR_PSAVE:
		{
			EnableSleep(SLEEPMODE_PSAVE);
			return;
		}
		case SLEEPMGR_STDBY:
		{
			EnableSleep(SLEEPMODE_STDBY);
			return;
		}
		case SLEEPMGR_ESTDBY:
		{
			EnableSleep(SLEEPMODE_ESTDBY);
			return;
		}
	}	
}