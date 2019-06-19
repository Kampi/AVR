/*
 * Bootloader.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR XMega bootloader.

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

/** @file Bootloader/Bootloader.c
 *  @brief AVR XMega bootloader.
 *
 *  This file contains the implementation of the bootloader.
 *
 *  @author Daniel Kampert
 */

#include "Bootloader.h"

static uint8_t Bootloader_GetChar(void)
{
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_RXCIF_bm));
	return ((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA;
}

static void Bootloader_PutChar(const uint8_t Data)
{
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->DATA = Data;
	while(!(((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->STATUS & USART_DREIF_bm));
}

void Bootloader_Init(void)
{
	// Enable the default clock
	SysClock_SetClockSource(CLOCK_SOURCE_INT2MHZ);
	 uint8_t Flags = CPU_IRQSave();

	asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLK.CTRL), 
						"r" (Source), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
				
	 CPU_IRQRestore(Flags);

	// Enable the USART interface
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRSET = (0x01 << BOOTLOADER_TX);
	((PORT_t*)(&PORT_NAME(BOOTLOADER_INTERFACE)))->DIRCLR = (0x01 << BOOTLOADER_RX);
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLA = BOOTLOADER_BRREG_VALUE;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->BAUDCTRLB = (BOOTLOADER_SCALE_VALUE << USART_BSCALE_gp) & USART_BSCALE_gm;
	((USART_t*)(&USART_NAME(BOOTLOADER_INTERFACE)))->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	
	Bootloader_PutChar('A');
}