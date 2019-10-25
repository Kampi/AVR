/*
 * Endpoints.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB Endpoints for AT90USB1287.

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

/** @file USB/Core/AVR8/Endpoint.h
 *  @brief USB Endpoints for AT90USB1287.
 *
 *  This file contains the prototypes and definitions for the AT90USB1287 USB Endpoints.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

 #include "USB/Core/AVR8/USB_DeviceController.h"

 /** 
  * Control endpoint address for USB devices
  */
 #define ENDPOINT_CONTROL_ADDRESS						0x00
 
 /** 
  * Control endpoint size for USB devices
  */
 #define ENDPOINT_CONTROL_SIZE							0x08
 
 /** 
  * Mask for IN endpoint
  */
 #define ENDPOINT_DIR_MASK_IN							0x80

 /** 
  * Endpoint types
  */
 typedef enum
 {
	 ENDPOINT_TYPE_CONTROL = 0x00,						/**< Control endpoint */ 
	 ENDPOINT_TYPE_ISOCHRONOUS = 0x01,					/**< Isochronous endpoint */ 
	 ENDPOINT_TYPE_BULK = 0x02,							/**< Bulk endpoint */ 
	 ENDPOINT_TYPE_INTERRUPT = 0x03						/**< Interrupt endpoint */ 
 } Endpoint_Type_t;

 /** 
  * Endpoint size in bytes
  */
 typedef enum
 {
	 ENDPOINT_SIZE_8 = 0x00,							/**< Endpoint size 8 bytes */ 
	 ENDPOINT_SIZE_16 = 0x01,							/**< Endpoint size 16 bytes */ 
	 ENDPOINT_SIZE_32 = 0x02,							/**< Endpoint size 32 bytes */ 
	 ENDPOINT_SIZE_64 = 0x03,							/**< Endpoint size 64 bytes */ 
	 ENDPOINT_SIZE_128 = 0x04,							/**< Endpoint size 128 bytes */ 
	 ENDPOINT_SIZE_256 = 0x05,							/**< Endpoint size 256 bytes */ 
 } Endpoint_Size_t;

 /** 
  * Endpoint directions
  */
 typedef enum
 {
	 ENDPOINT_DIRECTION_OUT = 0x00,						/**< OUT endpoint */ 
	 ENDPOINT_DIRECTION_IN = 0x80,						/**< IN endpoint */ 
 } Endpoint_Direction_t;

 /** @brief				Configure an endpoint.
  *  @param Address		Endpoint address
  *  @param Type		Endpoint type
  *  @param Size		Endpoint size
  *  @param DoubleBank	Set to use a double bank
  *  @
  */
 uint8_t Endpoint_Configure(const uint8_t Address, const Endpoint_Type_t Type, const Endpoint_Size_t Size, const uint8_t DoubleBank);

#endif /* ENDPOINT_H_ */