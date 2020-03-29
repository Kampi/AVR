/*
 * USART_OneWire.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 tinyAVR USART module in 1-Wire mode.

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

/** @file Arch/AVR8/tinyAVR/tiny0/USART/USART.c
 *  @brief 1-Wire driver for Atmel AVR8 tiny0 USART module.
 *
 *  This contains the implementation of the tiny0 USART 1-Wire driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/tinyAVR/tiny0/USART/USART.h"

/** @brief 1-Wire logical zero pattern @ 115200 baud.
 */
#define USART_ONEWIRE_WRITE_0			0x00

/** @brief 1-Wire logical one pattern @ 115200 baud.
 */
#define USART_ONEWIRE_WRITE_1			0xFF

/** @brief 1-Wire read bit pattern @ 115200 baud.
 */
#define USART_ONEWIRE_READ				0xFF

/** @brief 1-Wire read reset pattern @ 9600 baud.
 */
#define USART_ONEWIRE_RESET				0xF0

/** @brief Baudrate value for 115200 baud.
 */
static uint32_t _Baud115200 = 0x00;

/** @brief Baudrate value for 9600 baud.
 */
static uint16_t _Baud9600 = 0x00;

/** @brief		Transmit a data package over the USART 1-Wire interface and receive the answer.
 *  @param Data	Data byte
 *  @return		Answer from device
 */
static bool USART_OneWire_ReadWrite(const uint8_t Data)
{
	USART0.TXDATAL = Data;
	while(!(USART0.STATUS & USART_RXCIF_bm));
	
	return (bool)USART0.RXDATAL;
}

void USART_OneWire_Init(void)
{
	PORTB.DIRSET = (0x01 << 0x02);
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;

	USART0.CTRLA = USART_LBME_bm;
	USART0.CTRLB = USART_ODME_bm | USART_RXEN_bm | USART_TXEN_bm;
	USART0.CTRLC = USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc;

	// Check if the main clock prescaler is enabled and get the prescaler
	uint32_t Clock = SysClock_GetClockPer();

	// Get the values for the necessary baudrates
	_Baud115200 = (Clock << 0x02) / 115200;
	_Baud9600 = (Clock << 0x02) / 9600;
	USART0.BAUD = _Baud115200;
}

void USART_OneWire_WriteBit(const bool Bit)
{
	if(Bit)
	{
		USART_OneWire_ReadWrite(USART_ONEWIRE_WRITE_1);
	}
	else
	{
		USART_OneWire_ReadWrite(USART_ONEWIRE_WRITE_0);
	}
}

uint8_t USART_OneWire_ReadBit(void)
{
	return (USART_OneWire_ReadWrite(USART_ONEWIRE_READ) == USART_ONEWIRE_READ);
}

bool USART_OneWire_Reset(void)
{
	bool Presence = false;

	USART0.CTRLB &= ~(USART_RXEN_bm);
	USART0.CTRLB |= (USART_RXEN_bm);

	// Switch baudrate to 9600
	USART0.BAUD = _Baud9600;

	Presence = USART_OneWire_ReadWrite(USART_ONEWIRE_RESET);

	// Switch baudrate back to 115200
	USART0.BAUD = _Baud115200;
	
	return Presence;
}