/*
 * MassStorage_Common.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Common definitions for USB mass storage class.

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

/** @file Common/Services/USB/Class/HID/HID_Common.h
 *  @brief Common definitions for USB mass storage class.
 *		   See https://www.usb.org/sites/default/files/usb_msc_cbi_1.1.pdf for more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef MASSSTORAGE_COMMON_H_
#define MASSSTORAGE_COMMON_H_

 #include "Common/Common.h"

 /** @ingroup USB-MS
  *  USB mass storage class specific reports
  *  NOTE: Please take a look at https://www.usb.org/sites/default/files/usb_msc_cbi_1.1.pdf if you need more information. */
 /*\@{*/
 typedef enum 
 {
	MS_REQUEST_GET_MAX_LUN = 0xFE,										/**< Get the total Logical Units (LUNs) in the SCSI device */
	MS_REQUEST_RESET = 0xFF,											/**< Reset the Mass Storage interface */
 } USB_MS_ClassRequests_t;
 /*\@}*/
 
#endif /* MASSSTORAGE_COMMON_H_ */