/*
 * USB.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB service.

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

/** @file Common/Services/USB/USB.c
 *  @brief USB service.
 *
 *  This file contains the implementation of the USB service.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "Common/Services/USB/USB.h"

volatile USB_State_t __DeviceState;
USB_DeviceCallbacks_t __USBEvents;

void USB_Init(const USB_DeviceCallbacks_t* Events)
{
	// Initialize the USB controller
	USBController_Init(USB_MODE_DEVICE, USB_SPEED_LOW);
	
	__USBEvents = *Events;
	__DeviceState = USB_STATE_RESET;
}

void USB_Poll(void)
{
	if(__DeviceState == USB_STATE_UNATTACHED)
	{
		return;
	}

	uint8_t Endpoint = Endpoint_GetCurrent();
	
	Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);

	if(Endpoint_SETUPReceived())
	{
		USB_Device_ControlRequest();
	}

	Endpoint_Select(Endpoint);
}

volatile USB_State_t USB_GetState(void)
{
	return __DeviceState;
}