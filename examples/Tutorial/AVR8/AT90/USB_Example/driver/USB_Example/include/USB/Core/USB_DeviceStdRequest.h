/*
 * USB_DeviceStdRequest.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB standard device request implementation.

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

/** @file USB/Core/USB_DeviceStdRequest.h
 *  @brief USB device standard request handling implementation.
 *
 *  This file contains the prototypes and definitions for the USB device standard request handling.
 *  NOTE: See https://www.beyondlogic.org/usbnutshell/usb6.shtml for more information
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_DEVICESTDREQUEST_H_
#define USB_DEVICESTDREQUEST_H_
 
 #include "USB/Core/AVR8/USB_Controller.h"

 #include "USB/Core/USB_DeviceStream.h"
 #include "USB/Core/StandardRequest.h"
 #include "USB/Core/StandardDescriptor.h"
 
 /** @brief	Handle the standard USB device requests.
  */
 void USBDevice_ControlRequest(void);

#endif /* USB_DEVICESTDREQUEST_H_  */ 