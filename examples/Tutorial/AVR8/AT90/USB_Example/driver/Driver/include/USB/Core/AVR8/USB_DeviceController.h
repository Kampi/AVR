/*
 * USB_DeviceController.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB device controller definition for AT90USB1287.

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

/** @file USB/Core/AVR8/USB_DeviceController.h
 *  @brief USB device controller definition for AT90USB1287.
 *
 *  This file contains the prototypes and definitions for AT90USB1287 as USB device.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef USB_DEVICECONTROLLER_H_
#define USB_DEVICECONTROLLER_H_

 #include "USB/Core/Common.h"
 #include "USB/Core/StandardRequest.h"
 
 /** @brief			Set the USB speed of the device.
  *  @param Speed	USB speed option
  */
 static inline void USBDevice_SetSpeed(const USB_Speed_t Speed) __attribute__ ((always_inline));
 static inline void USBDevice_SetSpeed(const USB_Speed_t Speed)
 {
	 if(Speed == USB_SPEED_HIGH)
	 {
		 UDCON &= ~(0x01 << LSM);
	 }
	 else
	 {
		 UDCON |= (0x01 << LSM);
	 }
 }

 /** @brief			Set the USB device address of the device.
  *  @param Address	USB device address
  */
 static inline void USBDevice_SetAddress(const uint8_t Address) __attribute__ ((always_inline));
 static inline void USBDevice_SetAddress(const uint8_t Address)
 {
	 UDADDR = (Address & 0x7F);
 }

 /** @brief	Enable the USB address of the device.
  */
 static inline void USBDevice_EnableAddress(void) __attribute__ ((always_inline));
 static inline void USBDevice_EnableAddress(void)
 {
	 UDADDR |= (0x01 << ADDEN);
 }

 /** @brief		Get the frame number.
  *  @return	Frame number
  */
 static inline uint16_t USBDevice_GetFrameNumber(void) __attribute__ ((always_inline));
 static inline uint16_t USBDevice_GetFrameNumber(void)
 {
	 return UDFNUM;
 }

#endif /* USB_DEVICECONTROLLER_H_  */ 