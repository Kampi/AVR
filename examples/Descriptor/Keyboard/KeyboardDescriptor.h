/*
 * KeyboardDescriptor.h
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

/** @file KeyboardDescriptor.h
 *  @brief USB descriptor for a generic USB keyboard.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef KEYBOARDDESCRIPTOR_H_
#define KEYBOARDDESCRIPTOR_H_

 #include "Common/Services/USB/USB.h"

 /** 
  * Size of the Keyboard HID endpoint
  */
 #define MOUSE_EPSIZE				8

 /** 
  * Address of the Keyboard HID IN endpoint
  */
 #define KEYBOARD_EP_IN_ADDR       (ENDPOINT_DIRECTION_IN  | 0x01)
 
 /** 
  * Address of the Mouse Keyboard OUT endpoint
  */
 #define KEYBOARD_EP_OUT_ADDR      (ENDPOINT_DIRECTION_OUT | 0x02)

 /** 
  * Device configuration descriptor structure
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