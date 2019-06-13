/*
 * MassStorageDescriptor.c
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

/** @file MassStorageDescriptor.c
 *  @brief USB descriptor for a generic mass storage device.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "MassStorageDescriptor.h"

const USB_StringDescriptor_t PROGMEM LanguageString = BYTES_TO_STRING_DESCRIPTOR(0x0409);
const USB_StringDescriptor_t PROGMEM ManufacturerString = CHAR_TO_STRING_DESCRIPTOR(L"Daniel Kampert");
const USB_StringDescriptor_t PROGMEM ProductString = CHAR_TO_STRING_DESCRIPTOR(L"AT90USBKey");

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
	.idProduct				= 0x2045,
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
		.bmAttributes = USB_MASK2CONFIG(0),
		.bMaxPower = USB_CURRENT_CONSUMPTION(100),
	},
	.Interface =
	{
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x02,
		.bInterfaceClass = USB_CLASS_MASS_STORAGE,
		.bInterfaceSubClass = 0x06,
		.bInterfaceProtocol = 0x50,
		.iInterface = 0x00,
	},
	.DataINEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = MASS_EP_IN_ADDR,
		.bmAttributes = USB_MASK2ENDPOINT(USB_ENDPOINT_USAGE_DATA, USB_ENDPOINT_SYNC_NO, USB_ENDPOINT_TRANSFER_BULK),
		.wMaxPacketSize = 64,
		.bInterval = 0x05,
	},
	.DataOUTEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = MASS_EP_OUT_ADDR,
		.bmAttributes = USB_MASK2ENDPOINT(USB_ENDPOINT_USAGE_DATA, USB_ENDPOINT_SYNC_NO, USB_ENDPOINT_TRANSFER_BULK),
		.wMaxPacketSize = 64,
		.bInterval = 0x05,
	}
};

uint16_t USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** const Address)
{
	// Get the descriptor type and number
	uint8_t DescriptorType = (wValue >> 0x08);
	uint8_t DescriptorNumber = (wValue & 0xFF);

	const void* Temp = NULL;
	uint16_t Size = 0x00;

	// Get the descriptor from program memory
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
			Temp = &ConfigurationDescriptor;
			Size = sizeof(USB_Configuration_t);

			break;
		}
		case DESCRIPTOR_TYPE_STRING:
		{
			switch(DescriptorNumber)
			{
				case STRING_ID_Language:
				{
					Temp = &LanguageString;
					Size = pgm_read_byte(&LanguageString.bLength);

					break;
				}
				case STRING_ID_Manufacturer:
				{
					Temp = &ManufacturerString;
					Size = pgm_read_byte(&ManufacturerString.bLength);

					break;
				}
				case STRING_ID_Product:
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