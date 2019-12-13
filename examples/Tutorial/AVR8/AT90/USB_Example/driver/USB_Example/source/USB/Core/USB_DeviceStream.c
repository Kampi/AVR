/*
 * USB_DeviceStream.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB device endpoint stream implementation.

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

/** @file USB/Core/USB_DeviceStream.c
 *  @brief USB device endpoint stream implementation.
 * 
 *  This file contains the implementation for the USB stream.
 *
 *  @author Daniel Kampert
 */

#include "USB/Core/USB_DeviceStream.h"

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