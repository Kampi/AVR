/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB example for AT90USB1287.

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

/** @file main.c
 *  @brief USB example for AT90USB1287.
 *
 *  Software for the AT90USB1287 USB tutorial from
 *  https://www.kampis-elektroecke.de/avr/avr8/at90usb1287-usb/
 *
 *  @author Daniel Kampert
 */

#include "USB/USB.h"
#include "TestDescriptor.h"

void USB_Event_OnError(void);
void USB_Event_ConfigurationChanged(const uint8_t Configuration);
void USB_Event_ControlRequest(const uint8_t bRequest, const uint8_t bmRequestType, const uint16_t wValue);

uint16_t Idle = 0x00;
uint8_t Protocol = 0x00;

const USB_DeviceCallbacks_t Events_USB =
{
	.Error = USB_Event_OnError,
	.ConfigurationChanged = USB_Event_ConfigurationChanged,
	.ControlRequest = USB_Event_ControlRequest,
};

int main(void)
{
	/*
		Initialize the GPIO
			-> PD4 - PD7 as outputs
			-> Disable all outputs
	*/
	DDRD |= (0x01 << 0x07) | (0x01 << 0x06) | (0x01 << 0x05) | (0x01 << 0x04);
	PORTD &= ~((0x01 << 0x07) | (0x01 << 0x06) | (0x01 << 0x05) | (0x01 << 0x04));
	
	/*
		Initialize the USB
			-> USB device
			-> Low-Speed
	*/
	USB_Init(&Events_USB);

	/*
		Enable global interrupts
	*/
	sei();

	while(1) 
	{
	    USB_Poll();
	}
}

void USB_Event_OnError(void)
{
}

void USB_Event_ConfigurationChanged(const uint8_t Configuration)
{
}

void USB_Event_ControlRequest(const uint8_t bRequest, const uint8_t bmRequestType, const uint16_t wValue)
{
}