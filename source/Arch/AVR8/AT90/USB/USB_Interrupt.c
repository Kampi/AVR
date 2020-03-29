/*
 * USB_Interrupt.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt functions for Atmel AVR AT90 USB interface.

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

/** @file Arch/AVR8/AT90/USB/USB_Interrupt.c
 *  @brief Interrupt functions for Atmel AVR AT90 USB interface.
 * 
 *  This file contains the implementation of the AT90USB1287 USB driver interrupts.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/AT90/USB/USB_Controller.h"

void USB_Controller_EnableInterrupt(const USB_InterruptType_t Source)
{
	switch(Source)
	{
		/*
			Device & Host interrupts
		*/
		case USB_VBUS_INTERRUPT:
		{
			USBCON |= (0x01 << VBUSTE);
			break;
		}

		/*
			Device interrupts
		*/
		case USB_WAKE_INTERRUPT:
		{
			UDIEN |= (0x01 << WAKEUPE);
			break;
		}
		case USB_SUSPEND_INTERRUPT:
		{
			UDIEN |= (0x01 << SUSPE);
			break;
		}
		case USB_EOR_INTERRUPT:
		{
			UDIEN |= (0x01 << EORSTE);
			break;
		}
		case USB_SOF_INTERRUPT:
		{
			UDIEN |= (0x01 << SOFE);
			break;
		}
		case USB_RXSTP_INTERRUPT:
		{
			UEIENX |= (0x01 << RXSTPE);
			break;
		}
		default:
		{
			break;
		}

		/*
			Host interrupts
		*/
	}
}

bool USB_Controller_IsInterruptEnabled(const USB_InterruptType_t Interrupt)
{
	switch(Interrupt)
	{
		/*
			Device & Host interrupts
		*/
		case USB_VBUS_INTERRUPT:
		{
			return (USBCON & (0x01 << VBUSTE));
		}

		/*
			Device interrupts
		*/
		case USB_WAKE_INTERRUPT:
		{
			return ((UDIEN & (0x01 << WAKEUPE)) >> WAKEUPE);
		}
		case USB_SUSPEND_INTERRUPT:
		{
			return (UDIEN & (0x01 << SUSPE));
		}
		case USB_EOR_INTERRUPT:
		{
			return ((UDIEN & (0x01 << EORSTE)) >> EORSTE);
		}
		case USB_SOF_INTERRUPT:
		{
			return ((UDIEN & (0x01 << SOFE)) >> SOFE);
		}
		case USB_RXSTP_INTERRUPT:
		{
			return ((UEIENX & (0x01 << RXSTPE)) >> RXSTPE);
		}

		/*
			Host interrupts
		*/
	}
	 
	return false;
}

bool USB_Controller_CheckForInterrupt(const USB_InterruptType_t Interrupt)
{
	switch(Interrupt)
	{
		/*
			Device & Host interrupts
		*/
		case USB_VBUS_INTERRUPT:
		{
			return (USBINT & (0x01 << VBUSTI));
		}

		/*
			Device interrupts
		*/
		case USB_WAKE_INTERRUPT:
		{
			return ((UDINT & (0x01 << WAKEUPI)) >> WAKEUPI);
		}
		case USB_SUSPEND_INTERRUPT:
		{
			return (UDINT & (0x01 << SUSPI));
		}
		case USB_EOR_INTERRUPT:
		{
			return ((UDINT & (0x01 << EORSTI)) >> EORSTI);
		}
		case USB_SOF_INTERRUPT:
		{
			return ((UDINT & (0x01 << SOFI)) >> SOFI);
		}
		case USB_RXSTP_INTERRUPT:
		{
			return ((UEINTX & (0x01 << RXSTPI)) >> RXSTPI);
		}

		/*
			Host interrupts
		*/
	}
 
	return false;
}

void USB_Controller_ClearInterruptFlag(const USB_InterruptType_t Interrupt)
{
	switch(Interrupt)
	{
		/*
			Device & Host interrupts
		*/
		case USB_VBUS_INTERRUPT:
		{
			USBINT &= ~(0x01 << VBUSTI);
			break;
		}

		/*
			Device interrupts
		*/
		case USB_WAKE_INTERRUPT:
		{
			UDINT &= ~(0x01 << WAKEUPI);
			break;
		}
		case USB_SUSPEND_INTERRUPT:
		{
			UDINT &= ~(0x01 << SUSPI);
			break;
		}
		case USB_EOR_INTERRUPT:
		{
			UDINT &= ~(0x01 << EORSTI);
			break;
		}
		case USB_SOF_INTERRUPT:
		{
			UDINT &= ~(0x01 << SOFI);
			break;
		}
		case USB_RXSTP_INTERRUPT:
		{
			UEINTX &= ~(0x01 << RXSTPI);
			break;
		}

		/*
			Host interrupts
		*/
	}
}

