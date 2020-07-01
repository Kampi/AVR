/*
 * USB.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB driver for AT90USB1287.

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

/** @file USB/USB.c
 *  @brief USB driver for the AT90USB1287.
 *
 *  This file contains the implementation of the USB driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "USB/USB.h"

volatile USB_State_t _DeviceState;
USB_DeviceCallbacks_t _USBEvents;

void USB_Init(const USB_Config_t* Config)
{
	// Initialize the USB controller
	USB_Controller_Init(Config->Mode, Config->Speed);

	_USBEvents = *Config->Callbacks;
	_DeviceState = USB_STATE_RESET;
}

void USB_Poll(void)
{
	if(_DeviceState == USB_STATE_UNATTACHED)
	{
		return;
	}

	// Save the current selected endpoint
	uint8_t CurrEndpoint = Endpoint_GetCurrent();

	// Switch to the control endpoint
	Endpoint_Select(ENDPOINT_CONTROL_ADDRESS);

	// Wait for a new setup packet
	if(Endpoint_SETUPReceived())
	{
		USB_Device_ControlRequest();
	}

	// Switch back to the previous endpoint
	Endpoint_Select(CurrEndpoint);
}

volatile USB_State_t USB_GetState(void)
{
	return _DeviceState;
}