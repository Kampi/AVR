/*
 * USART_Bootloader_XMega.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USART bootloader for XMega architecture.

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

/** @file Common/Bootloader/Arch/XMega/USART_Bootloader_XMega.c
 *  @brief USART bootloader for XMega architecture.
 *
 *  This file contains the implementation for the USART bootloader for XMega architecture.
 *
 *  @author Daniel Kampert
 */

#include "Bootloader/Bootloader.h"

/** @brief	Data buffer for the current line.
 */
static unsigned char _LineBuffer[PARSER_MAX_DATA_BYTES];

/** @brief	Data block object for the parsing engine.
 */
static Parser_Block_t _Line = {
		.pBuffer = _LineBuffer,
	};

/** @brief	Receive a single character with the USART.
 *	@return	Received character
 */
static unsigned char Bootloader_GetChar(void)
{
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_RXCIF_bm));
	return ((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA;
}

/** @brief		Transmit a single character with the USART.
 *	@param Data	Data byte to transmit
 */
static void Bootloader_PutChar(const unsigned char Data)
{
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_DREIF_bm));
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA = Data;
}

/** @brief		Transmit a zero ended character array with the USART.
 *	@param Data	Pointer to character array
 */
static void Bootloader_PutString(const char* Message)
{
	while(*Message)
	{
		Bootloader_PutChar(*Message++);
	}
}

void Bootloader_Init(void)
{
	// Remove the old application
	NVM_EraseApplication();

	// Enable the default clock
	asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLK.CTRL), 
						"r" (0x00), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);

	// Configure the I/O for the USART interface
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRSET = (0x01 << BOOTLOADER_TX);
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_RX);

	// Enable and configure the USART interface
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLA = (BOOTLOADER_BRREG_VALUE & 0xFF) << USART_BSEL_gp;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLB = (BOOTLOADER_SCALE_VALUE << USART_BSCALE_gp) & USART_BSCALE_gm;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB = USART_RXEN_bm | USART_TXEN_bm;

	// Initialize the hex file parser
	Parser_Init();

	// Enable the transmitter by disabling the flow control
	Bootloader_PutChar(XON);
}

Bool_t Bootloader_Enter(void)
{
	uint16_t Page = 0x00;
	uint8_t Offset = 0x00;
	uint16_t Words = 0x00;

	Bootloader_PutString("Enter bootloader...\n\r");

	do
	{
		if(Parser_GetByte(Bootloader_GetChar()) == PARSER_STATE_SUCCESSFUL)
		{
			// Disable the transmitter
			Bootloader_PutChar(XOFF);

			// Parsing successfully?
			if((Parser_Parse(&_Line) == PARSER_STATE_SUCCESSFUL) && _Line.Valid)
			{
				if(_Line.Type == PARSER_TYPE_DATA)
				{
					uint32_t Address = (_Line.Offset + _Line.Address) >> 0x01;
					Offset = Address & 0xFF;
					Page = (Address & 0x3FF00) >> 0x08;

					for(uint8_t i = 0x00; i < _Line.Length; i += 0x02)
					{
						uint16_t CodeWord = (_LineBuffer[i + 1] << 0x08) | _LineBuffer[i];
						NVM_LoadFlashBuffer(Offset + (i >> 0x01), CodeWord);

						// Write the page when the buffer is full
						if(Words++ == (APP_SECTION_PAGE_SIZE / 2))
						{
							NVM_FlushFlash(Page);
							Words = 0x00;
						}
					}
				}
			}
			else
			{
				// Error handling

				return FALSE;
			}

			// Enable the transmitter
			Bootloader_PutChar(XON);
		}
	} while(_Line.Type != PARSER_TYPE_EOF);

	NVM_FlushFlash(Page);

	return TRUE;
}

void Bootloader_Exit(void)
{
	Bootloader_PutString("Leave bootloader...\n\r");

	// Wait until last transmission has finished
	for(uint16_t i = 0x00; i < 0xFFFF; i++);	

	// Disable the SPM command
	NVM_LockSPM();

	// Reset the I/O
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_TX) | (0x01 << BOOTLOADER_RX);

	// Disable the USART interface
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);

	// Clear the extended indirect register and jump to the main application
	EIND = 0x00;
	asm volatile("ld   %0, Z" "\n\t"
				 "ijmp"		  "\n\t"
			::	 "r" (_Line.StartAddress)
			:    "r30", "r31"
		);
}