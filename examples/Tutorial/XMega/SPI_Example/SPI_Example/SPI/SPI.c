/*
 * SPIM.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega master mode SPI.

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

#include "SPI.h"

static SPI_Message_t CurrentMessage;
static SPI_Buffer_t SlaveBuffer;

static uint8_t SPI_Master = 0x00;

void SPIM_Init(void)
{
	SPI_Master = 0x01;
	
	PORTC.DIRSET = (0x01 << 0x07) | (0x01 << 0x05) | (0x01 << 0x04);
	
	SPIC.CTRL |= (SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV4_gc);
}

void SPIM_InitInterrupt(void)
{
	SPI_Master = 0x01;
	
	PORTC.DIRSET = (0x01 << 0x07) | (0x01 << 0x05) | (0x01 << 0x04);
	
	SPIC.CTRL |= (SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV4_gc);
	SPIC.INTCTRL = SPI_INTLVL_LO_gc;
	
	CurrentMessage.Status = 0x00;
}

uint8_t SPIM_SendData(uint8_t Data)
{
	SPIC.DATA = Data;
	
	while(!(SPIC.STATUS & SPI_IF_bm));
	
	return SPIC.DATA;
}

void SPIM_Select(PORT_t* Port, uint8_t Pin)
{
	Port->OUTCLR = (0x01 << Pin);
}

void SPIM_Deselect(PORT_t* Port, uint8_t Pin)
{
	Port->OUTSET = (0x01 << Pin);
}

void SPIM_Transmit(SPI_Message_t* Message)
{
	// Save the message object
	CurrentMessage = *Message;
	
	// Reset the read and write counter
	CurrentMessage.BytesProcessed = 0x00;

	// Wait for active transmissions
	while(CurrentMessage.Status);

	CurrentMessage.Status = 0x00;
	
	// Select the device
	SPIM_Select(CurrentMessage.Port, CurrentMessage.Pin);
	
	// Send the first byte
	SPIC.DATA = CurrentMessage.BufferOut[CurrentMessage.BytesProcessed];
}

uint8_t SPIM_Status(void)
{
	return CurrentMessage.Status;
}

void SPIS_Init(uint8_t* ReceiveBuffer, uint8_t* TransmitBuffer)
{
	SPI_Master = 0x00;
	
	PORTC.DIRSET = (0x01 << 0x06);
	
	SPIC.CTRL |= (SPI_ENABLE_bm | SPI_MODE_0_gc);
	SPIC.INTCTRL = SPI_INTLVL_LO_gc;
	
	// Create a new SPI buffer object
	SlaveBuffer.BytesProcessed = 0x00;
	SlaveBuffer.RxBuffer = ReceiveBuffer;
	SlaveBuffer.TxBuffer = TransmitBuffer;
	SlaveBuffer.Status = 0x00;

	SPIS_InitBuffer(TransmitBuffer);
}

void SPIS_InitBuffer(uint8_t* TransmitBuffer)
{
	// Place the first byte in the data register
	SPIC.DATA = SlaveBuffer.TxBuffer[0x00];
}

uint8_t SPIS_ActiveTransmission(void)
{
	return (!(PORTC.IN & (0x01 << 0x04)) >> 0x04);
}

ISR(SPIC_INT_vect)
{
	// Check if master or slave mode is used
	if(SPI_Master)
	{
		CurrentMessage.BufferIn[CurrentMessage.BytesProcessed] = SPIC.DATA;
		
		if(CurrentMessage.BytesProcessed < (CurrentMessage.Length - 1))
		{
			SPIC.DATA = CurrentMessage.BufferOut[++CurrentMessage.BytesProcessed];
		}
		else
		{
			SPIM_Deselect(CurrentMessage.Port, CurrentMessage.Pin);
			CurrentMessage.Status = 0x01;
		}
	}
	else
	{
		SlaveBuffer.RxBuffer[SlaveBuffer.BytesProcessed] = SPIC.DATA;
		
		if(!(SlaveBuffer.BytesProcessed < (SPI_BUFFER_SIZE - 1)))
		{
			SPIC.DATA = SlaveBuffer.TxBuffer[++SlaveBuffer.BytesProcessed];
		}
		else
		{
			SlaveBuffer.BytesProcessed = 0x00;
			SlaveBuffer.Status = 0x01;
		}
	}
}