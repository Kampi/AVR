/*
 * SCSI.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: SCSI implementation for AVR.

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

/** @file Interfaces/SCSI/SCSI.c
 *  @brief SCSI implementation for AVR.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#if(SCSI_INTERFACE == INTERFACE_USB)
	#include "Services/USB/Core/USB_DeviceStream.h"
	#define SCSI_WRITE_STREAM(Buffer, Length, Offset)		USB_DeviceStream_DataIN(Buffer, Length, Offset)
#else
	#error "Interface not supported for SCSI!"
#endif

#include "Interfaces/SCSI/SCSI.h"

/** @brief	Structure to hold the sense data information for the last issued SCSI command. This information is
 *			returned to the host after a SCSI REQUEST SENSE command is send.
 */
static SCSI_Sense_t _SCSI_SenseData =
{
	.ResponseCode = 0x70,
};

/** @brief				Extract the parameter from the data and execute the INQUIRY command.
 *  @param Data			Pointer to command data array
 *  @param Size			Length of the data array
 *  @param Transfered	Pointer to transfered data bytes
 *  @return				#true when successful
 */
static bool SCSI_Inquiry(uint8_t* Data, uint8_t Size, uint16_t* Transfered)
{
	uint16_t AllocationLength;

	// Check for invalid length
	if(Size != 0x06)
	{
		return false;
	}

	// Check for EVPD and CMDDT = 1 and report an illegal request
	// Set the sense key to ILLEGAL REQUEST
	// Set the additional sense key and the sense qualifier to INVALID FIELD IN CDB
	// (see SCSI command reference page 62 for more information)
	if((Data[1] & ((0x01 << 0x00) | (0x01 << 0x01))) || Data[2])
	{
		_SCSI_SenseData.SenseKey = SCSI_ILLEGAL_REQUEST;
		_SCSI_SenseData.AdditionalSenseCode = 0x24;
		_SCSI_SenseData.AdditionalSenseQualifier = 0x00;

		return false;
	}

	// Get the allocation length from the data (allocation length use big-endian format)
	AllocationLength = (*(uint16_t*)&Data[3]);
	#if(defined(MCU_LITTLE_ENDIAN))
		AllocationLength = ENDIAN_SWAP_16(AllocationLength);
	#endif

	if(AllocationLength > sizeof(SCSI_DeviceInquiryData))
	{
		AllocationLength = sizeof(SCSI_DeviceInquiryData);
	}

	// Start the data transmission
	SCSI_WRITE_STREAM(&SCSI_DeviceInquiryData, AllocationLength, Transfered);

	return true;
}

/** @brief				Extract the parameter from the data and execute the REQUEST SENSE command.
 *  @param Data			Pointer to command data array
 *  @param Size			Length of the data array
 *  @param Transfered	Pointer to transfered data bytes
 *  @return				#true when successful
 */
static bool SCSI_RequestSense(uint8_t* Data, uint8_t Size, uint16_t* Transfered)
{
	uint8_t AllocationLength;

	// Check for invalid length
	if(Size != 0x06)
	{
		return false;
	}

	// Get the allocation length from the data
	AllocationLength = Data[4];

	if(AllocationLength > sizeof(_SCSI_SenseData))
	{
		AllocationLength = sizeof(_SCSI_SenseData);
	}

	// Start the data transmission
	SCSI_WRITE_STREAM(&_SCSI_SenseData, AllocationLength, Transfered);

	return true;
}

bool SCSI_DecodeCommand(uint8_t* Data, uint8_t Size, uint16_t* Transfered)
{
	bool Successful = false;

	switch(Data[0])
	{
		case SCSI_CMD_INQUIRY:
		{
			Successful = SCSI_Inquiry(Data, Size, Transfered);

			break;
		}
		case SCSI_CMD_REQUEST_SENSE:
		{
			Successful = SCSI_RequestSense(Data, Size, Transfered);

			break;
		}
		default:
		{
			// Check for an invalid command
			// Set the sense key to ILLEGAL REQUEST
			// Set the additional sense key and the sense qualifier to INVALID COMMAND OPERATION CODE
			// (see SCSI command reference page 62 for more information)
			_SCSI_SenseData.SenseKey = SCSI_ILLEGAL_REQUEST;
			_SCSI_SenseData.AdditionalSenseCode = 0x20;
			_SCSI_SenseData.AdditionalSenseQualifier = 0x00;

			break;
		}
	}

	// Set the sense data for a successful command
	if(Successful)
	{
		_SCSI_SenseData.SenseKey = SCSI_SENSE_NO;
		_SCSI_SenseData.AdditionalSenseCode = 0x00;
		_SCSI_SenseData.AdditionalSenseQualifier = 0x00;
	}

	return Successful;
}