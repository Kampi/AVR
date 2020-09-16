/*
 * TWIS.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega slave mode TWI.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

#include "TWI.h"

static TWI_Buffer_t SlaveBuffer;

static void TWIS_ErrorHandler(void)
{
	SlaveBuffer.Status = TWI_SLAVE_ERROR;
}

static void TWIS_AddressMatchHandler(void)
{
	SlaveBuffer.BytesReceived = 0x00;
	SlaveBuffer.BytesSend = 0x00;

	SlaveBuffer.Device->SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
}

static void TWIS_StopHandler(void)
{
	SlaveBuffer.Device->SLAVE.STATUS |= TWI_SLAVE_APIF_bm;
}

static void TWIS_DataHandler(void)
{
	uint8_t Status = SlaveBuffer.Device->SLAVE.STATUS;

	// Transmit data to the master (Read from master)
	if(Status & TWI_SLAVE_DIR_bm) 
	{
		// Check if NACK
		if(SlaveBuffer.Device->SLAVE.STATUS & TWI_SLAVE_RXACK_bm)
		{
			SlaveBuffer.Device->SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
		}
		else
		{
			SlaveBuffer.BytesSend++;
			SlaveBuffer.Device->SLAVE.DATA = SlaveBuffer.Buffer[SlaveBuffer.ReadIndex++];

			if(SlaveBuffer.ReadIndex > TWI_BUFFER_SIZE)
			{
				SlaveBuffer.ReadIndex = 0x00;
				SlaveBuffer.Status = TWI_SLAVE_BUFFEROVERFLOW;
			}

			SlaveBuffer.Device->SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
		}
	}
	// Receive data from master (Write from master)
	else 
	{
		SlaveBuffer.BytesReceived++;
		SlaveBuffer.Buffer[SlaveBuffer.WriteIndex++] = SlaveBuffer.Device->SLAVE.DATA;

		if(SlaveBuffer.WriteIndex > TWI_BUFFER_SIZE)
		{
			SlaveBuffer.Device->SLAVE.CTRLB = TWI_SLAVE_ACKACT_bm;
			SlaveBuffer.Status = TWI_SLAVE_BUFFEROVERFLOW;
		}

		SlaveBuffer.Device->SLAVE.CTRLB |= TWI_SLAVE_CMD_RESPONSE_gc;
	}
}

void TWIS_Init(uint8_t* Buffer)
{
	SlaveBuffer.Buffer = Buffer;
	SlaveBuffer.Device = &TWIE;
	SlaveBuffer.BytesReceived = 0x00;
	SlaveBuffer.BytesSend = 0x00;
	SlaveBuffer.WriteIndex = 0x00;
	SlaveBuffer.ReadIndex = 0x00;
	SlaveBuffer.Status = TWI_SLAVE_IDLE;

	TWIE.SLAVE.CTRLA = TWI_SLAVE_INTLVL_LO_gc | TWI_SLAVE_DIEN_bm | TWI_SLAVE_APIEN_bm | TWI_SLAVE_PIEN_bm | TWI_SLAVE_ENABLE_bm;

	// Set the slave address
	TWIE.SLAVE.ADDR = (TWI_SLAVE_ADDRESS << 0x01);

	// Use no mask or second device address
	TWIE.SLAVE.ADDRMASK = 0x00;
}

TWI_SlaveStatus_t TWIS_Status(void)
{
	return SlaveBuffer.Status;
}

ISR(TWIE_TWIS_vect)
{
	uint8_t Status = SlaveBuffer.Device->SLAVE.STATUS;

	/*
		Check for bus error
	*/
	if(Status & TWI_SLAVE_BUSERR_bm) 
	{
		TWIS_ErrorHandler();
	}
	/*
		Check for transmit collision
	*/
	else if(Status & TWI_SLAVE_COLL_bm) 
	{
		TWIS_ErrorHandler();
	}
	/*
		Check for address match
	*/
	else if((Status & TWI_SLAVE_APIF_bm) && (Status & TWI_SLAVE_AP_bm)) 
	{
		TWIS_AddressMatchHandler();
	}
	/*
		Check for stop condition
	*/
	else if(Status & TWI_SLAVE_APIF_bm) 
	{
		TWIS_StopHandler();
	}
	/*
		Check for data
	*/
	else if(Status & TWI_SLAVE_DIF_bm) 
	{
		TWIS_DataHandler();
	}
	/*
		Error
	*/
	else
	{
		TWIS_ErrorHandler();
	}
}