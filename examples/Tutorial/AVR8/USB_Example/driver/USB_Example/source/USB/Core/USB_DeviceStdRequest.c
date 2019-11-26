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

/** @file USB/Core/USB_DeviceStdRequest.c
 *  @brief USB device standard request handling implementation.
 *
 *  This file contains the implementation for the USB device standard request handling.
 *	NOTE: Please take a look at chapter 8 of the USB 2.0 specification if you need more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "USB/Core/USB_DeviceStdRequest.h"

uint8_t __Configuration;
volatile USB_State_t __DeviceState;

USB_DeviceCallbacks_t __USBEvents;
static USB_SetupPacket_t __ControlRequest;

void USB_Device_ControlRequest(void)
{
	uint8_t* RequestHeader = (uint8_t*)&__ControlRequest;
	
	// Get the setup packet
	for(uint8_t i = 0x00; i < sizeof(USB_SetupPacket_t); i++)
	{
		*(RequestHeader++) = Endpoint_ReadByte();
	}

	Endpoint_ClearSETUP();

	// Process the requests
	switch(__ControlRequest.bRequest)
	{
		case REQUEST_GET_STATUS:
		{
			uint16_t Status = 0x00;
			
			// Get the status of the device
			if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				// Check if remote wakeup and self powered is enabled
			}
			// Get the status of an Interface
			else if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_INTERFACE))
			{
				// Set status to zero
			}
			// Get the status of an Endpoint
			else if(__ControlRequest.bmRequestType & (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_ENDPOINT))
			{
				// Get the endpoint address
				uint8_t Endpoint = ((uint8_t)__ControlRequest.wIndex & 0x0F);

				if(Endpoint >= 7)
				{
					return;
				}

				// Check if the endpoint is stalled
				Endpoint_Select(Endpoint);
				Status = Endpoint_IsSTALL();
			}

			// Write the status information into the endpoint
			Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);
			Endpoint_WriteInteger(Status);
			Endpoint_ClearEndpointBank(__ControlRequest.bmRequestType);

			break;
		}
		case REQUEST_GET_INTERFACE:
		{
			break;
		}
		case REQUEST_SET_INTERFACE:
		{
			break;
		}
		case REQUEST_CLEAR_FEATURE:
		{
			break;
		}
		case REQUEST_SET_FEATURE:
		{
			break;
		}
		case REQUEST_SET_ADDRESS:
		{
			if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				// Get the new address from the host request (USB supports up to 127 devices)
				uint8_t Address = (__ControlRequest.wValue & 0x7F);
				
				// Save the address
 				USBDevice_SetAddress(Address);
 
				// Send the STATUS stage
				Endpoint_ClearEndpointBank(__ControlRequest.bmRequestType);
	
				// Enable the address
				USBDevice_EnableAddress();
				
				__DeviceState = USB_STATE_ADDRESSED;
			}

			break;
		}
		case REQUEST_GET_DESCRIPTOR:
		{
			if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				const void* Descriptor;
				uint16_t DescriptorSize;

				// Load the device descriptor
				DescriptorSize = USB_GetDescriptor(__ControlRequest.wValue, __ControlRequest.wIndex, &Descriptor);
				if((DescriptorSize == 0x00) && (__USBEvents.Error != NULL))
				{
					__USBEvents.Error();
				}

				// Send the descriptor
				USBControlStream_IN(Descriptor, DescriptorSize, __ControlRequest.wLength);
			}

			break;
		}
		case REQUEST_GET_CONFIGURATION:
		{
			/*
			if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				Endpoint_WriteByte(__Configuration);
				Endpoint_ClearEndpointBank(__ControlRequest.bmRequestType);
			}*/
			
			break;
		}
		case REQUEST_SET_CONFIGURATION:
		{/*
			if(__ControlRequest.bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_STANDARD | REQUEST_RECIPIENT_DEVICE))
			{
				__Configuration = (uint8_t)__ControlRequest.wValue & 0xFF;
				if(__Configuration > 0x00)
				{
					USB_DeviceDescriptor_t* Descriptor;
					
					Endpoint_ClearEndpointBank(__ControlRequest.bmRequestType);

					// Check if the desired configuration exist
					// NOTE: The configurations start with 1! Please check chapter 9.4.7 for additional information.
					if((USB_GetDescriptor((DESCRIPTOR_TYPE_CONFIGURATION << 0x08) | (__Configuration - 0x01), __ControlRequest.wIndex, (void*)&Descriptor) == 0x00) && (__USBEvents.Error != NULL))
					{
						__USBEvents.Error();
					}
					else
					{
						__DeviceState = USB_STATE_CONFIGURED;
					
						if(__USBEvents.ConfigurationChanged != NULL)
						{
							__USBEvents.ConfigurationChanged(__Configuration);
						}
					}
				}
			}*/
			
			break;	
		}
	}

	if(__USBEvents.ControlRequest != NULL)
	{
		__USBEvents.ControlRequest(__ControlRequest.bRequest, __ControlRequest.bmRequestType, __ControlRequest.wValue);
	}
}