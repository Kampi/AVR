/*
 * DescriptorTypes.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Standard USB Descriptor definitions.

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

/** @file Common/Services/USB/Core/StandardDescriptor.h
 *  @brief Standard USB Descriptor definitions.
 *		   See http://www.usbmadesimple.co.uk/ums_4.htm for additional information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DESCRIPTORTYPES_H_
#define DESCRIPTORTYPES_H_

 #include "Common/Common.h"

 /** @brief	Macro to convert multiple bytes to a Unicode string descriptor.
 */
 #define BYTES_TO_STRING_DESCRIPTOR(...)				{ .bLength = sizeof(USB_StringDescriptor_t) + sizeof((uint16_t){__VA_ARGS__}), .bDescriptorType = DESCRIPTOR_TYPE_STRING, .bString = {__VA_ARGS__} }

 /** @brief	Macro to convert a char array to a Unicode string descriptor.
 */
 #define CHAR_TO_STRING_DESCRIPTOR(Array)				{ .bLength = sizeof(USB_StringDescriptor_t) + (sizeof(Array) - 2), .bDescriptorType = DESCRIPTOR_TYPE_STRING, .bString = Array }

 /** @brief	Macro to convert a USB BCD version number.
 */
 #define USB_VERSION(Major, Minor, Revision)			((Major & 0xFF) << 0x08) | ((Minor & 0x0F) << 0x04) | (Revision & 0x0F)

 /** @brief	Macro to create the value for the maximum power consumption of a device in mA.
 */
 #define USB_CURRENT_CONSUMPTION(Current)				((Current) >> 0x01)

 /** @brief	Macro to create the USB configuration descriptor bmAttributes bitmap from a given #USB_ConfigAttributes_t mask.
 */
 #define USB_MASK2CONFIG(Mask)							(0x80 | Mask)

 /** @brief Macro to create the USB endpoint descriptor bmAttributes bitmap from a given #USB_EndpointAttributes_t mask.
  */
 #define USB_MASK2ENDPOINT(Usage, Synch, Transfer)		(Usage << 0x04 | Synch << 0x02 | Transfer)

 /** @ingroup USB
  *  USB descriptor types */
 /*\@{*/
 typedef enum
 {
	 DESCRIPTOR_TYPE_DEVICE = 0x01,										/**< Device descriptor ID */ 
	 DESCRIPTOR_TYPE_CONFIGURATION = 0x02,								/**< Configuration descriptor ID */ 
	 DESCRIPTOR_TYPE_STRING = 0x03,										/**< String descriptor ID */ 
	 DESCRIPTOR_TYPE_INTERFACE = 0x04,									/**< Interface descriptor ID */ 
	 DESCRIPTOR_TYPE_ENDPOINT = 0x05,									/**< Endpoint descriptor ID */ 
 } DescriptorTypes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB string descriptor types */
 /*\@{*/
 typedef enum
 {
	 STRING_ID_LANGUAGE = 0x00,											/**< Supported Languages string descriptor ID (must be zero) */
	 STRING_ID_MANUFACTURER = 0x01,										/**< Manufacturer string ID */
	 STRING_ID_PRODUCT = 0x02,											/**< Product string ID */
 } StringDescriptors_t;
 /*\@}*/

 /** @ingroup USB
  *  USB base classes
  *  NOTE: Visit https://www.usb.org/defined-class-codes for more information. */
 /*\@{*/
 typedef enum
 {
	 USB_CLASS_USE_INTERFACE = 0x00,									/**< Use the USB Interface Descriptor for the class information 
																			 NOTE: Use this in the Device Descriptor */
	 USB_CLASS_AUDIO = 0x01,											/**< Audio device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_CDC = 0x02,												/**< Communications and CDC Control class
																			 NOTE: Use this in the Device and Interface Descriptor */
	 USB_CLASS_HID = 0x03,												/**< HID device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_PHYSICAL = 0x05,											/**< Physical device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_IMAGE = 0x06,											/**< Image device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_PRINTER = 0x07,											/**< Printer device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_MASS_STORAGE = 0x08,										/**< Mass storage device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_HUB = 0x08,												/**< Hub device class
																			 NOTE: Use this in the Device Descriptor */
	 USB_CLASS_CDC_DATA = 0x0A,											/**< CDC data device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_SMARTCARD = 0x0B,										/**< Smart card device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_SECURITY = 0x0D,											/**< Content Security device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_VIDEO = 0x0E,											/**< Video device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_HEALTHCARE = 0x0F,										/**< Healthcare device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_AUDIO_VIDEO = 0x10,										/**< Audio/Video device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_BILLBOARD = 0x11,										/**< Billboard device class
																			 NOTE: Use this in the Device Descriptor */
 	 USB_CLASS_USB_C_BRIDGE = 0x12,										/**< USB Type-C Bridge class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_DIAGNOSTIC = 0xDC,										/**< Diagnostic class
																			 NOTE: Use this in the Device and Interface Descriptor */
 	 USB_CLASS_WIRELESS = 0xE0,											/**< Wireless controller device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_MISC = 0xEF,												/**< Miscellaneous device class
																			 NOTE: Use this in the Device and Interface Descriptor */
 	 USB_CLASS_APPLICATION = 0xFE,										/**< Application specific device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_VENDOR = 0xFF,											/**< Vendor specific device class
																			 NOTE: Use this in the Device and Interface Descriptor */
 } USB_BaseClass_t;
 /*\@}*/

 /** @ingroup USB
  *  USB subclasses */
 /*\@{*/
 typedef enum
 {
	 USB_SUBCLASS_NONE = 0x00,											/**< No subclass */
	 USB_SUBCLASS_BOOT_INTERFACE = 0x01,								/**< Boot interface subclass */
 } USB_Subclass_t;
 /*\@}*/

 /** @ingroup USB
  *  USB protocols
  *  NOTE: Only needed if you choose #USB_SUBCLASS_BOOT_INTERFACE in the Interface Descriptor. Otherwise set it to #USB_PROTOCOL_NONE. */
 /*\@{*/
 typedef enum
 {
	 USB_PROTOCOL_NONE = 0x00,											/**< No protocol */
	 USB_PROTOCOL_KEYBOARD = 0x01,										/**< Keyboard protocol */
	 USB_PROTOCOL_MOUSE = 0x02,											/**< Mouse protocol */
 } USB_Protocol_t;
 /*\@}*/

 /** @ingroup USB
  *  USB configuration #bmAttributes */
 /*\@{*/
 typedef enum
 {
	 USB_CONFIG_SELF_POWERED = 0x40,									/**< Device Self-powered mask */
	 USB_CONFIG_REMOTE_WAKE = 0x20,										/**< Remote Wakeup attribute mask */
 } USB_ConfigAttributes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB endpoint #bmAttributes */
 /*\@{*/
 typedef enum
 {
	 USB_ENDPOINT_USAGE_DATA = 0x00,									/**< Data endpoint */
	 USB_ENDPOINT_USAGE_FEEDBACK = 0x01,								/**< Feedback endpoint */
	 USB_ENDPOINT_USAGE_IMPL_DATA = 0x02,								/**< Implicit feedback Data endpoint */
	 USB_ENDPOINT_SYNC_NO = 0x00,										/**< No Synchronization */
	 USB_ENDPOINT_SYNC_ASYNC = 0x01,									/**< Asynchronous Synchronization */
	 USB_ENDPOINT_SYNC_ADAPTIVE = 0x02,									/**< Adaptive Synchronization */
	 USB_ENDPOINT_SYNC_SYNCH = 0x03,									/**< Synchronous Synchronization */
	 USB_ENDPOINT_TRANSFER_CONTROL = 0x00,								/**< Control transfer */
	 USB_ENDPOINT_TRANSFER_ISOCHR = 0x01,								/**< Isochronous transfer */
	 USB_ENDPOINT_TRANSFER_BULK = 0x02,									/**< Bulk transfer */
	 USB_ENDPOINT_TRANSFER_INTERRUPT = 0x03,							/**< Interrupt transfer */
 } USB_EndpointAttributes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB device descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< DEVICE descriptor type (=1) */
	 uint16_t bcdUSB;													/**< USB Spec release number */
	 uint8_t bDeviceClass;												/**< Class code assigned by USB-IF */
	 uint8_t bDeviceSubClass;											/**< SubClass Code assigned by USB-IF */
	 uint8_t bDeviceProtocol;											/**< Protocol Code assigned by USB-IF */
	 uint8_t bMaxPacketSize0;											/**< Max packet size for endpoint 0. Must be 8, 16, 32 or 64 */
	 uint16_t idVendor;													/**< Vendor ID - must be obtained from USB-IF */
	 uint16_t idProduct;												/**< Product ID - assigned by the manufacturer */
	 uint16_t bcdDevice;												/**< Device release number in binary coded decimal */
	 uint8_t iManufacturer;												/**< Index of string descriptor describing the manufacturer - set to 0 if no string */
	 uint8_t iProduct;													/**< Index of string descriptor describing the product - set to 0 if no string */
	 uint8_t iSerialNumber;												/**< Index of string descriptor describing the device serial number - set to 0 if no string */
	 uint8_t bNumConfigurations; 										/**< Number of possible configurations */
 } __attribute__((packed)) USB_DeviceDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB configuration descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< CONFIGURATION descriptor type (=2) */
	 uint16_t wTotalLength;												/**< Total number of bytes in this descriptor and all the following descriptors */
	 uint8_t bNumInterfaces;											/**< Number of interfaces supported by this configuration */
	 uint8_t bConfigurationValue;										/**< Value used by Set Configuration to select this configuration */
	 uint8_t iConfiguration;											/**< Index of string descriptor describing configuration - set to 0 if no string */
	 uint8_t bmAttributes;												/**< D7: Must be set to 1
																			 D6: Self-powered
																			 D5: Remote Wakeup
																			 D4-D0: Set to 0 */
	 uint8_t bMaxPower;													/**< Maximum current drawn by device in this configuration. In units of 2mA. So 50 means 100 mA */
 } __attribute__((packed)) USB_ConfigurationDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB string descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< STRING descriptor type (=3) */
	 wchar_t bString[];													/**< UNICODE encoded string */
 } __attribute__((packed)) USB_StringDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB interface descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< INTERFACE descriptor type (=4) */
	 uint8_t bInterfaceNumber;											/**< Number identifying this interface. Zero-based value */
	 uint8_t bAlternateSetting;											/**< Value used to select this alternate setting for this interface */
	 uint8_t bNumEndpoints;												/**< Number of endpoints used by this interface. Doesn't include control endpoint 0 */
	 uint8_t bInterfaceClass;											/**< Class code assigned by USB-IF */
	 uint8_t bInterfaceSubClass;										/**< SubClass Code assigned by USB-IF */
	 uint8_t bInterfaceProtocol;										/**< Protocol Code assigned by USB-IF */
	 uint8_t iInterface;												/**< Index of string descriptor describing interface - set to 0 if no string */
 } __attribute__((packed)) USB_InterfaceDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB endpoint descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< ENDPOINT descriptor type (=5) */
	 uint8_t bEndpointAddress;											/**< The address of this endpoint within the device.
																			 D7: Direction
																			 0 = OUT, 1 = IN
																			 D6-D4: Set to 0
																			 D3-D0: Endpoint number */
	 uint8_t bmAttributes;												/**< D1:0 Transfer Type
																			 00 = Control
																			 01 = Isochronous
																			 10 = Bulk
																			 11 = Interrupt
																			 The following only apply to isochronous endpoints. Else set to 0.
																			 D3:2 Synchronization Type
																			 00 = No Synchronization
																			 01 = Asynchronous
																			 10 = Adaptive
																			 11 = Synchronous
																			 D5:4 Usage Type
																			 00 = Data endpoint
																			 01 = Feedback endpoint
																			 10 = Implicit feedback Data endpoint
																			 11 = Reserved
																			 D7:6 Reserved
																			 Set to 0 */
	 uint16_t wMaxPacketSize;											/**< Maximum packet size this endpoint can send or receive 
																		     when this configuration is selected  */
	 uint8_t bInterval;													/**< Interval for polling endpoint for data transfers. 
																			 Expressed in frames (ms) for low/full speed or micro frames (125us) for high speed */
 } __attribute__((packed)) USB_EndpointDescriptor_t;
 /*\@}*/

 /** @brief		Function prototype to load an descriptor from the program memory.
  *  @wValue	wValue from USB packet
  *  @wIndex	wIndex from USB packet
  *  @Address	Pointer to descriptor address in memory
  *  @return	Descriptor size
  */
 uint16_t USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** Address);

#endif /* DESCRIPTORTYPES_H_  */ 