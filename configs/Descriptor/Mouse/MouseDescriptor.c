/*
 * MouseDescriptor.c
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
 *  @brief USB descriptor for a generic USB mouse.
 *		   Please read https://usb.org/sites/default/files/documents/hid1_11.pdf if you need some more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#include "MouseDescriptor.h"

const USB_StringDescriptor_t PROGMEM LANGID = LANG_TO_STRING_DESCRIPTOR(CONV_LANG(LANG_ENGLISH, SUBLANG_ARABIC_SAUDI_ARABIA));
const USB_StringDescriptor_t PROGMEM ManufacturerString = WCHAR_TO_STRING_DESCRIPTOR(L"Daniel Kampert");
const USB_StringDescriptor_t PROGMEM ProductString = WCHAR_TO_STRING_DESCRIPTOR(L"AT90USBKey Mouse example");
const USB_StringDescriptor_t PROGMEM SerialString = WCHAR_TO_STRING_DESCRIPTOR(L"123456");

const uint8_t Endpoint_ControlSize = MOUSE_CTRL_EP_SIZE;

const uint8_t PROGMEM MouseReport[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0xff,                    //     LOGICAL_MINIMUM (-1)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0xff,                    //     PHYSICAL_MINIMUM (-1)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
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
	.bMaxPacketSize0		= MOUSE_CTRL_EP_SIZE,

	.idVendor				= 0x03EB,
	.idProduct				= 0x2042,
	.bcdDevice				= USB_VERSION(1, 0, 0),
	
	.iManufacturer			= MOUSE_STRING_ID_MANUFACTURER,
	.iProduct				= MOUSE_STRING_ID_PRODUCT,
	.iSerialNumber          = MOUSE_STRING_ID_SERIAL,

	.bNumConfigurations		= 1,
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
		.bInterfaceClass = USB_CLASS_HID,
		.bInterfaceSubClass = HID_SUBCLASS_BOOT_INTERFACE,
		.bInterfaceProtocol = HID_PROTOCOL_MOUSE,
		.iInterface = 0x00,
	},
	[0].MouseHID =
	{
		.bLength = sizeof(USB_HID_Descriptor_t),
		.bDescriptorType = HID_DESCRIPTOR_TYPE_HID,
		.bcdHID = USB_VERSION(1, 1, 0),
		.bCountryCode = HID_COUNTRYCODE_GERMAN,
		.bNumDescriptors = 0x01,
		.bReportType = HID_DESCRIPTOR_TYPE_REPORT,
		.wDescriptorLength = sizeof(MouseReport),
	},
	[0].DataINEndpoint =
	{
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress = MOUSE_IN_EP,
		.bmAttributes = USB_ENDPOINT_USAGE_DATA | USB_ENDPOINT_SYNC_NO | USB_ENDPOINT_TRANSFER_INTERRUPT,
		.wMaxPacketSize = MOUSE_EP_SIZE,
		.bInterval = 0x0A,
	}
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
			return &ConfigurationDescriptor[DescriptorNumber];
		}
		case DESCRIPTOR_TYPE_STRING:
		{
			switch(DescriptorNumber)
			{
				case MOUSE_STRING_ID_LANGUAGE:
				{
					*Size = pgm_read_byte(&LANGID.bLength);
					return &LANGID;
				}
				case MOUSE_STRING_ID_MANUFACTURER:
				{
					*Size = pgm_read_byte(&ManufacturerString.bLength);
					return &ManufacturerString;
				}
				case MOUSE_STRING_ID_PRODUCT:
				{
					*Size = pgm_read_byte(&ProductString.bLength);
					return &ProductString;
				}
				case MOUSE_STRING_ID_SERIAL:
				{
					*Size = pgm_read_byte(&SerialString.bLength);
					return &SerialString;
				}
			}
		}
		case HID_DESCRIPTOR_TYPE_HID:
		{
			*Size = sizeof(USB_HID_Descriptor_t);
			return &ConfigurationDescriptor[DescriptorNumber].MouseHID;
		}
		case HID_DESCRIPTOR_TYPE_REPORT:
		{
			*Size = sizeof(MouseReport);
			return &MouseReport;
		}
	}

	*Size = 0x00;
	return NULL;
}