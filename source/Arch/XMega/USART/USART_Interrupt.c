/*
 * USART_Interrupt.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt driver for Atmel AVR8 XMega USART module.

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

/** @file Arch/XMega/USART/USART_Interrupt.c
 *  @brief Interrupt driver for Atmel AVR8 XMega USART module.
 *
 *  This file contains the implementation of the interrupt functions for the Atmel AVR8 XMega USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/USART/USART.h"

#ifndef DOXYGEN
	extern struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t BufferOverflow;
	} _USART_Callbacks[USART_DEVICES][USART_CHANNEL];

	extern struct
	{
		SPI_Callback_t CompleteInterrupt;
	} _USART_SPI_Callbacks[USART_DEVICES][USART_CHANNEL];

	extern bool _USART_Echo[USART_DEVICES][USART_CHANNEL];
	extern USART_Message_t _USART_Messages[USART_DEVICES][USART_CHANNEL];

	bool _USART_IsSPI[USART_DEVICES][USART_CHANNEL];	
	SPI_Message_t _SPI_Messages[USART_DEVICES][USART_CHANNEL];
#endif

/** @brief			USART interrupt handler.
 *  @param Device	Device ID
 *  @param Channel	USART channel
 *  @param Callback	Type of interrupt
 */
static void _USART_InterruptHandler(const uint8_t Device, const uint8_t Channel, const USART_CallbackType_t Callback)
{
	if(_USART_IsSPI[Device][Channel] == true)
	{
		if(Callback == USART_TXC_INTERRUPT)
		{
			if(_SPI_Messages[Device][Channel].BytesProcessed < (_SPI_Messages[Device][Channel].Length - 1))
			{
				((USART_t*)(_SPI_Messages[Device][Channel].Device))->DATA = _SPI_Messages[Device][Channel].BufferOut[++_SPI_Messages[Device][Channel].BytesProcessed];
			}
			else
			{
				USART_SPI_DeselectDevice(_SPI_Messages[Device][Channel].Port, _SPI_Messages[Device][Channel].Pin);
				
				if(_USART_SPI_Callbacks[Device][Channel].CompleteInterrupt != NULL)
				{
					_USART_SPI_Callbacks[Device][Channel].CompleteInterrupt();
				}

				_SPI_Messages[Device][Channel].Status = SPI_MESSAGE_COMPLETE;
			}
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{
			_SPI_Messages[Device][Channel].BufferIn[_SPI_Messages[Device][Channel].BytesProcessed] = ((USART_t*)(_SPI_Messages[Device][Channel].Device))->DATA;
		}
	}
	else
	{
		if(Callback == USART_DRE_INTERRUPT)
		{
			if (_USART_Callbacks[Device][Channel].EmptyCallback)
			{
				_USART_Callbacks[Device][Channel].EmptyCallback();
			}

			if(!(RingBuffer_IsEmpty(_USART_Messages[Device][Channel].Ptr_TxRingBuffer)))
			{
				_USART_Messages[Device][Channel].Device->DATA = RingBuffer_Load(_USART_Messages[Device][Channel].Ptr_TxRingBuffer);
			}
			else
			{
				_USART_Messages[Device][Channel].Device->CTRLA &= ~0x03;
			}
		}
		else if(Callback == USART_TXC_INTERRUPT)
		{
			if (_USART_Callbacks[Device][Channel].TxCallback)
			{
				_USART_Callbacks[Device][Channel].TxCallback();
			}
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{		
			// Echo message when enabled
			if(_USART_Echo[Device][Channel] == true)
			{
				_USART_Messages[Device][Channel].Device->DATA = _USART_Messages[Device][Channel].Device->DATA;
			}

			if (_USART_Callbacks[Device][Channel].RxCallback)
			{
				_USART_Callbacks[Device][Channel].RxCallback();
			}
		}
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(USARTC0_RXC_vect) 
	{
		_USART_InterruptHandler(USARTC_ID, 0, USART_RXC_INTERRUPT);
	}
 
	ISR(USARTC0_TXC_vect)
	{
		_USART_InterruptHandler(USARTC_ID, 0, USART_TXC_INTERRUPT);
	}
 
	ISR(USARTC0_DRE_vect) 
	{
		_USART_InterruptHandler(USARTC_ID, 0, USART_DRE_INTERRUPT);
	}
 
	ISR(USARTD0_RXC_vect)
	{
		_USART_InterruptHandler(USARTD_ID, 0, USART_RXC_INTERRUPT);
	}
	
	ISR(USARTD0_TXC_vect)
	{
		_USART_InterruptHandler(USARTD_ID, 0, USART_TXC_INTERRUPT);
	}
	
	ISR(USARTD0_DRE_vect)
	{
		_USART_InterruptHandler(USARTD_ID, 0, USART_DRE_INTERRUPT);
	}
	
	ISR(USARTE0_RXC_vect)
	{
		_USART_InterruptHandler(USARTE_ID, 0, USART_RXC_INTERRUPT);
	}
	
	ISR(USARTE0_TXC_vect)
	{
		_USART_InterruptHandler(USARTE_ID, 0, USART_TXC_INTERRUPT);
	}
	
	ISR(USARTE0_DRE_vect)
	{
		_USART_InterruptHandler(USARTE_ID, 0, USART_DRE_INTERRUPT);
	}

	#if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		ISR(USARTC1_RXC_vect)
		{
			_USART_InterruptHandler(USARTC_ID, 1, USART_RXC_INTERRUPT);
		}
 
		ISR(USARTC1_TXC_vect)
		{
			_USART_InterruptHandler(USARTC_ID, 1, USART_TXC_INTERRUPT);
		}
 
		ISR(USARTC1_DRE_vect)
		{
			_USART_InterruptHandler(USARTC_ID, 1, USART_DRE_INTERRUPT);
		}
 
		ISR(USARTD1_RXC_vect)
		{
			_USART_InterruptHandler(USARTD_ID, 1, USART_RXC_INTERRUPT);
		}
 
		ISR(USARTD1_TXC_vect)
		{
			_USART_InterruptHandler(USARTD_ID, 1, USART_TXC_INTERRUPT);
		}
 
		ISR(USARTD1_DRE_vect)
		{
			_USART_InterruptHandler(USARTD_ID, 1, USART_DRE_INTERRUPT);
		}
 
		ISR(USARTF0_RXC_vect)
		{
			_USART_InterruptHandler(USARTF_ID, 0, USART_RXC_INTERRUPT);
		}
		
		ISR(USARTF0_TXC_vect)
		{
			_USART_InterruptHandler(USARTF_ID, 0, USART_TXC_INTERRUPT);
		}
		
		ISR(USARTF0_DRE_vect)
		{
			_USART_InterruptHandler(USARTF_ID, 0, USART_DRE_INTERRUPT);
		}
	#endif
#endif