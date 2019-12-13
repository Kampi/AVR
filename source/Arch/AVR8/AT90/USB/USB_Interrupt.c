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

/** @file Arch/AVR8/AT90/USB/USB_Interrupt.c
 *  @brief Interrupt functions for AT90USB1287 USB.
 * 
 *  This file contains the implementation of the AT90USB1287 USB driver interrupts.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/AT90/USB/USB_Controller.h"

volatile USB_State_t __DeviceState;

extern uint8_t __Configuration;
extern USB_DeviceCallbacks_t __USBEvents;

ISR(USB_GEN_vect)
{
	if(USB_Controller_CheckForInterrupt(USB_SOF_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_SOF_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_SOF_INTERRUPT);

		if(__USBEvents.StartOfFrame != NULL)
		{
			__USBEvents.StartOfFrame();
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_VBUS_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_VBUS_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_VBUS_INTERRUPT);

		if(USB_Controller_CheckVBUS())
		{
			USB_Controller_EnablePLL();

			__DeviceState = USB_STATE_POWERED;

			if(__USBEvents.ConnectWithBus != NULL)
			{
				__USBEvents.ConnectWithBus();
			}
		}
		else
		{
			USB_Controller_DisablePLL();

			__DeviceState = USB_STATE_UNATTACHED;
			
			if(__USBEvents.DisconnectFromBus != NULL)
			{
				__USBEvents.DisconnectFromBus();
			}
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_SUSPEND_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_SUSPEND_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_WAKE_INTERRUPT);

		USB_Controller_DisableClk();

		__DeviceState = USB_STATE_SUSPEND;
		
		if(__USBEvents.Suspend != NULL)
		{
			__USBEvents.Suspend();
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_WAKE_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_WAKE_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_WAKE_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_WAKE_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_SUSPEND_INTERRUPT);
		
		USB_Controller_EnableClk();

		// Check if the device is configured
		if(!__Configuration)
		{
			__DeviceState = USB_STATE_CONFIGURED;
		}
		else
		{
			// Check if the device got an address from the host
			if(USB_Device_IsAddressEnabled())
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

	if(USB_Controller_CheckForInterrupt(USB_EOR_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_EOR_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_EOR_INTERRUPT);
		USB_Controller_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_WAKE_INTERRUPT);

		if(Endpoint_Configure(ENDPOINT_CONTROL_ADDRESS, ENDPOINT_TYPE_CONTROL, ENDPOINT_CONTROL_SIZE, FALSE))
		{
			if(__USBEvents.EndOfReset != NULL)
			{
				__USBEvents.EndOfReset();
			}
		}
		else
		{
			if(__USBEvents.Error != NULL)
			{
				__USBEvents.Error();
			}
		}

		__DeviceState = USB_STATE_RESET;
	}
}