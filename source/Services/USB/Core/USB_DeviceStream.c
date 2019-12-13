/*
 * USB_DeviceStream.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB device stream implementation for USB service.

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

/** @file Common/Services/USB/Core/USB_DeviceStream.c
 *  @brief USB device stream implementation for USB service.
 * 
 *  This file contains the implementation of the USB stream.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/AT90/USB/USB_Controller.h"

#include "Services/USB/Core/StandardRequest.h"
#include "Services/USB/Core/USB_DeviceStream.h"

 USB_SetupPacket_t __ControlRequest;
volatile USB_State_t __DeviceState;

/** @brief	Check the control endpoint for errors.
 *  @return	Error code
 */
static Endpoint_CS_State_t USB_DeviceStream_GetControlEndpointState(void)
{
	// Cancel transmission if the device got disconnected
	if(__DeviceState == USB_STATE_UNATTACHED)
	{
		return ENDPOINT_CS_DISCONNECT;
	}
	// Cancel transmission if the device enter suspend mode
	else if(__DeviceState == USB_STATE_SUSPEND)
	{
		return ENDPOINT_CS_SUSPEND;
	}
	// Cancel transmission when the host initiate a new transmission
	else if(Endpoint_SETUPReceived())
	{
		return ENDPOINT_CS_ABORT_FROM_HOST;
	}
	
	return ENDPOINT_CS_NO_ERROR;
}

/** @brief	Wait until the device become ready.
 *  @return	Error code
 */
static Endpoint_DS_ErrorCode_t USBStream_WaitUntilReady(void)
{
	uint8_t Timeout = 100;
	
	// Get the start frame
	uint16_t StartFrame = USB_Device_GetFrameNumber();

	while(1)
	{
		// Check if the IN direction is ready
		if(Endpoint_GetDirection() == ENDPOINT_DIRECTION_IN)
		{
			if(Endpoint_INReady())
			{
				return ENDPOINT_DS_NO_ERROR;
			}
		}
		// Check if the OUT direction is ready
		else
		{
			if(Endpoint_OUTReceived())
			{
				return ENDPOINT_DS_NO_ERROR;
			}
		}

		if(__DeviceState == USB_STATE_UNATTACHED)
		{
			return ENDPOINT_DS_DISCONNECT;
		}
		else if(__DeviceState == USB_STATE_SUSPEND)
		{
			return ENDPOINT_DS_SUSPEND;
		}
		else if(Endpoint_IsSTALL())
		{
			return ENDPOINT_DS_STALLED;
		}

		uint16_t NewFrame = USB_Device_GetFrameNumber();
		if(NewFrame != StartFrame)
		{
			StartFrame = NewFrame;

			if(!(Timeout--))
			{
				return ENDPOINT_DS_TIMEOUT;
			}
		}
	}
}

Endpoint_CS_State_t USB_DeviceStream_ControlIN(const void* Buffer, const uint16_t Length, const uint16_t RequestedLength)
{
	uint8_t* Buffer_Temp = (uint8_t*)Buffer;
	uint16_t Length_Temp = Length;

	if(Length > RequestedLength)
	{
		Length_Temp = RequestedLength;
	}

	while(Length)
	{
		Endpoint_CS_State_t State = USB_DeviceStream_GetControlEndpointState();
		if(State != ENDPOINT_CS_NO_ERROR)
		{
			return State;
		}
		// Cancel transmission when the host has send new data to the OUT endpoint
		else if(Endpoint_OUTReceived())
		{
			break;
		}

		// Check if the IN endpoint is ready
		if(Endpoint_INReady())
		{
			while(Length_Temp && (Endpoint_GetBytes() < ENDPOINT_CONTROL_SIZE))
			{
				Endpoint_WriteByte(pgm_read_byte(Buffer_Temp++));
				Length_Temp--;
			}

			// Start the transmission of the data
			Endpoint_FlushIN();
			while(!Endpoint_INReady());
		}
	}

	// Set transmit ready
	Endpoint_FlushIN();

	// Wait for receive complete
	while(!Endpoint_OUTReceived())
	{
		Endpoint_CS_State_t State = USB_DeviceStream_GetControlEndpointState();
		if(State != ENDPOINT_CS_NO_ERROR)
		{
			return State;
		}
	}

	// Clear the receive complete
	Endpoint_AckOUT();

	return ENDPOINT_CS_NO_ERROR;
}

uint8_t USBDataStream_IN(const void* Buffer, uint16_t Length, uint16_t* BytesProcessed)
{
	uint8_t* Buffer_Temp = ((uint8_t*)Buffer);
	uint16_t BytesInTransfer = 0x00;
	Endpoint_DS_ErrorCode_t ErrorCode = ENDPOINT_DS_NO_ERROR;

	// Wait for the endpoint to become ready
	ErrorCode = USBStream_WaitUntilReady();
	if(ErrorCode != ENDPOINT_DS_NO_ERROR)
	{
		return ErrorCode;
	}

	if(BytesProcessed != NULL)
	{
		Length -= *BytesProcessed;
		Buffer_Temp += *BytesProcessed;
	}

	while(Length)
	{
		// Check if the bank is full
		if(!(Endpoint_IsReadWriteAllowed()))
		{
			if(BytesProcessed != NULL)
			{
				*BytesProcessed += BytesInTransfer;
				return ENDPOINT_DS_INCOMPLETE;
			}

			ErrorCode = USBStream_WaitUntilReady();
			if(ErrorCode != ENDPOINT_DS_NO_ERROR)
			{
				return ErrorCode;
			}

			// Clear the bank
			Endpoint_FlushIN();
		}
		// Send data if write permissions are given
		else
		{
			Endpoint_WriteByte(*Buffer_Temp++);
			Length--;
			BytesInTransfer++;
		}
	}

	Endpoint_FlushIN();

	return ENDPOINT_DS_NO_ERROR;
}