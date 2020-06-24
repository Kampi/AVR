/*
 * Timer0.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 megaAVR Timer0 module.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Arch/AVR8/megaAVR/Timer0/Timer0.c
 *  @brief Driver for Atmel AVR8 megaAVR Timer0 module.
 *
 *  This contains the implementation of the megaAVR Timer0 driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/Timer/Timer.h"

#ifndef DOXYGEN
	struct
	{
		Timer_Callback_t OverflowCallback;
		Timer_Callback_t CompareCallback;
	} Timer0_Callbacks[TIMER0_DEVICES];
	
	uint8_t Period = 0x00;
#endif

/** @brief				Timer0 interrupt handler.
 *  @param Device		Device ID
 *  @param Callback		Type of interrupt
 */
static inline void Timer0_InterruptHandler(const uint8_t Device, const Timer_CallbackType_t Callback)
{
	DisableGlobalInterrupts();

	if(Callback & TIMER_OVERFLOW_INTERRUPT)
	{
		TCNT0 = Period;

		if(Timer0_Callbacks[Device].OverflowCallback != NULL)
		{
			Timer0_Callbacks[Device].OverflowCallback();
		}
	}
	else if(Callback & TIMER_COMPMATCH_INTERRUPT)
	{
		if(Timer0_Callbacks[Device].CompareCallback != NULL)
		{
			Timer0_Callbacks[Device].CompareCallback();
		}
	}

	EnableGlobalInterrupts();
}

void Timer0_Init(Timer0_Config_t* Config)
{
	Period = Config->Period;
	
	Timer0_SetPrescaler(Config->Prescaler);
	Timer0_SetMode(TIMER_MODE_0);
	Timer0_SetCompareOut(Config->CompareOutput);
	Timer0_SetCounter(Period);
}

void Timer0_WaveInit(Timer0_WaveConfig_t* Config)
{
	Period = Config->Period;
	
	Timer0_SetPrescaler(Config->Prescaler);
	Timer0_SetMode(Config->Mode);
	Timer0_SetCompareOut(Config->CompareOutput);
	Timer0_SetCounter(Period);
	Timer0_SetCompare(Config->Compare);
}

void Timer0_InstallCallback(Timer0_InterruptConfig_t* Config)
{
	if(Config->Source & TIMER_COMPMATCH_INTERRUPT)
	{
		Timer0_Callbacks[0].CompareCallback = Config->Callback;
		
		TIMSK |= (0x01 << OCIE0);
	}
	
	if(Config->Source & TIMER_OVERFLOW_INTERRUPT)
	{
		Timer0_Callbacks[0].OverflowCallback = Config->Callback;
		
		TIMSK |= (0x01 << TOIE0);
	}
}

void Timer0_RemoveCallback(const Timer_CallbackType_t Callback)
{
	if(Callback & TIMER_COMPMATCH_INTERRUPT)
	{
		Timer0_Callbacks[0].CompareCallback = NULL;
		
		TIMSK &= ~(0x01 << OCIE0);
	}
	
	if(Callback & TIMER_OVERFLOW_INTERRUPT)
	{
		Timer0_Callbacks[0].OverflowCallback = NULL;
		
		TIMSK &= ~(0x01 << TOIE0);
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TIMER0_OVF_vect)
	{
		Timer0_InterruptHandler(0, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TIMER0_COMP_vect)
	{
		Timer0_InterruptHandler(0, TIMER_COMPMATCH_INTERRUPT);
	}
#endif