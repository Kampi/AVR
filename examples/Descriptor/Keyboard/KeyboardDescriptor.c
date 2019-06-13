/*
 * KeyboardDescriptor.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB descriptor for a generic USB mouse.

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

/** @file MouseDescriptor.c
 *  @brief USB descriptor for a generic USB keyboard.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "KeyboardDescriptor.h"

const USB_StringDescriptor_t PROGMEM LanguageString = BYTES_TO_STRING_DESCRIPTOR(0x0409);
const USB_StringDescriptor_t PROGMEM ManufacturerString = CHAR_TO_STRING_DESCRIPTOR(L"Daniel Kampert");
const USB_StringDescriptor_t PROGMEM ProductString = CHAR_TO_STRING_DESCRIPTOR(L"AT90USBKey");

const uint8_t PROGMEM KeyboardReport[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x01,                    //   OUTPUT (Cnst,Ary,Abs)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

const USB_DeviceDescriptor_t PROGMEM DeviceDescriptor =
{
	.bLength                = sizeof(USB_DeviceDescriptor_t),
	.bDescriptorType		= DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB			        = USB_VERSION(1, 1, 0),
	.bDeviceClass			= USB_CLASS_USE_INTERFACE,
	.bDeviceSubClass		= USB_SUBCLASS_NONE,
	.bDeviceProtocol		= USB_PROTOCOL_NONE,
	.bMaxPacketSize0		= 8,

	.idVendor				= 0x03EB,
	.idProduct				= 0x2042,
	.bcdDevice				= USB_VERSION(1, 0, 0),
	
	.iManufacturer			= 1,
	.iProduct				= 2,
	.iSerialNumber          = 0,

	.bNumConfigurations		= 1
};

const USB_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Configuration =
	{
		.bLength = sizeof(USB_ConfigurationDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_CONFIGURATION,
		.wTotalLength = sizeof(USB_Configuration_t),
		.bNumInterfaces = 0x01,
		.bConfigurationValue = 0x01,
		.iConfiguration = 0x00,
		.bmAttributes = USB_MASK2CONFIG(USB_CONFIG_SELF_POWERED),
		.bMaxPower = USB_CURRENT_CONSUMPTION(100),
	},
	.Interface =
	{
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x02,
		.bInterfaceClass = USB_CLASS_HID,
		.bInterfaceSubClass = USB_SUBCLASS_BOOT_INTERFACE,
		.bInterfaceProtocol = USB_PROTOCOL_KEYBOARD,
		.iInterface = 0x00,
	},
	.KeyboardHID =
	{
		.bLength = sizeof(USB_HID_Descriptor_t),
		.bDescriptorType = HID_DESCRIPTOR_TYPE_HID,
		.bcdHID = USB_VERSION(1, 1, 0),
		.bCountryCode = HID_COUNTRYCODE_GERMAN,
		.bNumDescriptors = 0x01,
		.bReportType = HID_DESCRIPTOR_TYPE_REPORT,
		.wDescriptorLength = sizeof(KeyboardReport),
	},
	.DataINEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = KEYBOARD_EP_IN_ADDR,
		.bmAttributes = USB_MASK2ENDPOINT(USB_ENDPOINT_USAGE_DATA, USB_ENDPOINT_SYNC_NO, USB_ENDPOINT_TRANSFER_INTERRUPT),
		.wMaxPacketSize = MOUSE_EPSIZE,
		.bInterval = 0x05,
	},
	.DataOUTEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = KEYBOARD_EP_OUT_ADDR,
		.bmAttributes = USB_MASK2ENDPOINT(USB_ENDPOINT_USAGE_DATA, USB_ENDPOINT_SYNC_NO, USB_ENDPOINT_TRANSFER_INTERRUPT),
		.wMaxPacketSize = MOUSE_EPSIZE,
		.bInterval = 0x05,
	}
};

uint16_t USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** Address)
{
	// Get the descriptor type and number
	uint8_t DescriptorType = (wValue >> 0x08);
	uint8_t DescriptorNumber = (wValue & 0xFF);

	uint16_t Size = 0x00;

	// Get the descriptor from program memory
	switch(DescriptorType)
	{
		case DESCRIPTOR_TYPE_DEVICE:
		{
			*Address = &DeviceDescriptor;
			Size = sizeof(USB_DeviceDescriptor_t);

			break;
		}
		case DESCRIPTOR_TYPE_CONFIGURATION:
		{
			*Address = &ConfigurationDescriptor;
			Size = sizeof(USB_Configuration_t);

			break;
		}
		case DESCRIPTOR_TYPE_STRING:
		{
			switch(DescriptorNumber)
			{
				case STRING_ID_Language:
				{
					*Address = &LanguageString;
					Size = pgm_read_byte(&LanguageString.bLength);

					break;
				}
				case STRING_ID_Manufacturer:
				{
					*Address = &ManufacturerString;
					Size = pgm_read_byte(&ManufacturerString.bLength);

					break;
				}
				case STRING_ID_Product:
				{
					*Address = &ProductString;
					Size = pgm_read_byte(&ProductString.bLength);

					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case HID_DESCRIPTOR_TYPE_HID:
		{
			*Address = &ConfigurationDescriptor.KeyboardHID;
			Size = sizeof(USB_HID_Descriptor_t);

			break;
		}
		case HID_DESCRIPTOR_TYPE_REPORT:
		{
			*Address = &KeyboardReport;
			Size = sizeof(KeyboardReport);

			break;
		}
		default:
		{
			break;
		}
	}

	return Size;
}