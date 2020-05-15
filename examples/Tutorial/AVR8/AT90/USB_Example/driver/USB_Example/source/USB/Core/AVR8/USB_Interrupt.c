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
#include "USB/Core/AVR8/USB_Device.h"

volatile USB_State_t _DeviceState;
USB_DeviceCallbacks_t _USBEvents;

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
	}
}

uint8_t USB_Controller_CheckForInterrupt(const USB_InterruptType_t Interrupt)
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
	}
 
	return 0x00;
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
	if(USB_Controller_CheckForInterrupt(USB_VBUS_INTERRUPT))
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

	if(USB_Controller_CheckForInterrupt(USB_EOR_INTERRUPT))
	{
		USB_Controller_ClearInterruptFlag(USB_EOR_INTERRUPT);

		_DeviceState = USB_STATE_RESET;

		// Configure the default control endpoint
		if(Endpoint_Configure(ENDPOINT_CONTROL_ADDRESS, ENDPOINT_TYPE_CONTROL, ENDPOINT_CONTROL_SIZE, 0))
		{
			if(_USBEvents.EndOfReset != NULL)
			{
				_USBEvents.EndOfReset();
			}
		}
		else
		{
			if(_USBEvents.Error != NULL)
			{
				_USBEvents.Error();
			}
		}
	}
}