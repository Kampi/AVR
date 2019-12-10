/*
 * Timer1.c
 *
 * Created: 11.05.2017 21:33:35
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the Atmel AVR XMega Timer type 1.

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

/** @file Arch/XMega/Timer/Timer1.c
 *  @brief Driver for the Atmel AVR XMega Timer type 1.
 *
 *  This contains the implementation of the Atmel AVR XMega Timer1 driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/Timer/Timer.h"

#ifndef DOXYGEN
	struct
	{
		Timer_Callback_t OverflowCallback;
		Timer_Callback_t ErrorCallback;
		Timer_Callback_t CCACallback;
		Timer_Callback_t CCBCallback;
		Timer_Callback_t CCCCallback;
		Timer_Callback_t CCDCallback;
	} Timer1_Callbacks[TIMER1_DEVICES];
#endif

/** @brief				Timer1 interrupt handler.
 *  @param Device		Device ID
 *  @param Callback		Type of interrupt
 */
static void __Timer1_InterruptHandler(const uint8_t Device, const Timer_CallbackType_t Callback)
{

}

void Timer1_Init(Timer1_Config_t* Config)
{
	
}

void Timer1_WaveInit(Timer1_WaveConfig_t* Config)
{
	
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TCC1_ERR_vect)
	{
		__Timer1_InterruptHandler(0, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCD1_ERR_vect)
	{
		__Timer1_InterruptHandler(1, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCE1_ERR_vect)
	{
		__Timer1_InterruptHandler(2, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCC1_OVF_vect)
	{
		__Timer1_InterruptHandler(0, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCD1_OVF_vect)
	{
		__Timer1_InterruptHandler(1, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCE1_OVF_vect)
	{
		__Timer1_InterruptHandler(2, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCC1_CCA_vect)
	{
		Timer1_ClearFlag(&TCC1, TIMER_CCA_INTERRUPT);
		__Timer1_InterruptHandler(0, TIMER_CCA_INTERRUPT);
	}

	ISR(TCC1_CCB_vect)
	{
		Timer1_ClearFlag(&TCC1, TIMER_CCB_INTERRUPT);
		__Timer1_InterruptHandler(0, TIMER_CCB_INTERRUPT);
	}

	ISR(TCD1_CCA_vect)
	{
		Timer1_ClearFlag(&TCD1, TIMER_CCA_INTERRUPT);
		__Timer1_InterruptHandler(1, TIMER_CCA_INTERRUPT);
	}

	ISR(TCD1_CCB_vect)
	{
		Timer1_ClearFlag(&TCD1, TIMER_CCB_INTERRUPT);
		__Timer1_InterruptHandler(1, TIMER_CCB_INTERRUPT);
	}

	ISR(TCE1_CCA_vect)
	{
		Timer1_ClearFlag(&TCE1, TIMER_CCA_INTERRUPT);
		__Timer1_InterruptHandler(2, TIMER_CCA_INTERRUPT);
	}

	ISR(TCE1_CCB_vect)
	{
		Timer1_ClearFlag(&TCE1, TIMER_CCB_INTERRUPT);
		__Timer1_InterruptHandler(2, TIMER_CCB_INTERRUPT);
	}
#endif