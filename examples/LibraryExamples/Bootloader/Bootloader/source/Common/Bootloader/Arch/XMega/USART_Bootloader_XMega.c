/*
 * USART_Bootloader_XMega.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR USART bootloader for XMega architecture.

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
 *  @brief AVR USART bootloader for XMega architecture.
 *
 *  This file contains the implementation for the AVR USART bootloader for XMega architecture.
 *
 *  @author Daniel Kampert
 */

#include "Common/Bootloader/Bootloader.h"

/** @brief	Data buffer for the current line.
 */
static unsigned char __LineBuffer[PARSER_MAX_DATA_BYTES];

/** @brief	Intel hex parser line object.
 */
static Parser_Line_t __Line = {
		.pBuffer = __LineBuffer,
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

	// Enable the USART interface
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRSET = (0x01 << BOOTLOADER_TX);
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_RX);
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLA = BOOTLOADER_BRREG_VALUE;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLB = (BOOTLOADER_SCALE_VALUE << USART_BSCALE_gp) & USART_BSCALE_gm;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB = USART_RXEN_bm | USART_TXEN_bm;

	// Initialize the hex file parser
	Parser_Init();

	// Disable the targets flow control
	Bootloader_PutChar(XON);
}

Bool_t Bootloader_Enter(void)
{
	Bootloader_PutString("Enter bootloader...\n\r");

	uint16_t Word = 0x00;
	uint16_t Page = 0x00;

	do
	{
		Parser_State_t LineRead = Parser_GetLine(Bootloader_GetChar());
		if(LineRead == PARSER_STATE_SUCCESSFULL)
		{
			Bootloader_PutChar(XOFF);

			Parser_State_t State = IntelParser_ParseLine(&__Line);
			if(State == PARSER_STATE_SUCCESSFULL)
			{
				if(__Line.Type == PARSER_TYPE_DATA)
				{
					for(uint8_t i = 0x00; i < __Line.Bytes; i = i + 0x02)
					{
						uint16_t CodeWord = (__LineBuffer[i + 1] << 0x08) | __LineBuffer[i];
						NVM_LoadFlashBuffer((__Line.Address + i) >> 0x01, CodeWord);
					}

					Word += __Line.Bytes;

					if(Word == APP_SECTION_PAGE_SIZE)
					{
						Word = 0x00;
						NVM_FlushFlash(Page++);
					}
				}
			}
			else if(State == PARSER_STATE_ERROR)
			{
				// Error handling

				return FALSE;
			}

			Bootloader_PutChar(XON);
		}
		else
		{
			// Error handling
			
			return FALSE;
		}
	} while(__Line.Type != PARSER_TYPE_EOF);
	
	NVM_FlushFlash(Page);
	
	return TRUE;
}

void Bootloader_Exit(void)
{
	void (*Main)(void) = 0x0000;
	
	Bootloader_PutString("Leave bootloader...");
	
	// Wait until last transmission finish
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_DREIF_bm));

	// Disable the SPM command
	NVM_LockSPM();

	// Reset the I/O
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_TX);
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_RX);
	
	// Disable the USART
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);

	// Jump to application
	EIND = 0x00;
	Main();
}