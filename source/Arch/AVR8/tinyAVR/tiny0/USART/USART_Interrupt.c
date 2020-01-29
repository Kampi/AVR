/*
 * USART_Interrupt.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt driver for Atmel AVR8 tiny0 USART module.

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

/** @file Arch/AVR8/tinyAVR/tiny0/USART/USART_Interrupt.c
 *  @brief Interrupt driver for Atmel AVR8 tiny0 USART module.
 *
 *  This file contains the implementation of the interrupt functions for the Atmel AVR8 tiny0 USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/tinyAVR/tiny0/USART/USART.h"

#ifndef DOXYGEN
	extern 	struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t StartCallback;
		USART_Callback_t SyncFieldCallback;
	} __USART_Callbacks[USART_DEVICES];

	extern Bool_t __USART_Echo[USART_DEVICES];
	extern USART_Message_t __USART_Messages[USART_DEVICES];

	Bool_t __USART_IsSPI[USART_DEVICES];	
#endif

/** @brief			USART interrupt handler.
 *  @param Device	Device ID
 *  @param Channel	USART channel
 *  @param Callback	Type of interrupt
 */
static void __USART_InterruptHandler(const uint8_t Device, const USART_CallbackType_t Callback)
{
	if(__USART_IsSPI[Device] == TRUE)
	{
		if(Callback == USART_TXC_INTERRUPT)
		{
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{
		}
	}
	else
	{
		if(Callback == USART_DRE_INTERRUPT)
		{
			if (__USART_Callbacks[Device].EmptyCallback)
			{
				__USART_Callbacks[Device].EmptyCallback();
			}

			if(!(RingBuffer_IsEmpty(__USART_Messages[Device].Ptr_TxRingBuffer)))
			{
				__USART_Messages[Device].Device->TXDATAL = RingBuffer_Load(__USART_Messages[Device].Ptr_TxRingBuffer);
			}
			else
			{
				__USART_Messages[Device].Device->CTRLA &= ~0x03;
			}
		}
		else if(Callback == USART_TXC_INTERRUPT)
		{
			if (__USART_Callbacks[Device].TxCallback)
			{
				__USART_Callbacks[Device].TxCallback();
			}
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{
			// Echo message when enabled
			if(__USART_Echo[Device] == TRUE)
			{
				__USART_Messages[Device].Device->TXDATAL = __USART_Messages[Device].Device->RXDATAL;
				__USART_Messages[Device].Device->TXDATAH = __USART_Messages[Device].Device->RXDATAH;
			}
		
			if (__USART_Callbacks[Device].RxCallback)
			{
				__USART_Callbacks[Device].RxCallback();
			}
		}
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(USART0_RXC_vect) 
	{
		__USART_InterruptHandler(0, USART_RXC_INTERRUPT);
	}
 
	ISR(USART0_TXC_vect)
	{
		__USART_InterruptHandler(0, USART_TXC_INTERRUPT);
	}
 
	ISR(USART0_DRE_vect) 
	{
		__USART_InterruptHandler(0, USART_DRE_INTERRUPT);
	}

#endif