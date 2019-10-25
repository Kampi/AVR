/*
 * USB_Controller.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB controller for AT90USB1287.

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

/** @file USB/Core/AVR8/USB_Controller.c
 *  @brief USB controller for AT90USB1287.
 * 
 *  This file contains the implementation of the AT90USB1287 USB driver.
 *
 *  @author Daniel Kampert
 */

#include "USB/Core/AVR8/USB_Controller.h"
#include "USB/Core/AVR8/USB_DeviceController.h"
#include "USB/Core/AVR8/USB_Interrupt.h"

extern volatile USB_State_t __DeviceState;

void USBController_Init(const USB_Mode_t Mode, const USB_Speed_t Speed)
{
	USBController_ResetInterface();
	USBController_EnableClk();
	USBController_EnableReg();
	USBController_EnableVBUSPad();
	
	USBController_SetMode(Mode);
	if(Mode == USB_MODE_DEVICE)
	{
		USBDevice_SetSpeed(Speed);
		
		// Enable all necessary interrupts
		USBController_EnableInterrupt(USB_VBUS_INTERRUPT);
		USBController_EnableInterrupt(USB_SUSPEND_INTERRUPT);
		USBController_EnableInterrupt(USB_EOR_INTERRUPT);
		
		// Attach the device to the bus
		USBController_Attach();
	}
}

void USBController_Disable(void)
{
	USBController_DisableAllInterrupts();
	USBController_ClearInterrupts();

	USBController_Detach();
	USBController_Disable();
}

void USBController_ResetInterface(void)
{
	USBController_Reset();
	
	__DeviceState = USB_STATE_UNATTACHED;
}