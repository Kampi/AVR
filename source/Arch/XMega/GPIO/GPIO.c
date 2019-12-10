/*
 * GPIO.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega GPIO.

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

/** @file @file Arch/XMega/GPIO/GPIO.c
 *  @brief Driver for Atmel AVR XMega GPIO.
 *
 *  This file contains the implementation of the Atmel AVR XMega GPIO driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/GPIO/GPIO.h"

/** @brief	ID declaration for the different MCU types
 */
#define GPIO_PORT_A		0							/**< Port A */
#define GPIO_PORT_B		1							/**< Port B */
#define GPIO_PORT_C		2							/**< Port C */
#define GPIO_PORT_D		3							/**< Port D */
#define GPIO_PORT_E		4							/**< Port E */
#define GPIO_PORT_F		5							/**< Port F */
#define GPIO_PORT_R		6							/**< Port R */

#ifndef DOXYGEN
	struct
	{
		GPIO_Callback_t Int0;
		GPIO_Callback_t Int1;
	} GPIO_Callbacks[GPIO_PORT_COUNT];
#endif

/** @brief			GPIO interrupt handler
 *  @param Port		GPIO port
 *  @param Channel	Interrupt channel
 */
static void __GPIO_InterruptHandler(uint8_t Port, GPIO_InterruptChannel_t Channel)
{
	if(Channel == GPIO_INTERRUPT_0)
	{
		if(GPIO_Callbacks[Port].Int0)
		{
			GPIO_Callbacks[Port].Int0();
		}
	}
	else if(Channel == GPIO_INTERRUPT_1)
	{
		if(GPIO_Callbacks[Port].Int1)
		{
			GPIO_Callbacks[Port].Int1();
		}		
	}
}

void GPIO_InstallCallback(GPIO_InterruptConfig_t* Config)
{
	uint8_t Port = 0x00;

	if(Config->Port == &PORTA)
	{
		Port = GPIO_PORT_A;
	}
	else if(Config->Port == &PORTB)
	{
		Port = GPIO_PORT_B;
	}
	else if(Config->Port == &PORTC)
	{
		Port = GPIO_PORT_C;
	}
	else if(Config->Port == &PORTD)
	{
		Port = GPIO_PORT_D;
	}
	else if(Config->Port == &PORTE)
	{
		Port = GPIO_PORT_E;
	}
	else if(Config->Port == &PORTF)
	{
		Port = GPIO_PORT_F;
	}
	else if(Config->Port == &PORTR)
	{
		Port = GPIO_PORT_R;
	}

	if(Config->Channel & GPIO_INTERRUPT_0)
	{
		Config->Port->INTCTRL |= (Config->Port->INTCTRL & ~PORT_INT0LVL_gm) | Config->InterruptLevel;
		Config->Port->INT0MASK |= (Config->Port->INT0MASK & (~(0x01 << Config->Pin))) | (0x01 << Config->Pin);
		GPIO_Callbacks[Port].Int0 = Config->Callback;
	}
	
	if(Config->Channel & GPIO_INTERRUPT_1)
	{
		Config->Port->INTCTRL |= (Config->Port->INTCTRL & ~PORT_INT1LVL_gm) | (Config->InterruptLevel << 0x03);
		Config->Port->INT1MASK |= (Config->Port->INT1MASK & (~(0x01 << Config->Pin))) | (0x01 << Config->Pin);
		GPIO_Callbacks[Port].Int1 = Config->Callback;
	}
	
	GPIO_SetConfig(Config->Port, Config->Pin, Config->Sense | Config->PullConfig);
}

void GPIO_RemoveCallback(PORT_t* Port, const GPIO_InterruptChannel_t Channel)
{
	if(Channel & GPIO_INTERRUPT_0)
	{
		Port->INTCTRL &= ~0x03;
	}
	else if(Channel & GPIO_INTERRUPT_0)
	{
		Port->INTCTRL &= ~0x03;
	}
}

void GPIO_ChangeInterruptLevel(PORT_t* Port, const uint8_t Pin, const GPIO_InterruptChannel_t Channel, const Interrupt_Level_t InterruptLevel)
{
	if(Channel && GPIO_INTERRUPT_0)
	{
		Port->INTCTRL = (Port->INTCTRL & ~PORT_INT0LVL_gm) | InterruptLevel;
		Port->INT0MASK = (Port->INT0MASK & (~(0x01 << Pin))) | (0x01 << Pin);
	}
	
	if(Channel && GPIO_INTERRUPT_1)
	{
		Port->INTCTRL |= (Port->INTCTRL & ~PORT_INT1LVL_gm) | (InterruptLevel << 0x03);
		Port->INT1MASK |= (Port->INT1MASK & (~(0x01 << Pin))) | (0x01 << Pin);
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(PORTA_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_A, GPIO_INTERRUPT_0);
	}

	ISR(PORTA_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_A, GPIO_INTERRUPT_1);
	}

	ISR(PORTB_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_B, GPIO_INTERRUPT_0);
	}
	ISR(PORTB_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_B, GPIO_INTERRUPT_1);
	}

	ISR(PORTC_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_C, GPIO_INTERRUPT_0);
	}

	ISR(PORTC_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_C, GPIO_INTERRUPT_1);
	}

	ISR(PORTD_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_D, GPIO_INTERRUPT_0);
	}

	ISR(PORTD_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_D, GPIO_INTERRUPT_1);
	}

	ISR(PORTE_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_E, GPIO_INTERRUPT_0);
	}

	ISR(PORTE_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_E, GPIO_INTERRUPT_1);
	}

	ISR(PORTF_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_F, GPIO_INTERRUPT_0);
	}

	ISR(PORTF_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_F, GPIO_INTERRUPT_1);
	}

	ISR(PORTR_INT0_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_R, GPIO_INTERRUPT_0);
	}

	ISR(PORTR_INT1_vect)
	{
		__GPIO_InterruptHandler(GPIO_PORT_R, GPIO_INTERRUPT_1);
	}
#endif