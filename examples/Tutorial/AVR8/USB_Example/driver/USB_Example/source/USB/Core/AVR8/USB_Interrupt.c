/*
 * USB_Interrupt.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt functions for AT90USB1287 USB.

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

/** @file USB/Core/AVR8/USB_Interrupt.c
 *  @brief Interrupt functions for AT90USB1287 USB.
 * 
 *  This file contains the implementation of the AVR8 AT90USB1287 driver interrupts.
 *
 *  @author Daniel Kampert
 */

#include "USB/Core/AVR8/Endpoint.h"
#include "USB/Core/AVR8/USB_Interrupt.h"
#include "USB/Core/AVR8/USB_DeviceController.h"

volatile USB_State_t __DeviceState;
USB_DeviceCallbacks_t __USBEvents;

ISR(USB_GEN_vect)
{
	if(USBController_CheckForInterrupt(USB_VBUS_INTERRUPT) && USBController_IsInterruptEnabled(USB_VBUS_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_VBUS_INTERRUPT);

		if(USBController_CheckVBUS())
		{
			USBController_EnablePLL();
			__DeviceState = USB_STATE_POWERED;
			
			if(__USBEvents.ConnectWithBus != NULL)
			{
				__USBEvents.ConnectWithBus();
			}
		}
		else
		{
			USBController_DisablePLL();
			__DeviceState = USB_STATE_UNATTACHED;
			
			if(__USBEvents.DisconnectFromBus != NULL)
			{
				__USBEvents.DisconnectFromBus();
			}
		}
	}


	if(USBController_CheckForInterrupt(USB_EOR_INTERRUPT) && USBController_IsInterruptEnabled(USB_EOR_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_EOR_INTERRUPT);

		__DeviceState = USB_STATE_RESET;

		// Configure the default control endpoint
		if(Endpoint_Configure(0, ENDPOINT_TYPE_CONTROL, Endpoint_ControlSize, 0))
		{
			PORTD |= (0x01 << 0x05);
		}
		else
		{
			PORTD |= (0x01 << 0x04);
		}
	}
}