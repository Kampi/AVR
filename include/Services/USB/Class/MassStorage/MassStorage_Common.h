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

/** @file Services/USB/Class/MassStorage/MassStorage_Common.h
 *  @brief Common definitions for USB mass storage class.
 *		   See https://www.usb.org/sites/default/files/usb_msc_cbi_1.1.pdf for more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef MASSSTORAGE_COMMON_H_
#define MASSSTORAGE_COMMON_H_

 #include "Common/Common.h"

 /** @brief Standard mass storage subclass codes.
  *			NOTE: See https://www.usb.org/sites/default/files/Mass_Storage_Specification_Overview_v1.4_2-19-2010.pdf for more information.
  */
 typedef enum
 {
	 MS_SUBCLASS_SCSI_NO = 0x00,										/**< SCSI command set not reported */
	 MS_SUBCLASS_RBC = 0x01,											/**< Allocated by USB-IF for RBC */
	 MS_SUBCLASS_MMC5 = 0x02,											/**< Allocated by USB-IF for MMC-5 */
	 MS_SUBCLASS_UFI = 0x04,											/**< Specifies how to interface Floppy Disk Drivers to USB */
	 MS_SUBCLASS_SCSI = 0x06,											/**< Allocated by USB-IF for SCSI */
	 MS_SUBCLASS_LSD_FS = 0x07,											/**< LSDFS specifies how host has to negotiate access before trying SCSI */
	 MS_SUBCLASS_IEEE1667 = 0x08,										/**< Allocated by USB-IF for IEEE 1667 */
	 MS_SUBCLASS_VENDOR = 0xFF,											/**< Device vendor specific */
 } USB_MS_SubClass_t;
 
 /** @brief Standard mass storage protocol codes.
  *			NOTE: See https://www.usb.org/sites/default/files/Mass_Storage_Specification_Overview_v1.4_2-19-2010.pdf for more information.
  */
 typedef enum
 {
	 MS_PROTOCOL_CBI = 0x00,											/**< USB Mass Storage Class. Control/Bulk/Interrupt Transport */
	 MS_PROTOCOL_BBB = 0x50,											/**< USB Mass Storage Class. Bulk-Only Transport */
	 MS_PROTOCOL_UAS = 0x62,											/**< Allocated by USB-IF for UAS */
	 MS_PROTOCOL_VENDOR = 0xFF,											/**< Device vendor specific */
 } USB_MS_Protocol_t;
 
#endif /* MASSSTORAGE_COMMON_H_ */