/*
 * USB.c
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

/** @file USB/USB.c
 *  @brief USB driver for AT90USB1287.
 *
 *  This file contains the implementation of the USB driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "USB/USB.h"

volatile USB_State_t __DeviceState;

void USB_Init(void)
{
	// Initialize the USB controller
	USBController_Init(USB_MODE_DEVICE, USB_SPEED_LOW);

	__DeviceState = USB_STATE_RESET;
}