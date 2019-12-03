/*
 * USB.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB driver for AT90USB1287.

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

/** @file USB/USB.h
 *  @brief USB driver for AT90USB1287.
 *
 *  This file contains the prototypes and definitions for the USB driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_H_
#define USB_H_

 #include "USB/Core/StandardDescriptor.h"
 #include "USB/Core/AVR8/USB_Controller.h"
 #include "USB/Core/USB_DeviceStdRequest.h"

 /** @brief			Initialize the USB service.
  *  @param Events	Pointer to USB event structure
  */
 void USB_Init(const USB_DeviceCallbacks_t* Events);

 /** @brief	Polling function. 
  */
 void USB_Poll(void);

#endif /* USB_H_ */