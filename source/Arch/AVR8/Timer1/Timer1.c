/*
 * Timer1.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 Timer1 module.

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

/** @file Arch/AVR8/Timer1/Timer1.c
 *  @brief Driver for AVR8 Timer1 module.
 *
 *  This contains the implementation of the AVR8 Timer1 driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/Timer1/Timer1.h"

#ifndef DOXYGEN
	struct
	{
		Timer_Callback_t OverflowCallback;
		Timer_Callback_t CaptureCallback;
		Timer_Callback_t CompareACallback;
		Timer_Callback_t CompareBCallback;
	} Timer1_Callbacks[TIMER1_DEVICES];
#endif

/** @brief				Timer1 interrupt handler.
 *  @param Device		Device ID
 *  @param Callback		Type of interrupt
 */
static inline void Timer1_InterruptHandler(const uint8_t Device, const Timer_CallbackType_t Callback)
{
	DisableGlobalInterrupts();
	
	if(Callback == TIMER_OVERFLOW_INTERRUPT)
	{
		if(Timer1_Callbacks[Device].OverflowCallback != NULL)
		{
			Timer1_Callbacks[Device].OverflowCallback();
		}
	}
	else if(Callback == TIMER_CAPTURE_INTERRUPT)
	{
		if(Timer1_Callbacks[Device].CaptureCallback != NULL)
		{
			Timer1_Callbacks[Device].CaptureCallback();
		}
	}
	else if(Callback == TIMER_COMPMATCH_A_INTERRUPT)
	{
		if(Timer1_Callbacks[Device].CompareACallback != NULL)
		{
			Timer1_Callbacks[Device].CompareACallback();
		}
	}
	else if(Callback == TIMER_COMPMATCH_B_INTERRUPT)
	{
		if(Timer1_Callbacks[Device].CompareBCallback != NULL)
		{
			Timer1_Callbacks[Device].CompareBCallback();
		}
	}
	
	EnableGlobalInterrupts();
}

void Timer1_Init(Timer1_Config_t* Config)
{
	  TCCR1A = (1<<WGM01); // CTC Modus
	  TCCR1B |= (1<<CS01); // Prescaler 8
	  // ((1000000/8)/1000) = 125
	  OCR1A = 125-1;
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TIMER1_OVF_vect)
	{
		Timer1_InterruptHandler(0, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TIMER1_CAPT_vect)
	{
		Timer1_InterruptHandler(0, TIMER_CAPTURE_INTERRUPT);
	}

	ISR(TIMER1_COMPA_vect)
	{
		Timer1_InterruptHandler(0, TIMER_COMPMATCH_A_INTERRUPT);
	}

	ISR(TIMER1_COMPB_vect)
	{
		Timer1_InterruptHandler(0, TIMER_COMPMATCH_B_INTERRUPT);
	}
#endif