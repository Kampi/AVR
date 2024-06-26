/*
 * USB_DeviceStream.c
 *
 *  Copyright (C) Daniel Kampert, 2020
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

/** @file Services/USB/Core/USB_DeviceStream.c
 *  @brief USB device stream implementation for USB service.
 * 
 *  This file contains the implementation of the USB stream.
 *
 *  @author Daniel Kampert
 */

#include "Services/USB/Core/USB_DeviceStream.h"

/** @brief	Check the control endpoint for errors.
 *  @return	Error code
 */
static Endpoint_CS_ErrorCode_t USB_DeviceStream_GetControlEndpointState(void)
{
	// Cancel transmission if the device got disconnected
	if(_DeviceState == USB_STATE_UNATTACHED)
	{
		return ENDPOINT_CS_DISCONNECT;
	}
	// Cancel transmission if the device enter suspend mode
	else if(_DeviceState == USB_STATE_SUSPEND)
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

/** @brief			Wait until the device become ready.
 *  @param Timeout	Timeout in ms
 *  @return			Error code
 */
static Endpoint_DS_ErrorCode_t USB_DeviceStream_WaitReady(uint8_t Timeout)
{
	// Get the start frame
	uint16_t StartFrame = USB_Device_GetFrameNumber();

	while(Timeout)
	{
		// Report ready when IN endpoint and empty
		if((Endpoint_GetDirection() == ENDPOINT_DIRECTION_IN) && Endpoint_INReady())
		{
			return ENDPOINT_DS_NO_ERROR;
		}
		// Report ready when OUT endpoint and full
		else if((Endpoint_GetDirection() == ENDPOINT_DIRECTION_OUT) && Endpoint_OUTReceived())
		{
			return ENDPOINT_DS_NO_ERROR;
		}

		// Cancel transmission if the device got disconnected
		if(_DeviceState == USB_STATE_UNATTACHED)
		{
			return ENDPOINT_DS_DISCONNECT;
		}
		// Cancel transmission if the device enter suspend mode
		else if(_DeviceState == USB_STATE_SUSPEND)
		{
			return ENDPOINT_DS_SUSPEND;
		}
		// Cancel transmission when the endpoint is stalled
		else if(Endpoint_IsSTALL())
		{
			return ENDPOINT_DS_STALLED;
		}

		uint16_t NewFrame = USB_Device_GetFrameNumber();
		if(NewFrame != StartFrame)
		{
			StartFrame = NewFrame;
			Timeout--;
		}
	}

	// Cancel when timeout (only FULL- and HIGH-Speed)
	return ENDPOINT_DS_TIMEOUT;
}

Endpoint_CS_ErrorCode_t USB_DeviceStream_ControlIN(const void* Buffer, const uint16_t Length, const uint16_t RequestedLength)
{
	uint8_t* Buffer_Temp = (uint8_t*)Buffer;
	uint16_t Length_Temp = Length;

	if(Length > RequestedLength)
	{
		Length_Temp = RequestedLength;
	}

	while(Length_Temp)
	{
		Endpoint_CS_ErrorCode_t State = USB_DeviceStream_GetControlEndpointState();
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
			
			// Start the transmission of the data and wait for complete
			Endpoint_FlushIN();
			while(!Endpoint_INReady());
		}
	}

	// Set transmit ready
	Endpoint_FlushIN();

	// Wait for receive complete
	while(!Endpoint_OUTReceived())
	{
		Endpoint_CS_ErrorCode_t State = USB_DeviceStream_GetControlEndpointState();
		if(State != ENDPOINT_CS_NO_ERROR)
		{
			return State;
		}
	}

	// Clear the receive complete
	Endpoint_AckOUT();

	return ENDPOINT_CS_NO_ERROR;
}

Endpoint_DS_ErrorCode_t USB_DeviceStream_DataIN(const void* Buffer, const uint16_t Length, uint16_t* Offset)
{
	uint16_t Processed_Temp = 0x00;
	uint16_t Length_Temp = Length;
	uint8_t* Buffer_Temp = (uint8_t*)Buffer;

	if(Offset != NULL)
	{
		Length_Temp -= *Offset;
		Buffer_Temp += *Offset;
	}

	while(Length_Temp)
	{
		// Write the data into the transmit buffer
		if(Endpoint_IsReadWriteAllowed())
		{
			Endpoint_WriteByte(*Buffer_Temp++);
			Length_Temp--;
			Processed_Temp++;
		}
		// Buffer full
		else
		{
			// Clear the buffer
			Endpoint_FlushIN();

			// Wait until the endpoint becomes ready
			Endpoint_DS_ErrorCode_t ErrorCode = USB_DeviceStream_WaitReady(100);
			if(ErrorCode != ENDPOINT_DS_NO_ERROR)
			{
				*Offset = Processed_Temp;

				return ErrorCode;
			}
		}
	}

	// Clear the buffer and send the last packet
	Endpoint_FlushIN();

	*Offset = Processed_Temp;

	return ENDPOINT_DS_NO_ERROR;
}

Endpoint_DS_ErrorCode_t USB_DeviceStream_DataOUT(const void* Buffer, const uint16_t Length, uint16_t* Offset)
{
	uint16_t Processed_Temp = 0x00;
	uint16_t Length_Temp = Length;
	uint8_t* Buffer_Temp = (uint8_t*)Buffer;

	if(Offset != NULL)
	{
		Length_Temp -= *Offset;
		Buffer_Temp += *Offset;
	}

	while(Length_Temp)
	{
		uint16_t Transfer = 0x00;

		// Read the data from the receive buffer
		if(Endpoint_IsReadWriteAllowed())
		{
			*Buffer_Temp++ = Endpoint_ReadByte();
			Length_Temp--;
			Transfer++;
		}
		// Buffer full
		else
		{
			// Handshake the incoming data
			Endpoint_AckOUT();

			Endpoint_DS_ErrorCode_t ErrorCode = USB_DeviceStream_WaitReady(100);
			if(ErrorCode != ENDPOINT_DS_NO_ERROR)
			{
				*Offset += Processed_Temp;

				return ErrorCode;
			}
		}
	}

	// Handshake the last data packet
	Endpoint_AckOUT();

	*Offset += Processed_Temp;

	return ENDPOINT_DS_NO_ERROR;
}