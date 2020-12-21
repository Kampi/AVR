/*
 * MassStorage_Common.h
 *
 *  Copyright (C) Daniel Kampert, 2020
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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Services/USB/Class/MassStorage/MassStorage_Common.h
 *  @brief Common definitions for USB mass storage class.
 *		   Please read 
 *			- https://usb.org/sites/default/files/usbmassbulk_10.pdf
 *			- https://www.seagate.com/files/staticfiles/support/docs/manual/Interface%20manuals/100293068j.pdf
 *		   when you need more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef MASSSTORAGE_COMMON_H_
#define MASSSTORAGE_COMMON_H_

 #include "Common/Common.h"

 #include "Interfaces/SCSI/SCSI.h"

 /** @brief	Command block wrapper magic number.
  */
 #define MASSSTORAGE_CBW_SIGNATURE					0x43425355UL

 /** @brief	Command status wrapper magic number.
  */
 #define MASSSTORAGE_CSW_SIGNATURE					0x53425355UL

 /** @ingroup	USB-MassStorage
  *  @brief		Standard mass storage subclass codes.
  *				NOTE: Please read https://www.usb.org/sites/default/files/Mass_Storage_Specification_Overview_v1.4_2-19-2010.pdf when you need more information.
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
 } USB_MassStorage_SubClass_t;

 /** @ingroup	USB-MassStorage
  *  @brief		Standard mass storage protocol codes.
  */
 typedef enum
 {
	 MS_PROTOCOL_CBI = 0x00,											/**< USB Mass Storage Class. Control/Bulk/Interrupt Transport */
	 MS_PROTOCOL_BBB = 0x50,											/**< USB Mass Storage Class. Bulk-Only Transport */
	 MS_PROTOCOL_UAS = 0x62,											/**< Allocated by USB-IF for UAS */
	 MS_PROTOCOL_VENDOR = 0xFF,											/**< Device vendor specific */
 } USB_MassStorage_Protocol_t;

 /** @ingroup 	USB-MassStorage
  *  @brief		USB mass storage class specific requests.
  */
 typedef enum 
 {
	MS_REQUEST_ADSC = 0x00,												/**< Assign in context by USB Mass Storage Class Control/Bulk/Interrupt (CBI) Transport, also aliases core USB request 0x00 Get Status */
	MS_REQUEST_GET_REQUESTS = 0xFC,										/**< Assigned by Lockable Storage Devices Feature Specification */
	MS_REQUEST_PUT_REQUESTS = 0xFD,										/**< Assigned by Lockable Storage Devices Feature Specification */
	MS_REQUEST_GET_MAX_LUN = 0xFE,										/**< Assigned by USB Mass Storage Class Bulk-Only (BBB) Transport */
	MS_REQUEST_RESET = 0xFF,											/**< Assigned by USB Mass Storage Class Bulk-Only (BBB) Transport */
 } USB_MassStorage_ClassRequests_t;

 /** @ingroup 	USB-MassStorage
  *  @brief		USB mass storage command status codes, used by \ref USB_MassStorage_CommandStatusWrapper_t.bCSWStatus.
  */
 typedef enum
 {
	 MS_STATUS_COMMAND_PASS = 0x00,										/**< Command Passed ("good status") */
	 MS_STATUS_COMMAND_FAIL = 0x01,										/**< Command failed */
	 MS_STATUS_COMMAND_PHASE_ERROR = 0x02,								/**< Phase error */
 } USB_MassStorage_CommandStatus_t;

 /** @ingroup 	USB-MassStorage
  *  @brief		USB mass storage flags used by \ref USB_MassStorage_CommandBlockWrapper_t.bmCBWFlags.
  */
 typedef enum
 {
	 MS_FLAGS_DIR_OUT_HOST_DEVICE = 0x00,								/**< Data-Out from host to the device */
	 MS_FLAGS_DIR_IN_DEVICE_HOST = 0x80,								/**< Data-In from device to the host */
 } USB_MassStorage_Flags_t;

 /** @ingroup 	USB-MassStorage
  *  @brief		USB mass storage class command block wrapper object.
  */
 typedef struct
 {
	 uint32_t dCBWSignature;											/**< Command block wrapper (CBW) signature. Must be #MASSSTORAGE_CBW_SIGNATURE to indicate a valid block. */
	 uint32_t dCBWTag;													/**< A Command Block Tag sent by the host. The device shall echo the contents of this field back to the \n
																			 host in the dCSWTag field of the associated CSW. The \ref USB_MassStorage_CommandBlockWrapper_t.dCSWTag positively associates a CSW with the \n
																			 corresponding CBW. */
 	 uint32_t dCBWDataTransferLength;									/**< The number of bytes of data that the host expects to transfer on the Bulk-In or Bulk-Out endpoint \n
																			 (as indicated by the Direction bit) during the execution of this command. If this field is zero, \n
																			 the device and the host shall transfer no data between the CBW and the associated CSW, and the \n
																			 device shall ignore the value of the Direction bit in \ref USB_MassStorage_CommandBlockWrapper_t.bmCBWFlags. */
	 uint8_t bmCBWFlags;												/**< The bits of this field are defined as follows: \n
																				Bit[7] Direction	The device shall ignore this bit if the \ref USB_MassStorage_CommandBlockWrapper_t.dCBWDataTransferLength field is \n
																									zero, otherwise: \n
																										0 = Data-Out from host to the device \n
																										1 = Data-In from the device to the host \n
																				Bit[6] Obsolete		The host shall set this bit to zero. \n
																				Bit[5:0] Reserved	The host shall set these bits to zero.
																				You can use the \ref USB_MassStorage_Flags_t enum for processing the values. */
	 uint8_t bCBWLUN;													/**< The device Logical Unit Number (LUN) to which the command block is being sent. For devices that \n
																			 support multiple LUNs, the host shall place into this field the LUN to which this command block is \n
																			 addressed. Otherwise, the host shall set this field to zero. */
	 uint8_t bCBWCBLength;												/**< The valid length of the CBWCB in bytes. This defines the valid length of the command block. The \n
																			 only legal values are 1 through 16 (0x01 through 0x10). All other values are reserved. */
	 uint8_t CBWCB[16];													/**< The command block to be executed by the device. The device shall interpret the first \ref USB_MassStorage_CommandBlockWrapper_t.bCBWCBLength \n
																			 bytes in this field as a command block as defined by the command set identified by \ref USB_InterfaceDescriptor_t.bInterfaceSubClass. \n
																			 If the command set supported by the device uses command blocks of fewer than 16 (0x10) bytes in \n
																			 length, the significant bytes shall be transferred first, beginning with the byte at offset 15 (0x0F). The \n
																			 device shall ignore the content of the CBWCB field past the byte at offset (15 + \ref USB_MassStorage_CommandBlockWrapper_t.bCBWCBLength - 1). */
 } __attribute__((packed)) USB_MassStorage_CommandBlockWrapper_t;

 /** @ingroup 	USB-MassStorage
  *  @brief		USB mass storage class command status wrapper object.
  */
 typedef struct
 {
	 uint32_t dCSWSignature;											/**< Command status wrapper (CSW) signature. Must be #MASSSTORAGE_CSW_SIGNATURE to indicate a valid block. */
	 uint32_t dCSWTag;													/**< The device shall set this field to the value received in the \ref USB_MassStorage_CommandBlockWrapper_t.dCBWTag of the associated CBW. */
	 uint32_t dCSWDataResidue;											/**< For Data-Out the device shall report in the \ref USB_MassStorage_CommandStatusWrapper_t.dCSWDataResidue the difference between the amount of \n
																			 data expected as stated in the \ref USB_MassStorage_CommandBlockWrapper_t.dCBWDataTransferLength, and the actual amount of data processed by \n
																			 the device. For Data-In the device shall report in the \ref USB_MassStorage_CommandStatusWrapper_t.dCSWDataResidue the difference between the \n
																			 amount of data expected as stated in the \ref USB_MassStorage_CommandBlockWrapper_t.dCBWDataTransferLength and the actual amount of relevant \n
																			 data sent by the device. The \ref USB_MassStorage_CommandStatusWrapper_t.dCSWDataResidue shall not exceed the value sent in the \n
																			 \ref USB_MassStorage_CommandBlockWrapper_t.dCBWDataTransferLength. */
	 uint8_t  bCSWStatus;												/**< \ref USB_MassStorage_CommandStatusWrapper_t.bCSWStatus indicates the success or failure of the command. The device shall set this byte to zero if \n
																			 the command completed successfully. A non-zero value shall indicate a failure during command \n
																			 execution according to the following table: \n
																				 0x00			Command Passed ("good status") \n
																				 0x01			Command Failed \n
																				 0x02			Phase Error
																				 0x03 / 0x04	Reserved (Obsolete) \n
																				 0x05 to 0xFF	Reserved
																			 You can use the \ref USB_MassStorage_CommandStatus_t enum for processing the values. */
 } __attribute__((packed)) USB_MassStorage_CommandStatusWrapper_t;

#endif /* MASSSTORAGE_COMMON_H_ */