/*
 * StandardRequest.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: 

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

/** @file Services/USB/Core/StandardRequest.h
 *  @brief
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef STANDARDREQUEST_H_
#define STANDARDREQUEST_H_

 #include "Common/Common.h"
 
 /** @ingroup 	USB
  *  @brief		USB standard requests.
  */
 typedef enum
 {
	 REQUEST_GET_STATUS = 0x00,											/**< Get Status request */
	 REQUEST_CLEAR_FEATURE = 0x01,										/**< Clear feature request */
	 REQUEST_SET_FEATURE = 0x03,										/**< Set feature request */
	 REQUEST_SET_ADDRESS = 0x05,										/**< Set address request */
	 REQUEST_GET_DESCRIPTOR = 0x06,										/**< Get descriptor request */
	 REQUEST_SET_DESCRIPTOR = 0x05,										/**< Set descriptor request */
	 REQUEST_GET_CONFIGURATION = 0x08,									/**< Get configuration request */
	 REQUEST_SET_CONFIGURATION = 0x09,									/**< Set configuration request */
	 REQUEST_GET_INTERFACE = 0x0A,										/**< Get interface request */
	 REQUEST_SET_INTERFACE = 0x0B,										/**< Set interface request */
	 REQUEST_SYNCH_FRAME = 0x0C,										/**< Sync frame request */
 } USB_Request_t;

 /** @ingroup 	USB
  *  @brief		USB request directions.
  */
 typedef enum
 {
	 REQUEST_DIRECTION_HOST_TO_DEVICE = (0x00 << 0x07),					/**< From Host to Device */
	 REQUEST_DIRECTION_DEVICE_TO_HOST = (0x01 << 0x07),					/**< From Device to Host */
 } USB_RequestDirection_t;
 
 /** @ingroup 	USB
  *  @brief		USB standard request types.
  */
 typedef enum
 {
	 REQUEST_TYPE_STANDARD = (0x00 << 0x05),							/**< Request type Standard */
	 REQUEST_TYPE_CLASS = (0x01 << 0x05),								/**< Request type Class */
	 REQUEST_TYPE_VENDOR = (0x02 << 0x05),								/**< Request type Vendor */
 } USB_RequestType_t;

 /** @ingroup 	USB
  *  @brief		USB standard request recipients.
  */
 typedef enum
 {
	  REQUEST_RECIPIENT_DEVICE = (0x00 << 0x00),						/**< Recipient Device */
	  REQUEST_RECIPIENT_INTERFACE = (0x01 << 0x00),						/**< Recipient Interface */
	  REQUEST_RECIPIENT_ENDPOINT = (0x02 << 0x00),						/**< Recipient Endpoint */
	  REQUEST_RECIPIENT_OTHER = (0x03 << 0x00),							/**< Other recipient */
 } USB_RequestRecipient_t;

 /** @ingroup 	USB
  *  @brief		USB feature selector.
  */
 typedef enum
 {
	 REQUEST_FEATURE_ENDPOINT_HALT = 0x00,								/**< Select the endpoint HALT feature */
	 REQUEST_FEATURE_DEVICE_WAKE = 0x01,								/**< Select the device remote wakeup */
	 REQUEST_FEATURE_DEVICE_TEST = 0x02,								/**< Select the device test mode */
 } USB_FeatureSelector_t;

 /** @ingroup 	USB
  *  @brief		USB test mode selector.
  */
 typedef enum
 {
	 REQUEST_TEST_TEST_J = 0x01,										/**< Test mode Test_J selector */
	 REQUEST_TEST_TEST_K = 0x02,										/**< Test mode Test_K selector */
	 REQUEST_TEST_TEST_SE0_NAK = 0x03,									/**< Test mode Test_SE0_NAK selector */
	 REQUEST_TEST_TEST_PACKET = 0x04,									/**< Test mode Test_Packet selector */
	 REQUEST_TEST_TEST_FORCE_ENABLE = 0x05,								/**< Test mode Test_Force_Enable selector */
	 REQUEST_TEST_STANDARD = 0x06,										/**< Standard test selectors (0x06 - 0x3F) */
	 REQUEST_TEST_VENDOR = 0xC0,										/**< Vendor-specific test modes (0xC0 - 0xFF) */
 } USB_TestSelector_t;

 /** @ingroup 	USB
  *  @brief		USB setup packet.
  */
 typedef struct
 {
	 uint8_t bmRequestType;												/**< Type of the request */
	 uint8_t bRequest;													/**< Specific request */
	 uint16_t wValue;													/**< Use varies according to request */
	 uint16_t wIndex;													/**< Use varies according to request */
	 uint16_t wLength;													/**< Number of bytes to transfer if there is a data stage */
 } __attribute__((packed)) USB_SetupPacket_t;
 
#endif /* STANDARDREQUEST_H_  */ 