/*
 * RTC.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega RTC.

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

/** @file Arch/XMega/RTC/RTC.c
 *  @brief Driver for Atmel AVR XMega RTC32 module.
 *
 *  This file contains the implementation of the Atmel AVR XMega RTC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/RTC/RTC.h"

#ifndef DOXYGEN
	struct
	{
		RTC_Callback_t Overflow;
		RTC_Callback_t Compare;
	} _RTC_Callbacks;
#endif

/** @brief			RTC interrupt handler.
 *  @param Callback	Type of interrupt
 */
static void _RTC_InterruptHandler(const RTC_CallbackType_t Callback)
{
	if(Callback == RTC_OVFL_INTERRUPT)
	{
		if(_RTC_Callbacks.Overflow)
		{
			_RTC_Callbacks.Overflow();
		}
	}
	else if(Callback == RTC_COMP_INTERRUPT)
	{
		if(_RTC_Callbacks.Compare)
		{
			_RTC_Callbacks.Compare();
		}
	}
}

void RTC_Init(RTC_Config_t* Config)
{
	RTC_PowerEnable();
	RTC_Enable(Config->ClockSource, Config->Prescaler);
	RTC_SetPeriod(Config->Period);
	RTC_SetCompare(Config->Compare);
	RTC_SetCount(Config->Count);
}

void RTC_ChangeInterruptLevel(const RTC_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel)
{
	if(Callback & RTC_OVFL_INTERRUPT)
	{
		RTC.INTCTRL |= (RTC.INTCTRL & (~InterruptLevel)) | InterruptLevel;
	}

	if(Callback & RTC_COMP_INTERRUPT)
	{
		RTC.INTCTRL |= (RTC.INTCTRL & (~(InterruptLevel << 0x02))) | (InterruptLevel << 0x02);
	}
}

void RTC_InstallCallback(const RTC_InterruptConfig_t* Config)
{
	if(Config->CallbackSource & RTC_OVFL_INTERRUPT)
	{
		RTC.INTCTRL = (RTC.INTCTRL & (~Config->InterruptLevel)) | Config->InterruptLevel;
		_RTC_Callbacks.Overflow = Config->Callback;
	}

	if(Config->CallbackSource & RTC_COMP_INTERRUPT)
	{
		RTC.INTCTRL = (RTC.INTCTRL & (~(Config->InterruptLevel << 0x02))) | (Config->InterruptLevel << 0x02);
		_RTC_Callbacks.Compare = Config->Callback;
	}
}

void RTC_RemoveCallback(const RTC_CallbackType_t Callback)
{
	if(Callback & RTC_OVFL_INTERRUPT)
	{
		_RTC_Callbacks.Overflow = NULL;
	}
	
	if(Callback & RTC_COMP_INTERRUPT)
	{
		_RTC_Callbacks.Compare = NULL;
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(RTC_COMP_vect)
	{
		_RTC_InterruptHandler(RTC_COMP_INTERRUPT);
	}

	ISR(RTC_OVF_vect)
	{
		_RTC_InterruptHandler(RTC_OVFL_INTERRUPT);
	}
#endif