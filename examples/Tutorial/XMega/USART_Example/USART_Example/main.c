/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USART example for XMega.

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

/** @file main.c
 *  @brief USART example for XMega.
 *
 *  Software for the XMega USART tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-usart/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "RingBuffer/RingBuffer.h"

#define BUFFER_SIZE				8

uint8_t USART_Data[BUFFER_SIZE];
RingBuffer_t Buffer;

int main(void)
{
	/*
		Initialize the ring buffer
	*/
	RingBuffer_Init(&Buffer, USART_Data, BUFFER_SIZE);
	
	/*
		Initialize the USART
			-> 19200 Baud @ 2 MHz with CLK2X = 0, BSCALE = -5
			-> Rx Interrupt
			-> Use Rx and Tx
			-> 8N1
	*/
	USARTC0.BAUDCTRLA = 0xB0 & 0xFF;
	USARTC0.BAUDCTRLB = ((0xB0 & 0xF00) >> 0x08);
	USARTC0.BAUDCTRLB |= ((-5 & 0x0F) << 0x04);
	USARTC0.CTRLA = USART_RXCINTLVL_LO_gc;
	USARTC0.STATUS |= USART_RXCIF_bm;
	USARTC0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
	USARTC0.CTRLC &= ~(USART_PMODE0_bm | USART_PMODE1_bm | USART_SBMODE_bm);
	
	/*
		Configure I/O ports
			-> C2 as input
			-> C3 as output
	*/
	PORTC.DIR = 0x08;
	
	/*
		Send a single character
	*/
	while(!(USARTC0.STATUS & USART_DREIF_bm));
	USARTC0.DATA = 'G';
	
	/*
		Send a message
	*/
	char* Data = "Hello, World";
	while(*Data)
	{
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = *Data++;
	}
	
	/*
		Send CR + LF
	*/
	while(!(USARTC0.STATUS & USART_DREIF_bm));
	USARTC0.DATA = 0x0D;
	while(!(USARTC0.STATUS & USART_DREIF_bm));
	USARTC0.DATA = 0x0A;
	
	/*
		Enable interrupts
			-> Priority low
	*/
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();
	
	while(1) 
	{
	}
}

ISR(USARTC0_RXC_vect)
{
	uint8_t Data = USARTC0.DATA;
	RingBuffer_Save(&Buffer, Data);
	USARTC0.DATA = Data;
	
	// Send the buffer if its full
	if(RingBuffer_IsFull(&Buffer))
	{		
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0D;
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0A;
			
		for(uint8_t i = 0; i < BUFFER_SIZE; i++)
		{	
			while(!(USARTC0.STATUS & USART_DREIF_bm));
			USARTC0.DATA = RingBuffer_Load(&Buffer);
		}
		
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0D;
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0A;
	}
}

