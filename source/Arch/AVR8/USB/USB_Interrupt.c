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

/** @file Arch/AVR8/USB/USB_Interrupt.c
 *  @brief Interrupt functions for AT90USB1287 USB.
 * 
 *  This file contains the implementation of the AT90USB1287 USB driver interrupts.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/USB/USB_Controller.h"

volatile USB_State_t __DeviceState;

extern uint8_t __Configuration;
extern USB_DeviceCallbacks_t __USBEvents;

ISR(USB_GEN_vect)
{
	if(USBController_CheckForInterrupt(USB_SOF_INTERRUPT) && USBController_IsInterruptEnabled(USB_SOF_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_SOF_INTERRUPT);

		if(__USBEvents.StartOfFrame != NULL)
		{
			__USBEvents.StartOfFrame();
		}
	}

	if(USBController_CheckForInterrupt(USB_VBUS_INTERRUPT) && USBController_IsInterruptEnabled(USB_VBUS_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_VBUS_INTERRUPT);

		if(USBController_CheckVBUS())
		{
			USBController_EnablePLL();

			__DeviceState = USB_STATE_POWERED;

			if(__USBEvents.ConnectedWithBus != NULL)
			{
				__USBEvents.ConnectedWithBus();
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

	if(USBController_CheckForInterrupt(USB_SUSPEND_INTERRUPT) && USBController_IsInterruptEnabled(USB_SUSPEND_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USBController_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USBController_EnableInterrupt(USB_WAKE_INTERRUPT);

		USBController_DisableClk();

		__DeviceState = USB_STATE_SUSPEND;
		
		if(__USBEvents.Suspend != NULL)
		{
			__USBEvents.Suspend();
		}
	}

	if(USBController_CheckForInterrupt(USB_WAKE_INTERRUPT) && USBController_IsInterruptEnabled(USB_WAKE_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_WAKE_INTERRUPT);

		USBController_DisableInterrupt(USB_WAKE_INTERRUPT);
		USBController_EnableInterrupt(USB_SUSPEND_INTERRUPT);
		
		USBController_EnableClk();

		// Check if the device is configured
		if(__Configuration > 0)
		{
			__DeviceState = USB_STATE_CONFIGURED;
		}
		else
		{
			// Check if the device got an address from the host
			if(USBDevice_IsAddressEnabled())
			{
				__DeviceState = USB_STATE_ADDRESSED;
			}
			// No configuration or address set
			else
			{
				__DeviceState = USB_STATE_POWERED;
			}
		}
	
		if(__USBEvents.Wake != NULL)
		{
			__USBEvents.Wake();
		}
	}

	if(USBController_CheckForInterrupt(USB_EOR_INTERRUPT) && USBController_IsInterruptEnabled(USB_EOR_INTERRUPT))
	{
		USBController_ClearInterruptFlag(USB_EOR_INTERRUPT);
		USBController_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USBController_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USBController_EnableInterrupt(USB_WAKE_INTERRUPT);

		Endpoint_Configure(ENDPOINT_CONTROL_ADDRESS, ENDPOINT_TYPE_CONTROL, ENDPOINT_CONTROL_SIZE, 1);

		__DeviceState = USB_STATE_RESET;
	}
}