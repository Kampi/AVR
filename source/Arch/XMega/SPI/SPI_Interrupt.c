/*
 * SPI_Interrupt.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega SPI

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

/** @file XMega/SPI/SPI_Interrupt.c
 *  @brief Driver for XMega SPI module.
 *
 *  This file contains the implementation of the interrupt functions for the XMega SPI driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/SPI/SPI.h"

#ifndef DOXYGEN
	extern struct
	{
		SPI_Callback_t CompleteInterrupt;
		SPI_Callback_t ErrorCallback;
	} _SPI_Callbacks[SPI_DEVICES];

	SPI_Message_t _SPIM_Messages[SPI_DEVICES];
	SPI_DeviceMode_t _SPI_DeviceMode[SPI_DEVICES];
	SPI_Buffer_t _SPI_SlaveBuffer[SPI_DEVICES];
#endif

/** @brief			SPI interrupt handler.
 *  @param Device	Device ID
 */
static void _SPI_InterruptHandler(const uint8_t Device)
{
	SPI_t* Device_Ptr = _SPIM_Messages[Device].Device;

	// Check if master or slave mode
	if(_SPI_DeviceMode[Device] == SPI_MASTER)
	{
		// Check for error
		if(Device_Ptr->STATUS & SPI_WRCOL_bm)
		{
			_SPIM_Messages[Device].Status = SPI_MESSAGE_ERROR;
		
			if(_SPI_Callbacks[Device].ErrorCallback != NULL)
			{
				_SPI_Callbacks[Device].ErrorCallback();
			}
		}
		else
		{
			_SPIM_Messages[Device].BufferIn[_SPIM_Messages[Device].BytesProcessed] = Device_Ptr->DATA;
		
			if(_SPIM_Messages[Device].BytesProcessed < (_SPIM_Messages[Device].Length - 1))
			{
				Device_Ptr->DATA = _SPIM_Messages[Device].BufferOut[++_SPIM_Messages[Device].BytesProcessed];
			}
			else
			{
				_SPIM_Messages[Device].Status = SPI_MESSAGE_COMPLETE;

				if(_SPI_Callbacks[Device].CompleteInterrupt != NULL)
				{
					_SPI_Callbacks[Device].CompleteInterrupt();
				}
			
				SPIM_DeselectDevice(_SPIM_Messages[Device].Port, _SPIM_Messages[Device].Pin);
			}
		}
	}
	else
	{
		_SPI_SlaveBuffer[Device].RxBuffer[_SPI_SlaveBuffer[Device].BytesProcessed] = SPIC.DATA;
		
		if(_SPI_SlaveBuffer[Device].BytesProcessed < (SPI_BUFFER_SIZE - 1))
		{
			SPIC.DATA = _SPI_SlaveBuffer[Device].TxBuffer[++_SPI_SlaveBuffer[Device].BytesProcessed];
		}
		else
		{
			_SPI_SlaveBuffer[Device].BytesProcessed = 0x00;
			_SPI_SlaveBuffer[Device].Status = SPI_BUFFER_OVERFLOW;
		}
	}
}

void SPI_EnableInterruptSupport(SPI_t* Device, const Interrupt_Level_t Level)
{
	Device->INTCTRL = (Device->INTCTRL & (~0x03)) | Level;
}

void SPI_DisableInterruptSupport(SPI_t* Device)
{
	Device->INTCTRL &= ~0x03;
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(SPIC_INT_vect)
	{
		_SPI_InterruptHandler(SPIC_ID);
	}
	
	ISR(SPID_INT_vect)
	{
		_SPI_InterruptHandler(SPID_ID);
	}
	
	#if(defined SPIE_INT_vect)
		ISR(SPIE_INT_vect)
		{
			_SPI_InterruptHandler(SPIE_ID);
		}
	#endif
	
	#if(defined SPIF_INT_vect)
		ISR(SPIF_INT_vect)
		{
			_SPI_InterruptHandler(SPIF_ID);
		}
	#endif
#endif