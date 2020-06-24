/*
 * KeyboardDescriptor.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB descriptor for a generic USB keyboard.

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

/** @file KeyboardDescriptor.h
 *  @brief USB descriptor for a generic USB keyboard.
 *		   Please read https://usb.org/sites/default/files/documents/hid1_11.pdf if you need some more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef KEYBOARDDESCRIPTOR_H_
#define KEYBOARDDESCRIPTOR_H_

 #include "Services/USB/USB.h"

 /** @brief	Address of the Keyboard HID IN endpoint.
  */
 #define KEYBOARD_IN_EP								(ENDPOINT_DIRECTION_IN  | 0x01)

 /** @brief	Address of the Keyboard HID OUT endpoint.
  */
 #define KEYBOARD_OUT_EP							(ENDPOINT_DIRECTION_OUT | 0x02)

 /** @brief	Size of the IN and OUT endpoints in byte.
  */
 #define KEYBOARD_EP_SIZE							8

 /** @brief Size of the keyboard control endpoint.
  */
 #define KEYBOARD_CTRL_EP_SIZE						ENDPOINT_CONTROL_DEFAULT_SIZE

 /** @brief USB string descriptors used by the keyboard descriptor.
  */
 typedef enum
 {
	 KEYBOARD_STRING_ID_LANGUAGE = 0x00,								/**< Supported Languages string descriptor ID (must be zero) */
	 KEYBOARD_STRING_ID_MANUFACTURER = 0x01,							/**< Manufacturer string ID */
	 KEYBOARD_STRING_ID_PRODUCT = 0x02,									/**< Product string ID */
	 KEYBOARD_STRING_ID_SERIAL = 0x03,									/**< Serial string ID */
 } StringDescriptors_t;

 /** @brief	Device configuration descriptor structure.
  */
 typedef struct
 {
	 USB_ConfigurationDescriptor_t Configuration;						/**< Configuration descriptor */
	 USB_InterfaceDescriptor_t Interface;								/**< Interface descriptor */
	 USB_HID_Descriptor_t KeyboardHID;									/**< HID descriptor */
	 USB_EndpointDescriptor_t DataINEndpoint;							/**< Endpoint descriptor */
	 USB_EndpointDescriptor_t DataOUTEndpoint;							/**< Endpoint descriptor */
 } USB_Configuration_t;

#endif /* KEYBOARDDESCRIPTOR_H_ */