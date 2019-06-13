/*
 * USB_Controller.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB controller for AVR8 MCUs.

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

/** @file Arch/AVR8/USB/USB_Controller.c
 *  @brief USB-Controller for AVR8 MCUs.
 * 
 *  This file contains the implementation of the AVR8 USB driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/USB/USB_Controller.h"

extern volatile USB_State_t __DeviceState;
extern USB_DeviceCallbacks_t __USBEvents;

void USBController_Init(const USB_Mode_t Mode, const USB_Speed_t Speed)
{
	USBController_ResetInterface();
	USBController_EnableReg();
	USBController_EnableClk();
	USBController_EnableVBUSPad();
	
	USBController_SetMode(Mode);
	if(Mode == USB_MODE_DEVICE)
	{
		USBDevice_SetSpeed(Speed);
		
		// Enable all interrupts for plug-in detection
		USBController_EnableInterrupt(USB_VBUS_INTERRUPT);
		USBController_EnableInterrupt(USB_SUSPEND_INTERRUPT);
		USBController_EnableInterrupt(USB_EOR_INTERRUPT);
		
		// Configure the control endpoint
		Endpoint_Configure(ENDPOINT_CONTROL_ADDRESS, ENDPOINT_TYPE_CONTROL, ENDPOINT_CONTROL_SIZE, 1);
		
		// Attach the device to the bus
		USBDevice_Attach();
	}
	else if(Mode == USB_MODE_HOST)
	{
	
	}
}

void USBController_Disable(void)
{
	USBController_DisableAllInterrupts();
	USBController_ClearInterrupts();

	USBDevice_Detach();
	USBController_Disable();
}

void USBController_ResetInterface(void)
{
	USBController_Reset();
	
	__DeviceState = USB_STATE_UNATTACHED;
}