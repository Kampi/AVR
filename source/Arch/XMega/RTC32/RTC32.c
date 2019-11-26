/*
 * RTC32.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the Atmel AVR XMega RTC32 module.

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

/** @file Arch/XMega/RTC32/RTC32.c
 *  @brief Driver for Atmel AVR XMega RTC32 module.
 *
 *  This file contains the implementation of the XMega RTC32 driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/RTC32/RTC32.h"

#ifndef DOXYGEN
	struct
	{
		RTC32_Callback_t Overflow;
		RTC32_Callback_t Compare;
	} __RTC32_Callbacks;
#endif

/** @brief			RTC32 interrupt handler.
 *  @param Callback	Type of interrupt
 */
static void __RTC32_InterruptHandler(const RTC32_CallbackType_t Callback)
{	
	if(Callback == RTC32_OVFL_INTERRUPT)
	{
		if(__RTC32_Callbacks.Overflow)
		{
			__RTC32_Callbacks.Overflow();
		}
	}
	else if(Callback == RTC32_COMP_INTERRUPT)
	{
		if(__RTC32_Callbacks.Compare)
		{
			__RTC32_Callbacks.Compare();
		}
	}
}

void RTC32_Init(RTC32_Config_t* Config)
{	
	RTC32_Disable();
	
	RTC32_SetPeriod(Config->Period);
	RTC32_SetCompare(Config->Compare);
	RTC32_SetCount(Config->Count);

	RTC32_Enable();
}

void RTC32_ChangeInterruptLevel(const RTC32_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel)
{
	if(Callback & RTC32_OVFL_INTERRUPT)
	{
		RTC32.INTCTRL |= (RTC32.INTCTRL & (~InterruptLevel)) | InterruptLevel;
	}
	
	if(Callback & RTC32_COMP_INTERRUPT)
	{
		RTC32.INTCTRL |= (RTC32.INTCTRL & (~(InterruptLevel << 0x02))) | (InterruptLevel << 0x02);
	}
}

void RTC32_InstallCallback(const RTC32_InterruptConfig_t* Config)
{
	if(Config->CallbackSource & RTC32_OVFL_INTERRUPT)
	{
		RTC32.INTCTRL = (RTC32.INTCTRL & (~Config->InterruptLevel)) | Config->InterruptLevel;
		__RTC32_Callbacks.Overflow = Config->Callback;
	}
	
	if(Config->CallbackSource & RTC32_COMP_INTERRUPT)
	{
		RTC32.INTCTRL = (RTC32.INTCTRL & (~(Config->InterruptLevel << 0x02))) | (Config->InterruptLevel << 0x02);
		__RTC32_Callbacks.Compare = Config->Callback;
	}
}

void RTC32_RemoveCallback(const RTC32_CallbackType_t Callback)
{
	if(Callback & RTC32_OVFL_INTERRUPT)
	{
		__RTC32_Callbacks.Overflow = NULL;
	}
	
	if(Callback & RTC32_COMP_INTERRUPT)
	{
		__RTC32_Callbacks.Compare = NULL;
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(RTC32_COMP_vect)
	{
		__RTC32_InterruptHandler(RTC32_COMP_INTERRUPT);
	}

	ISR(RTC32_OVF_vect)
	{
		__RTC32_InterruptHandler(RTC32_OVFL_INTERRUPT);
	}
#endif