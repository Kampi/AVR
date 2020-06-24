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

volatile USB_State_t _DeviceState;

uint8_t Endpoint_Configure(const uint8_t Address, const Endpoint_Type_t Type, const uint8_t Size, const uint8_t DoubleBank)
{
	uint8_t Address_Temp = Address & 0x0F;

	if((Address_Temp & 0x07) > MAX_ENDPOINTS)
	{
		return 0x00;
	}

	// Allocate the memory for the endpoints
	for(uint8_t i = Address_Temp; i < MAX_ENDPOINTS; i++)
	{
		uint8_t UECFG0X_Temp;
		uint8_t UECFG1X_Temp;
		uint8_t UEIENX_Temp;

		Endpoint_Select(Address_Temp);

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
			
			// Convert the endpoint size into the correct bit mask (see the datasheet for the mask values)
			uint8_t Temp = 0x08;
			uint8_t EPSIZE = 0x00;
			while(Temp < Size)
			{
				EPSIZE++;
				Temp <<= 0x01;
			}
			
			UECFG1X_Temp |= (EPSIZE << EPSIZE0);

			// Set the ALLOC bit
			UECFG1X_Temp |= (0x01 << ALLOC);

			// Configure UEIENX-register
			UEIENX_Temp = (0x01 << RXSTPE);
		}
		else
		{
			UECFG0X_Temp = UECFG0X;
			UECFG1X_Temp = UECFG1X;
			UEIENX_Temp = 0x00;
		}

		// Skip the endpoint if the memory isn´t allocated
		if(!(UECFG1X_Temp & (0x01 << ALLOC)))
		{
			continue;
		}

		// Disable the selected endpoint
		Endpoint_Disable();
		
		// Clear the ALLOC-bit, so the endpoints will slide down
		UECFG1X &= ~(0x01 << ALLOC);

		// Configure and activate the endpoint
		// See figure 23-2 in the device datasheet
		Endpoint_Enable();
		UECFG0X = UECFG0X_Temp;
		UECFG1X = UECFG1X_Temp;
		UEIENX = UEIENX_Temp;

		// Check the configuration
		if(!(UESTA0X & (0x01 << CFGOK)))
		{
			return 0x00;
		}
	}

	return 0x01;
}

void Endpoint_HandleSTATUS(const USB_RequestDirection_t Direction)
{
	// Data direction from device to host -> Status direction from host to device
	if(Direction & REQUEST_DIRECTION_DEVICE_TO_HOST)
	{
		while(!(Endpoint_OUTReceived()))
		{
			// Cancel if the device gets unattached
			if(_DeviceState == USB_STATE_UNATTACHED)
			{
				return;
			}
		}

		Endpoint_AckOUT();
	}
	// Data direction from host to device -> Status direction from device to host
	else
	{
		Endpoint_FlushIN();

		while(!(Endpoint_INReady()))
		{
			// Cancel if the device gets unattached
			if(_DeviceState == USB_STATE_UNATTACHED)
			{
				return;
			}
		}
	}
}