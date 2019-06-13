/*
 * HID_Report.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Report definition for USB HID.

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

/** @file Common/Services/USB/Class/HID/HID_Report.h
 *  @brief Report definition for USB HID.
 *		   See https://www.usb.org/sites/default/files/documents/hid1_11.pdf for more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef HID_REPORT_H_
#define HID_REPORT_H_

 /** @ingroup USB-HID
  * HID Usage Pages
  * NOTE: See https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf for more information. */
 /*\@{*/
 typedef enum
 {
	 HIS_USAGE_PAGE_UNDEFINED = 0x00,						/**< Undefined */
	 HIS_USAGE_PAGE_GENERIC_DESKTOP = 0x01,					/**< Generic Desktop Controls */
	 HIS_USAGE_PAGE_SIMULATION = 0x02,						/**< Simulation Controls */
	 HIS_USAGE_PAGE_VR = 0x03,								/**< VR Controls */
	 HIS_USAGE_PAGE_SPORT = 0x04,							/**< Sport Controls */
	 HIS_USAGE_PAGE_GAME = 0x05,							/**< Game Controls */
	 HIS_USAGE_PAGE_GENERIC = 0x06,							/**< Generic Device Controls */
	 HIS_USAGE_PAGE_KEYBOARD = 0x07,						/**< Keyboard/Keypad */
	 HIS_USAGE_PAGE_LED = 0x08,								/**< LEDs */
	 HIS_USAGE_PAGE_BUTTON = 0x09,							/**< Button */
	 HIS_USAGE_PAGE_ORDINAL = 0x0A,							/**< Ordinal */
	 HIS_USAGE_PAGE_TELEPHONY = 0x0B,						/**< Telephony */
	 HIS_USAGE_PAGE_CONSUMER = 0x0C,						/**< Consumer */
	 HIS_USAGE_PAGE_DIGITIZER = 0x0D,						/**< Digitizer */
 } HID_UsagePages_t;
 /*\@}*/

 /** @ingroup USB-HID
  * HID Item types
  * NOTE: See https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf for more information. */
 /*\@{*/
 typedef enum
 {
	 HID_TYPE_MAIN = 0x00,									/**< Type: Main */
	 HID_TYPE_GLOBAL = 0x01,								/**< Type: Global */
	 HID_TYPE_LOCAL = 0x02,									/**< Type: Local */
 } HID_ReportType_t;
 /*\@}*/

 /** @ingroup USB-HID
  * HID Item sizes
  * NOTE: See https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf for more information. */
 /*\@{*/
 typedef enum
 {
	 HID_SIZE_0 = 0x00,										/**< 0 byte data */
	 HID_SIZE_1 = 0x01,										/**< 1 byte data */
	 HID_SIZE_2 = 0x02,										/**< 2 byte data */
	 HID_SIZE_3 = 0x02,										/**< 3 byte data */
 } HID_ReportSize_t;
 /*\@}*/

#endif /* HID_REPORT_H_ */ 