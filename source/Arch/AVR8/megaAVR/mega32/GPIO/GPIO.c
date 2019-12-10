/*
 * GPIO.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 mega32 GPIO interface.

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

/** @file Arch/AVR8/megaAVR/GPIO/GPIO.c
 *  @brief Driver for Atmel AVR8 mega32 GPIO interface.
 *
 *  This file contains the implementation of the Atmel AVR8 mega32 GPIO driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/GPIO/GPIO.h"

#ifndef DOXYGEN
	static struct
	{
		GPIO_Callback_t Int0;
		GPIO_Callback_t Int1;
		GPIO_Callback_t Int2;
	} GPIO_Callbacks;
#endif

/** @brief			GPIO interrupt handler
 *  @param Channel	Interrupt channel
 */
static void GPIO_InterruptHandler(const GPIO_InterruptChannel_t Channel)
{
	if(Channel == GPIO_INTERRUPT_0)
	{
		if(GPIO_Callbacks.Int0 != NULL)
		{
			GPIO_Callbacks.Int0();
		}
	}
	else if(Channel == GPIO_INTERRUPT_1)
	{
		if(GPIO_Callbacks.Int1 != NULL)
		{
			GPIO_Callbacks.Int1();
		}
	}
	else if(Channel == GPIO_INTERRUPT_2)
	{
		if(GPIO_Callbacks.Int2 != NULL)
		{
			GPIO_Callbacks.Int2();
		}
	}
}

void GPIO_InstallCallback(const GPIO_InterruptConfig_t* Config)
{		
	if(Config->Channel == GPIO_INTERRUPT_0)
	{
		GPIO_Callbacks.Int0 = Config->Callback;

		MCUCR |= Config->Sense;
		GICR |= (0x01 << INT0);
	}
	else if(Config->Channel == GPIO_INTERRUPT_1)
	{
		GPIO_Callbacks.Int1 = Config->Callback;
		
		MCUCR |= (Config->Sense << 0x02);
		GICR |= (0x01 << INT1);
	}
	else if(Config->Channel == GPIO_INTERRUPT_2)
	{
		GPIO_Callbacks.Int2 = Config->Callback;

		GICR |= (0x01 << INT2);
	}
}

void GPIO_RemoveCallback(const GPIO_InterruptChannel_t Channel)
{
	if(Channel == GPIO_INTERRUPT_0)
	{
		GPIO_Callbacks.Int0 = NULL;
		GICR &= ~(0x01 << INT0);
	}
	else if(Channel == GPIO_INTERRUPT_1)
	{
		GPIO_Callbacks.Int1 = NULL;
		GICR &= ~(0x01 << INT1);
	}
	else if(Channel == GPIO_INTERRUPT_2)
	{
		GPIO_Callbacks.Int2 = NULL;
		GICR &= ~(0x01 << INT2);
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(INT0_vect)
	{
		GPIO_InterruptHandler(GPIO_INTERRUPT_0);
	}

	ISR(INT1_vect)
	{
		GPIO_InterruptHandler(GPIO_INTERRUPT_1);
	}
	
	ISR(INT2_vect)
	{
		GPIO_InterruptHandler(GPIO_INTERRUPT_2);
	}
#endif