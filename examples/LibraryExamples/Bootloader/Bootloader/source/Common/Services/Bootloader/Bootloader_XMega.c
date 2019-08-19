/*
 * Bootloader_XMega.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR bootloader for XMega architecture.

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

/** @file Common/Services/Bootloader/Bootloader_XMega.c
 *  @brief AVR bootloader for XMega architecture.
 *
 *  This file contains the implementation for the AVR XMega architecture.
 *
 *  @author Daniel Kampert
 */

#include "Common/Services/Bootloader/Bootloader.h"

static Bootloader_Parser_t __ParserState;

/** @brief			Convert a hexadecimal string number into an integer value.
 *  @param ASCII	Number as string
 *  @param Length	String length
 *  @return			Integer value of the string
 */
static uint16_t Hex2Num(const char* ASCII, const uint8_t Length)
{
	uint16_t Temp = 0;
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		uint8_t c = ASCII[i];
		
		if (c >= '0' && c <= '9')
		{
			c -= '0';
		}
		else if(c >= 'A' && c <= 'F')
		{
			c -= 'A' - 10;
		}
		else if(c >= 'a' && c <= 'f')
		{
			c -= 'a' - 10;
		}
		
		Temp = (Temp << 0x04) + c;
	}

	return Temp;
}

static unsigned char Bootloader_GetChar(void)
{
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_RXCIF_bm));
	return ((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA;
}

static void Bootloader_PutChar(const unsigned char Data)
{
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_DREIF_bm));
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA = Data;
}

static void Bootloader_PutString(const char* Message)
{
	while(*Message)
	{
		char i = *Message++;
		Bootloader_PutChar(i);
	}
}

void Bootloader_Init(void)
{
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
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLA = USART_RXCINTLVL_LO_gc;
	
	// Enable low level interrupts and place the interrupt vectors at the beginning of the bootloader section
	asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&PMIC.CTRL),
					"r" (PMIC_LOLVLEN_bm | PMIC_IVSEL_bm),
					"M" (CCP_IOREG_gc),
					"i" (&CCP)
					: "r16", "r30"
					);
	sei();
}

void Bootloader_Enter(void)
{
	uint8_t Active = 0x01;
	
	// Buffer for the current line
	char LineBuffer[16];
	
	// Byte index for the current line
	uint8_t LineIndex = 0x00;
	
	// Byte count per line
	uint8_t BytesPerLine = 0x00;
	uint16_t Checksum = 0x00;
	
	Bootloader_PutString("Enter bootloader...\n\r");
	
	__ParserState = BOOTLOADER_PARSER_STATE_WAIT;
	
	do
	{
		unsigned char c = Bootloader_GetChar();
		if(c)
		{
			switch(__ParserState)
			{
				case BOOTLOADER_PARSER_STATE_WAIT:
				{
					if(c == ':')
					{
						Bootloader_PutChar(XOFF);
						
						LineIndex = 0x00;
						Checksum = 0x00;
						
						__ParserState = BOOTLOADER_PARSER_GET_SIZE;
						
						Bootloader_PutChar(XON);
					}
					
					break;
				}
				case BOOTLOADER_PARSER_GET_SIZE:
				{
					LineBuffer[LineIndex++] = (uint8_t)c;
					if(LineIndex == 2)
					{
						Bootloader_PutChar(XOFF);
						
						LineIndex = 0;
						
						BytesPerLine = (uint8_t)Hex2Num(LineBuffer, 2);
						Checksum += BytesPerLine;
						
						__ParserState = BOOTLOADER_PARSER_GET_ADDRESS;
						
						Bootloader_PutChar(XON);
					}
					
					break;
				}
				case BOOTLOADER_PARSER_GET_ADDRESS:
				{
					break;
				}
				case BOOTLOADER_PARSER_STATE_EXIT:
				{
					Bootloader_PutString("Leave bootloader and jump to main...\n\r");
					Bootloader_Exit();
					
					Active = 0x00;
					
					break;
				}
				case BOOTLOADER_PARSER_STATE_ERROR:
				{
					
					break;
				}
			}
		}
	}
	while(Active);
}

void Bootloader_Exit(void)
{
	void (*Main)(void) = 0x0000;
	
	// Wait until last transmission finish
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_DREIF_bm));
	
	// Reset the I/O
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_TX);
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_RX);
	
	// Disable the USART
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLA &= ~USART_RXCINTLVL_LO_gc;
	
	// Disable interrupts
	cli();
	
	// Disable low level interrupts and place the interrupt vectors at the beginning of the application section
	asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&PMIC.CTRL),
					"r" (~(PMIC_LOLVLEN_bm | PMIC_IVSEL_bm)),
					"M" (CCP_IOREG_gc),
					"i" (&CCP)
					: "r16", "r30"
					);
	
	// Jump to main
	Main();
}