/*
 * USB_DeviceStream.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB device stream implementation for USB service.

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

/** @file Common/Services/USB/Core/USB_DeviceStream.h
 *  @brief USB device stream implementation for USB service.
 *
 *  This file contains the prototypes and definitions for the USB device stream.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_DEVICESTREAM_H_
#define USB_DEVICESTREAM_H_

 #include "Common/Common.h"

 /** 
  * Error codes for endpoint data stream
  */
 typedef enum
 {
	 ENDPOINT_DS_NO_ERROR = 0x00,						/**< No error */
	 ENDPOINT_DS_STALLED = 0x01,						/**< The endpoint got stalled during the transfer */
	 ENDPOINT_DS_DISCONNECT = 0x02,						/**< Device was disconnected */
	 ENDPOINT_DS_SUSPEND = 0x03,						/**< The USB bus has been suspended by the host */
	 ENDPOINT_DS_TIMEOUT = 0x04,						/**< The host failed to accept or send the next packet */
	 ENDPOINT_DS_INCOMPLETE = 0x05,						/**< Incomplete data transfer */
 } Endpoint_DS_ErrorCode_t;

 /** 
  * State codes for the endpoint control stream
  */
 typedef enum
 {
	 ENDPOINT_CS_NO_ERROR = 0x00,					/**< No error */ 
	 ENDPOINT_CS_ABORT_FROM_HOST = 0x01,			/**< Transfer aborted by host */
	 ENDPOINT_CS_DISCONNECT = 0x02,					/**< Device was disconnected */
	 ENDPOINT_CS_SUSPEND = 0x03,					/**< The device has entered the suspend mode */
 } Endpoint_CS_State_t;

 /** @brief					Send control data to the host by using an IN endpoint.
  *  @param Buffer			Pointer to data buffer
  *  @param Length			Length of data
  *	 @param RequestedLength	Requested data length from host
  *  @return				Error code
  */
 Endpoint_CS_State_t USB_DeviceStream_ControlIN(const void* Buffer, const uint16_t Length, const uint16_t RequestedLength);
 
 /** @brief					Send data to the host by using an IN endpoint.
  *  @param Buffer			Pointer to data buffer
  *  @param Length			Length of data
  *  @param BytesProcessed	Pointer to processed byte count
  *  @return				Error code
  */
 Endpoint_DS_ErrorCode_t USBDataStream_IN(const void* Buffer, uint16_t Length, uint16_t* BytesProcessed);

#endif /* USB_DEVICESTREAM_H_  */ 