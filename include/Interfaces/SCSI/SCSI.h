/*
 * SCSI.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: SCSI interface driver for AVR.

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

/** @file Interfaces/SCSI/SCSI.h
 *  @brief SCSI interface driver for AVR.
 *
 *  This file contains the prototypes and definitions for the SCSI interface.
 *
 *  @author Daniel Kampert
 *  @bugs	No known bugs
 */

#ifndef SCSI_H_
#define SCSI_H_

 #include "Common/Common.h"

 /** @ingroup 	SCSI
  *  @brief		SCSI command codes. Please refer the SCSI commands reference for more information.
  */
 typedef enum
 {
	 SCSI_CMD_REQUEST_SENSE = 0x03,										/**< REQUEST SENSE command */
	 SCSI_CMD_INQUIRY = 0x12,											/**< INQUIRY command */
 } SCSI_Commands_t;

 /** @ingroup 	SCSI
  *  @brief		Peripheral devices types for the \ref SCSI_Inquiry_t structure.
  */
 typedef enum
 {
	 SCSI_TYPE_DIRECT = 0x00,											/**< Direct access block device (e.g., magnetic disk). */
 } SCSI_Peripheral_Type_t;

 /** @ingroup 	SCSI
  *  @brief		Sense key and sense code definitions for the \ref SCSI_Sense_t structure.
  *			    Please read chapter 2.4.1.5 of the SCSI commands reference for more informations.
  */
 typedef enum
 {
	 SCSI_SENSE_NO = 0x00,												/**< Indicates that there is no specific sense key information to be reported. \n
																			 This may occur for a successful command or for a command that receives \n
																			 CHECK CONDITION status because one of the FILEMARK, EOM, or ILI bits is set to one. */
	 SCSI_RECOVERED_ERROR = 0x01,										/**< Indicates that the command completed successfully, with some recovery action performed \n
																			 by the device server. Details may be determined by examining the additional sense bytes \n
																			 and the INFORMATION field. When multiple recovered errors occur during one command, the \n
																			 choice of which error to report (e.g., first, last, most severe) is vendor specific. */
	 SCSI_NOT_READY = 0x02,												/**< Indicates that the logical unit is not accessible. Operator intervention may be required to correct this condition. */
	 SCSI_MEDIUM_ERROR = 0x03,											/**< Indicates that the command terminated with a non-recovered error condition that may have \n
																			 been caused by a flaw in the medium or an error in the recorded data. This sense key may \n
																			 also be returned if the device server is unable to distinguish between a flaw in the medium \n
																			 and a specific hardware failure (i.e., sense key 4h). */
	 SCSI_HARDWARE_ERROR = 0x04,										/**< Indicates that the device server detected a non-recoverable hardware failure (e.g., controller failure, \n
																			 device failure, or parity error) while performing the command or during a self test. */
	 SCSI_ILLEGAL_REQUEST = 0x05,										/**< Indicates that: \n
																				a) The command was addressed to an incorrect logical unit number (see SAM-5); \n
																				b) The command had an invalid task attribute (see SAM-5); \n
																				c) The command was addressed to a logical unit whose current configuration prohibits processing the command; \n
																				d) There was an illegal parameter in the CDB; or \n
																				e) There was an illegal parameter in the additional parameters supplied as data for some commands (e.g., PERSISTENT \n
																				   RESERVE OUT). \n
																			 If the device server detects an invalid parameter in the CDB, it shall terminate the command without altering \n
																			 the medium. If the device server detects an invalid parameter in the additional parameters supplied as data, \n
																			 the device server may have already altered the medium. */
	SCSI_UNIT_ATTENTION = 0x06,											/**< Indicates that a unit attention condition has been established (e.g., the removable medium may have been \n
																			 changed, a logical unit reset occurred). See SAM-5. */
	SCSI_DATA_PROTECT = 0x07,											/**< Indicates that a command that reads or writes the medium was attempted on a block that is protected. The read \n
																			 or write operation is not performed. */
	SCSI_BLANK_CHECK = 0x08,											/**< Indicates that a write-once device or a sequential-access device encountered blank medium or format-defined \n
																			 end-of-data indication while reading or that a write-once device encountered a non-blank medium while writing. */
	SCSI_VENDOR_SPECIFIC = 0x09,										/**< This sense key is available for reporting vendor specific conditions. */
	SCSI_COPY_ABORTED = 0x0A,											/**< Indicates an EXTENDED COPY command was aborted due to an error condition on the source device, the \n
																			 destination device, or both. */
	SCSI_ABORTED_COMMAND = 0x0B,										/**< Indicates that the device server aborted the command. The application client may be able to recover by \n
																			 trying the command again. */
	SCSI_VOLUME_OVERFLOW = 0x0D,										/**< Indicates that a buffered SCSI device has reached the end-of-partition and data may remain in the buffer \n
																			 that has not been written to the medium. One or more RECOVER BUFFERED DATA command(s) may be issued to read the \n
																			 unwritten data from the buffer. (See SSC-3.) */
	SCSI_MISCOMPARE = 0x0E,												/**< Indicates that the source data did not match the data read from the medium. */
	SCSI_COMPLETED = 0x0F,												/**< Indicates there is command completed sense data (see SAM-5) to be reported. This may occur for a successful command. */
 } SCSI_SenseKey_t;

 /** @ingroup 	SCSI
  *  @brief		Standard inquiry data for the INQUIRY command. See page 94 of the SCSI commands reference
  *				for more information.
  */
 typedef struct
 {
	 uint8_t PeripheralDeviceType:5;
	 uint8_t PeripheralQualifier:3;

	 uint8_t Reserved:7;
	 uint8_t Removable:1;

	 uint8_t Version;

	 uint8_t ResponseDataFormat:4;
	 uint8_t HISUP:1;
	 uint8_t NORMACA:1;
	 uint8_t Obsolete:2;

	 uint8_t AdditionalLength;

	 uint8_t Protect:1;
	 uint8_t Reserved1:2;
	 uint8_t PC:1;
	 uint8_t TPGS:1;
	 uint8_t ACC:1;
	 uint8_t SCCS:1;

	 uint8_t Obsolete2:4;
	 uint8_t MULTIP:1;
	 uint8_t VS1:1;
	 uint8_t ENCSERV:1;
	 uint8_t Obsolete3:1;

	 uint8_t VS2:1;
	 uint8_t CMDQUE:1;
	 uint8_t Obsolete4:6;

	 uint8_t VendorID[8];
	 uint8_t ProductID[16];
	 uint8_t ProductRev[4];
 } __attribute__((packed)) SCSI_Inquiry_t;

 /** @ingroup 	SCSI
  *  @brief		Fixed format sense data. See page 56 of the SCSI commands reference
  *				for more information.
  */
 typedef struct
 {
	 uint8_t ResponseCode:7;
	 uint8_t Valid:1;
	 uint8_t Obsolete;
	 uint8_t SenseKey:4;												/**< Sense key code. Use the values from the \ref SCSI_SenseKey_t enum. */
	 uint8_t Reserved:1;
	 uint8_t ILI:1;
	 uint8_t EOM:1;
	 uint8_t FileMark:1;
	 uint8_t Information[4];
	 uint8_t AdditionalSenseLength;
	 uint8_t CmdSpecificInformation[4];
	 uint8_t AdditionalSenseCode;										/**< Additional sense code field. See table 29 in the SCSI commands reference. */
	 uint8_t AdditionalSenseQualifier;
	 uint8_t FieldReplaceableUnitCode;
	 uint8_t SenseKeySpecific[3];
 } __attribute__((packed)) SCSI_Sense_t;

 extern const SCSI_Inquiry_t SCSI_DeviceInquiryData;

 /** @brief				Decode a SCSI command.
  *  @param Data		Pointer to command data		
  *  @param Size		Length of command data
  *	 @param Transfered	Pointer to transfered data bytes
  *  @return			#true when successfully
  */
 bool SCSI_DecodeCommand(uint8_t* Data, uint8_t Size, uint16_t* Transfered);

#endif /* SCSI_H_ */