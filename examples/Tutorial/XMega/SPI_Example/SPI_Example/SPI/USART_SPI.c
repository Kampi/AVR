/*
 * USART_SPI.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega USART SPI.

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

void USART_SPI_Init(void)
{
	PORTD.DIRSET = (0x01 << 0x03) | (0x01 << 0x01);
	PORTD.DIRCLR = (0x01 << 0x02);
	
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	USARTD0.CTRLC = USART_CMODE_MSPI_gc;
	USARTD0.BAUDCTRLA = 0x03;
}

uint8_t USART_SPI_SendData(uint8_t Data)
{
	while(!(USARTD0.STATUS & USART_DREIF_bm));
    USARTD0.DATA = Data;
    
	while(!(USARTD0.STATUS & USART_TXCIF_bm));
	USARTD0.STATUS = USART_TXCIF_bm;
    return USARTD0.DATA;
}

void USART_SPI_InitInterrupt(void)
{
	PORTD.DIRSET = (0x01 << 0x03) | (0x01 << 0x01);
	PORTD.DIRCLR = (0x01 << 0x02);
	
	USARTD0.CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_LO_gc;
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	USARTD0.CTRLC = USART_CMODE_MSPI_gc;
	USARTD0.BAUDCTRLA = 0x03;
}

void USART_SPI_Transmit(SPI_Message_t* Message)
{
	// Save the message object
	CurrentMessage = *Message;
	
	// Reset the read and write counter
	CurrentMessage.BytesProcessed = 0x00;

	// Wait for active transmissions
	while(CurrentMessage.Status);

	CurrentMessage.Status = 0x00;
	
	// Select the device
	USART_SPI_Select(CurrentMessage.Port, CurrentMessage.Pin);
	
	// Send the first byte
	USARTD0.DATA = CurrentMessage.BufferOut[CurrentMessage.BytesProcessed];
}

uint8_t USART_SPI_Status(void)
{
	return CurrentMessage.Status;
}

void USART_SPI_Select(PORT_t* Port, uint8_t Pin)
{
	Port->OUTCLR = (0x01 << Pin);
}

void USART_SPI_Deselect(PORT_t* Port, uint8_t Pin)
{
	Port->OUTSET = (0x01 << Pin);
}

ISR(USARTD0_TXC_vect)
{
	if(CurrentMessage.BytesProcessed < (CurrentMessage.Length - 1))
	{
		USARTD0.DATA = CurrentMessage.BufferOut[++CurrentMessage.BytesProcessed];
	}
	else
	{
		USART_SPI_Deselect(CurrentMessage.Port, CurrentMessage.Pin);
		CurrentMessage.Status = 0x01;
	}
}

ISR(USARTD0_RXC_vect)
{
	CurrentMessage.BufferIn[CurrentMessage.BytesProcessed] = USARTD0.DATA;
}