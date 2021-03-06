/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
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
#include "Joystick/Joystick.h"

#include "MouseDescriptor.h"

#include <string.h>

#define LED0_RED						PORTD, 4
#define LED0_GREEN						PORTD, 5

uint8_t Idle = 0x00;
uint8_t Protocol = 0x00;

void Mouse_Task(void);

void USB_Event_OnError(void);
void USB_Event_EndOfReset(void);
void USB_Event_ConfigurationChanged(const uint8_t Configuration);
void USB_Event_ControlRequest(const uint8_t bRequest, const uint8_t bmRequestType, const uint16_t wValue);

const USB_DeviceCallbacks_t Events_USB =
{
	.EndOfReset = USB_Event_EndOfReset,
	.Error = USB_Event_OnError,
	.ConfigurationChanged = USB_Event_ConfigurationChanged,
	.ControlRequest = USB_Event_ControlRequest,
};

USB_Config_t ConfigUSB = {
	.Callbacks = &Events_USB,
	.Mode = USB_MODE_DEVICE,
	.Speed = USB_SPEED_LOW,
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
		Initialize the joystick
	*/
	Joystick_Init();

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
	uint16_t BytesSend = 0x00;
	USB_MouseReport_t MouseReportData;

	if(USB_GetState() != USB_STATE_CONFIGURED)
	{
		return;
	}

	memset(&MouseReportData, 0x00, sizeof(MouseReportData));

	switch(Joystick_Read())
	{
		case JOYSTICK_NO_ACTION:
		{
			break;
		}
		case JOYSTICK_DOWN:
		{
			MouseReportData.Y = -1;
			break;
		}
		case JOYSTICK_UP:
		{
			MouseReportData.Y = 1;
			break;
		}
		case JOYSTICK_RIGHT:
		{
			MouseReportData.X = -1;
			break;
		}
		case JOYSTICK_LEFT:
		{
			MouseReportData.X = 1;
			break;
		}
		case JOYSTICK_PRESS:
		{
			MouseReportData.Button |= (0x01 << 0x00);
			break;
		}
	}

	Endpoint_Select(MOUSE_IN_EP);
	if(Endpoint_IsReadWriteAllowed())
	{
		USB_DeviceStream_DataIN(&MouseReportData, sizeof(USB_MouseReport_t), &BytesSend);
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

void USB_Event_ControlRequest(const uint8_t bRequest, const uint8_t bmRequestType, const uint16_t wValue)
{
	switch(bRequest)
	{
		case HID_REQUEST_GET_REPORT:
		{
			if(bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				// Allow the host to request a report through the control pipe
			}

			break;
		}
		case HID_REQUEST_SET_REPORT:
		{
			if(bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				// Allow the host to send a report through the control pipe
			}

			break;
		}
		case HID_REQUEST_GET_IDLE:
		{
			if(bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				Endpoint_WriteByte(Idle >> 0x02);
				Endpoint_FlushIN();

				Endpoint_HandleSTATUS(bmRequestType);
			}

			break;
		}
		case HID_REQUEST_SET_IDLE:
		{
			if(bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				Endpoint_HandleSTATUS(bmRequestType);

				Idle = ((wValue & 0xFF00) >> 0x06);
			}

			break;
		}
		case HID_REQUEST_GET_PROTOCOL:
		{
			if(bmRequestType == (REQUEST_DIRECTION_DEVICE_TO_HOST | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				Endpoint_WriteByte(Protocol);
				Endpoint_FlushIN();

				Endpoint_HandleSTATUS(bmRequestType);
			}

			break;
		}
		case HID_REQUEST_SET_PROTOCOL:
		{
			if(bmRequestType == (REQUEST_DIRECTION_HOST_TO_DEVICE | REQUEST_TYPE_CLASS | REQUEST_RECIPIENT_INTERFACE))
			{
				Endpoint_HandleSTATUS(bmRequestType);

				Protocol = wValue;
			}

			break;
		}
	}
}