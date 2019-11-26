/*
 * Endpoint.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB endpoint for AVR8 devices.

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

/** @file USB/Core/AVR8/Endpoint.c
 *  @brief USB endpoint for AVR8 devices.
 * 
 *  This file contains the implementation of the AVR8 endpoint driver.
 *
 *  @author Daniel Kampert
 */

#include "USB/Core/AVR8/Endpoint.h"

volatile USB_State_t __DeviceState;

uint8_t Endpoint_Configure(const uint8_t Address, const Endpoint_Type_t Type, const Endpoint_Size_t Size, const uint8_t DoubleBank)
{
	uint8_t Address_Temp = Address & 0x0F;

	// Allocate the memory for the endpoints
	for(uint8_t i = Address_Temp; i < 0x07; i++)
	{
		uint8_t UECFG0X_Temp;
		uint8_t UECFG1X_Temp;
		uint8_t UEIENX_Temp;

		UENUM = Address_Temp;

		if(i == Address_Temp)
		{
			// Configure UECFG0X-register
			UECFG0X_Temp = (Type << EPTYPE0);
			
			if(Address & ENDPOINT_DIR_MASK_IN)
			{
				UECFG0X_Temp |= (0x01 << EPDIR);
			}
			
			// Configure UECFG1X-register
			if(DoubleBank > 0x01)
			{
				UECFG1X_Temp |= (0x01 << EPBK0);
			}
			
			if(Size > 0x06)
			{
				UECFG1X_Temp &= ~((0x01 << EPSIZE2) | (0x01 << EPSIZE1) | (0x01 << EPSIZE0));
			}
			else
			{
				UECFG1X_Temp |= (Size << EPSIZE0);
			}

			// Set the ALLOC bit
			UECFG1X_Temp = (0x01 << ALLOC);

			// Configure UEIENX-register
			UEIENX_Temp = 0x00;
		}
		else
		{
			UECFG0X_Temp = UECFG0X;
			UECFG1X_Temp = UECFG1X;
			UEIENX_Temp = UEIENX;
		}

		// Skip the endpoint if the memory isn´t allocated
		if(!(UECFG1X_Temp & (0x01 << ALLOC)))
		{
			continue;
		}

		// Disable the selected endpoint
		UECONX &= ~(0x01 << EPEN);
		
		// Clear the ALLOC-bit, so the endpoints will slide down
		UECFG1X &= ~(0x01 << ALLOC);

		// Configure and activate the endpoint
		// See figure 23-2 in the device data sheet
		UECONX |= (0x01 << EPEN);
		UECFG0X = UECFG0X_Temp;
		UECFG1X = UECFG1X_Temp;
		UEIENX = UEIENX_Temp;

		// Check the configuration
		if(!(UESTA0X & (0x01 << CFGOK)))
		{
			return 0x0;
		}
	}

	// Select the configured endpoint
	UENUM = Address_Temp;
	
	return 0x01;
}

void Endpoint_ClearEndpointBank(const uint8_t RequestType)
{
	// Clear the OUT endpoint
	if(RequestType & REQUEST_DIRECTION_DEVICE_TO_HOST)
	{
		while(!(Endpoint_OUTReceived()))
		{
			// Cancel if the device gets unattached
			if(__DeviceState == USB_STATE_UNATTACHED)
			{
				return;
			}
		}

		Endpoint_AckOUT();
	}
	// Clear the IN endpoint
	else
	{
		Endpoint_FlushIN();

		while(!(Endpoint_INReady()))
		{
			// Cancel if the device gets unattached
			if(__DeviceState == USB_STATE_UNATTACHED)
			{
				return;
			}
		}
	}
}