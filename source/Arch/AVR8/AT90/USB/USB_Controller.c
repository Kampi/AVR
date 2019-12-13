/*
 * USB_Controller.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB controller for Atmel AVR8 MCUs.

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

/** @file Arch/AVR8/AT90/USB/USB_Controller.c
 *  @brief USB-Controller for Atmel AVR8 MCUs.
 * 
 *  This file contains the implementation of the AVR8 USB driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/AT90/USB/USB_Controller.h"

extern volatile USB_State_t __DeviceState;
extern USB_DeviceCallbacks_t __USBEvents;
extern uint8_t __Configuration;

void USB_Controller_Init(const USB_Config_t* Config)
{
	USB_Controller_ResetInterface();
	USB_Controller_EnableReg();
	USB_Controller_EnableClk();
	USB_Controller_EnableVBUSPad();

	USB_Controller_SetMode(Config->Mode);
	if(Config->Mode == USB_MODE_DEVICE)
	{
		USB_Device_SetSpeed(Config->Speed);
		
		// Enable all interrupts for plug-in detection
		USB_Controller_EnableInterrupt(USB_VBUS_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_SUSPEND_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_EOR_INTERRUPT);
		
		if(Config->EnableSOFCallbacks)
		{
			USB_Controller_EnableInterrupt(USB_SOF_INTERRUPT);
		}
		else
		{
			USB_Controller_DisableInterrupt(USB_SOF_INTERRUPT);
		}

		// Attach the device to the bus
		USB_Device_Attach();
	}
	else if(Config->Mode == USB_MODE_HOST)
	{
	
	}
}

void USB_Controller_Disable(void)
{
	USB_Controller_DisableAllInterrupts();
	USB_Controller_ClearInterrupts();

	USB_Device_Detach();
	USB_Controller_Disable();
}

void USB_Controller_ResetInterface(void)
{
	__Configuration = 0x00;

	USB_Controller_Reset();
	
	__DeviceState = USB_STATE_UNATTACHED;
}