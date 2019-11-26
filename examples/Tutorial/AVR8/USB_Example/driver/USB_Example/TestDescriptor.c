/*
 * TestDescriptor.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Test descriptor.

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

/** @file TestDescriptor.c
 *  @brief Test descriptor.
 *		   Visit https://usb.org/sites/default/files/documents/hid1_11.pdf if you need some more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "TestDescriptor.h"

const USB_StringDescriptor_t PROGMEM LANGID = LANG_TO_STRING_DESCRIPTOR(CONV_LANG(LANG_ENGLISH, SUBLANG_ARABIC_SAUDI_ARABIA));
const USB_StringDescriptor_t PROGMEM ManufacturerString = WCHAR_TO_STRING_DESCRIPTOR(L"Daniel Kampert");
const USB_StringDescriptor_t PROGMEM ProductString = WCHAR_TO_STRING_DESCRIPTOR(L"AT90USB1287 USB-Example");
const USB_StringDescriptor_t PROGMEM SerialString = WCHAR_TO_STRING_DESCRIPTOR(L"123456");

const uint8_t Endpoint_ControlSize = MOUSE_CTRL_EP_SIZE;

const USB_DeviceDescriptor_t PROGMEM DeviceDescriptor =
{
	.bLength                = sizeof(USB_DeviceDescriptor_t), 
	.bDescriptorType		= DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB					= USB_VERSION(1, 1, 0),
	.bDeviceClass			= USB_CLASS_VENDOR,
	.bDeviceSubClass		= USB_SUBCLASS_NONE,
	.bDeviceProtocol		= USB_PROTOCOL_NONE,
	.bMaxPacketSize0		= 8,

	.idVendor				= 0x0123,
	.idProduct				= 0x4567,
	.bcdDevice				= USB_VERSION(1, 0, 0),
	
	.iManufacturer			= 1,
	.iProduct				= 2,
	.iSerialNumber          = 3,

	.bNumConfigurations		= 1
};

const USB_Configuration_t PROGMEM ConfigurationDescriptor[] =
{
	[0].Configuration =
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
	[0].Interface =
	{
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x01,
		.bInterfaceClass = USB_CLASS_VENDOR,
		.bInterfaceSubClass = USB_SUBCLASS_NONE,
		.bInterfaceProtocol = USB_PROTOCOL_NONE,
		.iInterface = 0x00,
	},
	[0].DataINEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = MOUSE_IN_EP,
		.bmAttributes = USB_MASK2ENDPOINT(USB_ENDPOINT_USAGE_DATA, USB_ENDPOINT_SYNC_NO, USB_ENDPOINT_TRANSFER_INTERRUPT),
		.wMaxPacketSize = MOUSE_EP_SIZE,
		.bInterval = 0x0A,
	}
};

uint16_t USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** const Address)
{
	// Get the descriptor type and number
	uint8_t DescriptorType = (wValue >> 0x08);
	uint8_t DescriptorNumber = (wValue & 0xFF);

	const void* Temp = 0x00;
	uint16_t Size = 0x00;

	// Get the descriptor address
	switch(DescriptorType)
	{
		case DESCRIPTOR_TYPE_DEVICE:
		{
			Temp = &DeviceDescriptor;
			Size = sizeof(USB_DeviceDescriptor_t);

			break;
		}
		case DESCRIPTOR_TYPE_CONFIGURATION:
		{
			Temp = &ConfigurationDescriptor[DescriptorNumber];
			Size = sizeof(USB_Configuration_t);

			break;
		}
		case DESCRIPTOR_TYPE_STRING:
		{
			switch(DescriptorNumber)
			{
				case STRING_ID_LANGUAGE:
				{
					Temp = &LANGID.bString[DescriptorNumber];
					Size = 2;

					break;
				}
				case STRING_ID_MANUFACTURER:
				{
					Temp = &ManufacturerString;
					Size = pgm_read_byte(&ManufacturerString.bLength);

					break;
				}
				case STRING_ID_PRODUCT:
				{
					Temp = &ProductString;
					Size = pgm_read_byte(&ProductString.bLength);

					break;
				}
				case STRING_ID_SERIAL:
				{
					Temp = &ProductString;
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
		default:
		{
			break;
		}
	}

	*Address = Temp;

	return Size;
}