/*
 * USART_Interrupt.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega TWI

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

/** @file XMega/USART/USART_Interrupt.c
 *  @brief Driver for XMega USART module.
 *
 *  This file contains the implementation of the interrupt functions for the XMega USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/USART/USART.h"

#ifndef DOXYGEN
	struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t BufferOverflow;
	} __USART_Callbacks[USART_DEVICES][USART_CHANNEL];

	struct
	{
		SPI_Callback_t CompleteInterrupt;
	} __USART_SPI_Callbacks[USART_DEVICES][USART_CHANNEL];

	RingBuffer_t __USART_RxRingBuffer[USART_DEVICES][USART_CHANNEL];

	Bool_t __USART_Echo[USART_DEVICES][USART_CHANNEL];
	Bool_t __USART_IsSPI[USART_DEVICES][USART_CHANNEL];
	
	USART_Message_t __USART_Messages[USART_DEVICES][USART_CHANNEL];
	SPI_Message_t __SPI_Messages[USART_DEVICES][USART_CHANNEL];
#endif

/** @brief			USART interrupt handler.
 *  @param Device	Device ID
 *  @param Channel	USART channel
 *  @param Callback	Type of interrupt
 */
static void __USART_InterruptHandler(const uint8_t Device, const uint8_t Channel, const USART_CallbackType_t Callback)
{
	if(__USART_IsSPI[Device][Channel] == TRUE)
	{
		if(Callback == USART_TXC_INTERRUPT)
		{
			if(__SPI_Messages[Device][Channel].BytesProcessed < (__SPI_Messages[Device][Channel].Length - 1))
			{
				((USART_t*)(__SPI_Messages[Device][Channel].Device))->DATA = __SPI_Messages[Device][Channel].BufferOut[++__SPI_Messages[Device][Channel].BytesProcessed];
			}
			else
			{
				USART_SPI_DeselectDevice(__SPI_Messages[Device][Channel].Port, __SPI_Messages[Device][Channel].Pin);
				
				if(__USART_SPI_Callbacks[Device][Channel].CompleteInterrupt != NULL)
				{
					__USART_SPI_Callbacks[Device][Channel].CompleteInterrupt();
				}
			
				__SPI_Messages[Device][Channel].Status = SPI_MESSAGE_COMPLETE;
			}
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{
			__SPI_Messages[Device][Channel].BufferIn[__SPI_Messages[Device][Channel].BytesProcessed] = ((USART_t*)(__SPI_Messages[Device][Channel].Device))->DATA;
		}
	}
	else
	{
		if(Callback == USART_DRE_INTERRUPT)
		{
			if (__USART_Callbacks[Device][Channel].EmptyCallback)
			{
				__USART_Callbacks[Device][Channel].EmptyCallback();
			}

			if(!(RingBuffer_IsEmpty(__USART_Messages[Device][Channel].Ptr_TxRingBuffer)))
			{
				__USART_Messages[Device][Channel].Device->DATA = RingBuffer_Load(__USART_Messages[Device][Channel].Ptr_TxRingBuffer);
			}
			else
			{
				__USART_Messages[Device][Channel].Device->CTRLA &= ~0x03;
			}
		}
		else if(Callback == USART_TXC_INTERRUPT)
		{
			if (__USART_Callbacks[Device][Channel].TxCallback)
			{
				__USART_Callbacks[Device][Channel].TxCallback();
			}
		}
		else if(Callback == USART_RXC_INTERRUPT)
		{
			uint8_t ReceivedByte = __USART_Messages[Device][Channel].Device->DATA;
		
			// Echo message if enabled
			if(__USART_Echo[Device][Channel] == TRUE)
			{
				__USART_Messages[Device][Channel].Device->DATA = ReceivedByte;
			}
		
			// Save the data in the buffer
			RingBuffer_Save(&__USART_RxRingBuffer[Device][Channel], ReceivedByte);
		
			if (__USART_Callbacks[Device][Channel].RxCallback)
			{
				__USART_Callbacks[Device][Channel].RxCallback();
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
		__USART_InterruptHandler(USARTC_ID, 0, USART_RXC_INTERRUPT);
	}
 
	ISR(USARTC0_TXC_vect)
	{
		__USART_InterruptHandler(USARTC_ID, 0, USART_TXC_INTERRUPT);
	}
 
	ISR(USARTC0_DRE_vect) 
	{
		__USART_InterruptHandler(USARTC_ID, 0, USART_DRE_INTERRUPT);
	}
 
	ISR(USARTD0_RXC_vect)
	{
		__USART_InterruptHandler(USARTD_ID, 0, USART_RXC_INTERRUPT);
	}
	
	ISR(USARTD0_TXC_vect)
	{
		__USART_InterruptHandler(USARTD_ID, 0, USART_TXC_INTERRUPT);
	}
	
	ISR(USARTD0_DRE_vect)
	{
		__USART_InterruptHandler(USARTD_ID, 0, USART_DRE_INTERRUPT);
	}
	
	ISR(USARTE0_RXC_vect)
	{
		__USART_InterruptHandler(USARTE_ID, 0, USART_RXC_INTERRUPT);
	}
	
	ISR(USARTE0_TXC_vect)
	{
		__USART_InterruptHandler(USARTE_ID, 0, USART_TXC_INTERRUPT);
	}
	
	ISR(USARTE0_DRE_vect)
	{
		__USART_InterruptHandler(USARTE_ID, 0, USART_DRE_INTERRUPT);
	}

	#if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		ISR(USARTC1_RXC_vect)
		{
			__USART_InterruptHandler(USARTC_ID, 1, USART_RXC_INTERRUPT);
		}
 
		ISR(USARTC1_TXC_vect)
		{
			__USART_InterruptHandler(USARTC_ID, 1, USART_TXC_INTERRUPT);
		}
 
		ISR(USARTC1_DRE_vect)
		{
			__USART_InterruptHandler(USARTC_ID, 1, USART_DRE_INTERRUPT);
		}
 
		ISR(USARTD1_RXC_vect)
		{
			__USART_InterruptHandler(USARTD_ID, 1, USART_RXC_INTERRUPT);
		}
 
		ISR(USARTD1_TXC_vect)
		{
			__USART_InterruptHandler(USARTD_ID, 1, USART_TXC_INTERRUPT);
		}
 
		ISR(USARTD1_DRE_vect)
		{
			__USART_InterruptHandler(USARTD_ID, 1, USART_DRE_INTERRUPT);
		}
 
		ISR(USARTF0_RXC_vect)
		{
			__USART_InterruptHandler(USARTF_ID, 0, USART_RXC_INTERRUPT);
		}
		
		ISR(USARTF0_TXC_vect)
		{
			__USART_InterruptHandler(USARTF_ID, 0, USART_TXC_INTERRUPT);
		}
		
		ISR(USARTF0_DRE_vect)
		{
			__USART_InterruptHandler(USARTF_ID, 0, USART_DRE_INTERRUPT);
		}
	#endif
#endif