/*
 * TestDescriptor.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Simple test descriptor for the AVR USB tutorial.

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
 *  @brief Simple test descriptor for the AVR USB tutorial.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "TestDescriptor.h"

const USB_StringDescriptor_t PROGMEM LANGID = LANG_TO_STRING_DESCRIPTOR(CONV_LANG(LANG_ENGLISH, SUBLANG_ARABIC_SAUDI_ARABIA));
const USB_StringDescriptor_t PROGMEM ManufacturerString = WCHAR_TO_STRING_DESCRIPTOR(L"Daniel Kampert");
const USB_StringDescriptor_t PROGMEM ProductString = WCHAR_TO_STRING_DESCRIPTOR(L"AT90USB1287 USB-Example");
const USB_StringDescriptor_t PROGMEM SerialString = WCHAR_TO_STRING_DESCRIPTOR(L"123456");

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
	
	.iManufacturer			= STRING_ID_MANUFACTURER,
	.iProduct				= STRING_ID_PRODUCT,
	.iSerialNumber          = STRING_ID_SERIAL,

	.bNumConfigurations		= 0x01
};

const USB_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Configuration =
	{
		.bLength = sizeof(USB_ConfigurationDescriptor_t),													// Length of the descriptor
		.bDescriptorType = DESCRIPTOR_TYPE_CONFIGURATION,													// Descriptor type: Configuration descriptor
		.wTotalLength = sizeof(USB_Configuration_t),														// Total length of the descriptor
		.bNumInterfaces = 0x01,																				// Number of interfaces in this descriptor
		.bConfigurationValue = 0x01,																		// The configuration value must be greater than zero, because you 
																											// set the configuration with the SET_CONFIGURATION request and a 
																											// configuration value of zero will	place the device in its address state.
		.iConfiguration = 0x00,																				// No string descriptor for the configuration
		.bmAttributes = USB_MASK2CONFIG(USB_CONFIG_SELF_POWERED),
		.bMaxPower = USB_CURRENT_CONSUMPTION(100),
	},
	.Interface =
	{
		.bLength = sizeof(USB_InterfaceDescriptor_t),														// Length of the descriptor
		.bDescriptorType = DESCRIPTOR_TYPE_INTERFACE,														// Descriptor type: Interface descriptor
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x02,																				// Number of endpoints in this descriptor
		.bInterfaceClass = USB_CLASS_VENDOR,
		.bInterfaceSubClass = USB_SUBCLASS_NONE,
		.bInterfaceProtocol = USB_PROTOCOL_NONE,
		.iInterface = 0x00,																					// No string descriptor for the interface
	},
	.DataINEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),														// Length of the descriptor
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,														// Descriptor type: Endpoint descriptor
		.bEndpointAddress = IN_EP,
		.bmAttributes = USB_ENDPOINT_USAGE_DATA | USB_ENDPOINT_SYNC_NO | USB_ENDPOINT_TRANSFER_INTERRUPT,
		.wMaxPacketSize = EP_SIZE,
		.bInterval = 0x0A,
	},
	.DataOUTEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),														// Length of the descriptor
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,														// Descriptor type: Endpoint descriptor
		.bEndpointAddress = OUT_EP,
		.bmAttributes = USB_ENDPOINT_USAGE_DATA | USB_ENDPOINT_SYNC_NO | USB_ENDPOINT_TRANSFER_INTERRUPT,
		.wMaxPacketSize = EP_SIZE,
		.bInterval = 0x0A,
	},
};

const void* USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, uint16_t* Size)
{
	// Get the descriptor type and number
	uint8_t DescriptorType = (wValue >> 0x08);
	uint8_t DescriptorNumber = (wValue & 0xFF);

	// Get the descriptor address
	switch(DescriptorType)
	{
		case DESCRIPTOR_TYPE_DEVICE:
		{
			*Size = sizeof(USB_DeviceDescriptor_t);
			return &DeviceDescriptor;
		}
		case DESCRIPTOR_TYPE_CONFIGURATION:
		{
			*Size = sizeof(USB_Configuration_t);
			return &ConfigurationDescriptor;
		}
		case DESCRIPTOR_TYPE_STRING:
		{
			switch(DescriptorNumber)
			{
				case STRING_ID_LANGUAGE:
				{
					*Size = pgm_read_byte(&LANGID.bLength);
					return &LANGID;
				}
				case STRING_ID_MANUFACTURER:
				{
					*Size = pgm_read_byte(&ManufacturerString.bLength);
					return &ManufacturerString;
				}
				case STRING_ID_PRODUCT:
				{
					*Size = pgm_read_byte(&ProductString.bLength);
					return &ProductString;
				}
				case STRING_ID_SERIAL:
				{
					*Size = pgm_read_byte(&SerialString.bLength);
					return &SerialString;
				}
			}
		}
	}

	*Size = 0x00;
	return NULL;
}