void USB_Controller_DisableInterrupt(const USB_InterruptType_t Source)
{
	switch(Source)
	{
		/*
			Device & Host interrupts
		*/
		case USB_VBUS_INTERRUPT:
		{
			USBCON &= ~(0x01 << VBUSTE);
			break;
		}

		/*
			Device interrupts
		*/
		case USB_WAKE_INTERRUPT:
		{
			UDIEN &= ~(0x01 << WAKEUPE);
			break;
		}
		case USB_SUSPEND_INTERRUPT:
		{
			UDIEN &= ~(0x01 << SUSPE);
			break;
		}
		case USB_EOR_INTERRUPT:
		{
			UDIEN &= ~(0x01 << EORSTE);
			break;
		}
		case USB_SOF_INTERRUPT:
		{
			UDIEN &= ~(0x01 << SOFE);
			break;
		}
		case USB_RXSTP_INTERRUPT:
		{
			UEIENX &= ~(0x01 << RXSTPE);
			break;
		}

		/*
			Host interrupts
		*/
	} 
}

void USB_Controller_DisableAllInterrupts(void)
{
	USBCON &= ~((0x01 << VBUSTE) | (0x01 << IDTE));

	// Clear all device mode interrupt enable bits
	UDIEN = 0x00;

	// Clear all host mode interrupt enable bits
	UHIEN = 0x00;

	// Clear all OTG mode interrupt enable bits
	OTGIEN = 0x00;
}

void USB_Controller_ClearInterrupts(void)
{
	// Clear all interrupt flags of the USB controller
	USBINT = 0x00;

	// Clear all device mode interrupt flags
	UDINT = 0x00;

	// Clear all host mode interrupt flags
	UHINT = 0x00;

	// Clear all OTG mode interrupt flags
	OTGINT = 0x00;
}

ISR(USB_GEN_vect)
{
	if(USB_Controller_CheckForInterrupt(USB_SOF_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_SOF_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_SOF_INTERRUPT);

		if(_USBEvents.StartOfFrame != NULL)
		{
			_USBEvents.StartOfFrame();
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_VBUS_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_VBUS_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_VBUS_INTERRUPT);

		if(USB_Controller_CheckVBUS())
		{
			USB_Controller_EnablePLL();

			_DeviceState = USB_STATE_POWERED;

			if(_USBEvents.ConnectWithBus != NULL)
			{
				_USBEvents.ConnectWithBus();
			}
		}
		else
		{
			USB_Controller_DisablePLL();

			_DeviceState = USB_STATE_UNATTACHED;

			if(_USBEvents.DisconnectFromBus != NULL)
			{
				_USBEvents.DisconnectFromBus();
			}
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_SUSPEND_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_SUSPEND_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_WAKE_INTERRUPT);

		USB_Controller_DisableClk();

		_DeviceState = USB_STATE_SUSPEND;

		if(_USBEvents.Suspend != NULL)
		{
			_USBEvents.Suspend();
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_WAKE_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_WAKE_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_WAKE_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_WAKE_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_SUSPEND_INTERRUPT);

		USB_Controller_EnableClk();

		// Check if the device is configured
		if(!_Configuration)
		{
			_DeviceState = USB_STATE_CONFIGURED;
		}
		// Check if the device got an address from the host
		else if(USB_Device_IsAddressEnabled())
		{
			_DeviceState = USB_STATE_ADDRESSED;
		}
		// No configuration or address set
		else
		{
			_DeviceState = USB_STATE_POWERED;
		}

		if(_USBEvents.Wake != NULL)
		{
			_USBEvents.Wake();
		}
	}

	if(USB_Controller_CheckForInterrupt(USB_EOR_INTERRUPT) && USB_Controller_IsInterruptEnabled(USB_EOR_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_EOR_INTERRUPT);
		USB_Controller_ClearInterruptFlag(USB_SUSPEND_INTERRUPT);

		USB_Controller_DisableInterrupt(USB_SUSPEND_INTERRUPT);
		USB_Controller_EnableInterrupt(USB_WAKE_INTERRUPT);

		if(!Endpoint_Configure(ENDPOINT_CONTROL_ADDRESS, ENDPOINT_TYPE_CONTROL, ENDPOINT_CONTROL_SIZE, false))
		{
			if(_USBEvents.Error != NULL)
			{
				_USBEvents.Error();
			}
		}

		_DeviceState = USB_STATE_RESET;
	}
}

ISR(USB_COM_vect)
{
}