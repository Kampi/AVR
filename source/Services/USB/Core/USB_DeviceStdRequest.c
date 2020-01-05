 /*
 * USB_DeviceStdRequest.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB standard device request implementation.

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

/** @file Common/Services/USB/Core/USB_DeviceStdRequest.c
 *  @brief USB device standard request handling implementation.
 *
 *  This file contains the implementation for the USB device standard request handling.
 *	NOTE: Please take a look at chapter 8 of the USB 2.0 specification if you need more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "Services/USB/Core/USB_DeviceStdRequest.h"

static USB_SetupPacket_t _ControlRequest;

volatile USB_State_t _DeviceState;
uint8_t _Configuration;
USB_DeviceCallbacks_t _USBEvents;

void USB_Device_ControlRequest(void)
{	
	uint8_t* RequestHeader = (uint8_t*)&_ControlRequest;
	
	// Get the setup packet
	for(uint8_t i = 0x00; i < sizeof(USB_SetupPacket_t); i++)
	{
		*(RequestHeader++) = Endpoint_ReadByte();
	}

	// Process the SETUP stage
	Endpoint_ClearSETUP();

	// Handle the request
	switch(_ControlRequest.bRequest)
	{
		case REQUEST_GET_STATUS:
		{
			/*
				GET_STATUS request contains
					- SETUP stage
					- DATA stage
					- STATUS stage

				The request has a data length of 2 bytes with the recipient status.
			*/

			uint16_t Status = 0x00;

			// Get the device status
			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				// Check if remote wakeup and self powered is enabled
			}
			// Get the interface status
			else if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_INTERFACE))
			{
				// Set status to zero
			}
			// Get the endpoint status
			else if(_ControlRequest.bmRequestType & (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_ENDPOINT))
			{
				// Get the endpoint address
				uint8_t Endpoint = ((uint8_t)_ControlRequest.wIndex & 0x0F);

				if(Endpoint >= USB_TOTAL_ENDPOINTS)
				{
					return;
				}

				// Check if the endpoint is stalled
				Endpoint_Select(Endpoint);
				Status = Endpoint_IsSTALL();
			}

			// Select the control endpoint
			Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);

			// Process the DATA stage
			Endpoint_WriteInteger(Status);
			Endpoint_FlushIN();

			// Process the STATUS stage
			Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

			break;
		}
		case REQUEST_GET_INTERFACE:
		{
			/*
				GET_INTERFACE request contains
					- SETUP stage
					- DATA stage
					- STATUS stage

				The request has a data length of 1 byte with the interface number.
			*/

			// Process the DATA stage
			Endpoint_WriteByte(1);
			Endpoint_FlushIN();

			// Process the STATUS stage
			Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

			break;
		}
		case REQUEST_SET_INTERFACE:
		{
			break;
		}
		case REQUEST_CLEAR_FEATURE:
		{
			/*
				CLEAR_FEATURE request contains
					- SETUP stage
					- STATUS stage
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
			}
			else if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_INTERFACE))
			{
			}
			else if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_ENDPOINT))
			{
				if(_ControlRequest.wValue == REQUEST_FEATURE_ENDPOINT_HALT)
				{
					// Select the endpoint and clear HALT
					Endpoint_Select(_ControlRequest.wIndex);
					Endpoint_ClearSTALL();
				}
			}

			// Select the control endpoint
			Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);

			// Process the STATUS stage
			Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

			break;
		}
		case REQUEST_SET_FEATURE:
		{
			/*
				SET_FEATURE request contains
					- SETUP stage
					- STATUS stage
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				if(_ControlRequest.wValue == REQUEST_FEATURE_DEVICE_TEST)
				{
					// Select test with _ControlRequest.wIndex
				}
			}
			else if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_INTERFACE))
			{
			}
			else if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_ENDPOINT))
			{
				if(_ControlRequest.wValue == REQUEST_FEATURE_ENDPOINT_HALT)
				{
					// Select the endpoint and set HALT
				}
			}

			// Select the control endpoint
			Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);

			// Process the STATUS stage
			Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

			break;
		}
		case REQUEST_SET_ADDRESS:
		{
			/*
				SET_ADDRESS request contains
					- SETUP stage
					- STATUS stage
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				// Get the new address from the host request (USB supports up to 127 devices)
				uint8_t Address = (_ControlRequest.wValue & 0x7F);

				// Save the address
				USB_Device_SetAddress(Address);

				// Process the STATUS stage
				Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

				// Enable the new device address
				USB_Device_EnableAddress();

				_DeviceState = USB_STATE_ADDRESSED;
			}

			break;
		}
		case REQUEST_GET_DESCRIPTOR:
		{
			/*
				GET_DESCRIPTOR request contains
					- SETUP stage
					- DATA stage
					- STATUS stage
	
				The request has a data length of n byte with the descriptor data.
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE) || 
			  (_ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_INTERFACE)))
			{
				const void* Descriptor;
				uint16_t DescriptorSize;

				// Get the memory address of the requested descriptor
				Descriptor = USB_GetDescriptor(_ControlRequest.wValue, _ControlRequest.wIndex, &DescriptorSize);
				if((DescriptorSize == 0x00) || (Descriptor == NULL))
				{
					if(_USBEvents.Error != NULL)
					{
						_USBEvents.Error();
					}

					break;
				}

				// Transmit the descriptor
				USB_DeviceStream_ControlIN(Descriptor, DescriptorSize, _ControlRequest.wLength);
			}

			break;
		}
		case REQUEST_GET_CONFIGURATION:
		{
			/*
				GET_CONFIGURATION request contains
					- SETUP stage
					- STATUS stage
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				Endpoint_WriteByte(_Configuration);

				// Process the DATA stage
				Endpoint_FlushIN();

				// Process the STATUS stage
				Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);
			}

			break;
		}
		case REQUEST_SET_CONFIGURATION:
		{
			/*
				SET_DESCRIPTOR request contains
					- SETUP stage
					- STATUS stage
			*/

			if(_ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				_Configuration = (uint8_t)_ControlRequest.wValue & 0xFF;
				if(_Configuration > 0x00)
				{
					USB_DeviceDescriptor_t* Descriptor;

					// Process the STATUS stage
					Endpoint_HandleSTATUS(_ControlRequest.bmRequestType);

					// Check if the desired configuration exist
					// NOTE: The configurations start with 1! Please check chapter 9.4.7 for additional information.
					if((USB_GetDescriptor((DESCRIPTOR_TYPE_CONFIGURATION << 0x08) | (_Configuration - 0x01), _ControlRequest.wIndex, (void*)&Descriptor) == 0x00) && (_USBEvents.Error != NULL))
					{
						_USBEvents.Error();
					}
					else
					{
						_DeviceState = USB_STATE_CONFIGURED;

						if(_USBEvents.ConfigurationChanged != NULL)
						{
							_USBEvents.ConfigurationChanged(_Configuration);
						}
					}
				}
				// Put the device back into addressed state when the configuration index is zero
				else
				{
					_DeviceState = USB_STATE_ADDRESSED;
				}
			}

			break;
		}
	}

	// Call the control request event to handle the class specific requests
	if(_USBEvents.ControlRequest != NULL)
	{
		_USBEvents.ControlRequest(_ControlRequest.bRequest, _ControlRequest.bmRequestType, _ControlRequest.wValue);
	}
}