/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB mouse example for the AT90USB1287.

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
 *  @brief USB mouse example for the AT90USB1287.
 *
 *  Software for the AT90USB1287 USB tutorial from
 *  https://www.kampis-elektroecke.de/avr/avr8/at90usb1287-usb/
 *
 *  @author Daniel Kampert
 */

#include "USB/USB.h"
#include "GPIO/GPIO.h"
#include "MouseDescriptor.h"

#define LED0_RED						PORTD, 4
#define LED0_GREEN						PORTD, 5

void Mouse_Task(void);

void USB_Event_OnError(void);
void USB_Event_EndOfReset(void);
void USB_Event_ConfigurationChanged(const uint8_t Configuration);

const USB_DeviceCallbacks_t Events_USB =
{
	.EndOfReset = USB_Event_EndOfReset,
	.Error = USB_Event_OnError,
	.ConfigurationChanged = USB_Event_ConfigurationChanged,
};

USB_Config_t ConfigUSB = {
	.Callbacks = &Events_USB,
	.Mode = USB_MODE_DEVICE,
	.Speed = USB_SPEED_FULL,
};

int main(void)
{
	/*
		Initialize the GPIO
			-> LED0 red as output
			-> LED0 green as output
	*/
	GPIO_SetDirection(GET_PERIPHERAL(LED0_RED), GET_INDEX(LED0_RED), GPIO_DIRECTION_OUT);
	GPIO_SetDirection(GET_PERIPHERAL(LED0_GREEN), GET_INDEX(LED0_GREEN), GPIO_DIRECTION_OUT);

	/*
		Initialize the USB
			-> USB device
			-> Low-Speed
	*/
	USB_Init(&ConfigUSB);

	/*
		Enable global interrupts
	*/
	sei();

	while(1) 
	{
	    USB_Poll();
		Mouse_Task();
	}
}

void Mouse_Task(void)
{
	if(USB_GetState() != USB_STATE_CONFIGURED)
	{
		return;
	}
}

void USB_Event_EndOfReset(void)
{
	// Set D2 green to signal end of the reset
	GPIO_Set(GET_PERIPHERAL(LED0_GREEN), GET_INDEX(LED0_GREEN));
	GPIO_Clear(GET_PERIPHERAL(LED0_RED), GET_INDEX(LED0_RED));
}

void USB_Event_OnError(void)
{
	// Set D2 red when the endpoint configuration was not successfully
	GPIO_Clear(GET_PERIPHERAL(LED0_GREEN), GET_INDEX(LED0_GREEN));
	GPIO_Set(GET_PERIPHERAL(LED0_RED), GET_INDEX(LED0_RED));
}

void USB_Event_ConfigurationChanged(const uint8_t Configuration)
{
	// Configure the endpoints according to the configuration
	if(Endpoint_Configure(MOUSE_IN_EP, ENDPOINT_TYPE_INTERRUPT, MOUSE_EP_SIZE, 1))
	{
		// Set D2 red and green when the endpoint configuration was successfully
		GPIO_Set(GET_PERIPHERAL(LED0_GREEN), GET_INDEX(LED0_GREEN));
		GPIO_Set(GET_PERIPHERAL(LED0_RED), GET_INDEX(LED0_RED));
	}
	else
	{
		USB_Event_OnError();
	}
}