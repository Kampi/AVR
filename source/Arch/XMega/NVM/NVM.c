/*
 * NVM.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega NVM.

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

/** @file Arch/XMega/NVM/NVM.c
 *  @brief Driver for XMega NVM.
 *
 *  This file contains the implementation of the XMega NVM driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/CPU/CPU.h"
#include "Arch/XMega/NVM/NVM.h"

/** @brief			Execute a NVM command.
 *  @param Command	NVM command
 */
static inline void NVM_ExecuteCommand(const uint8_t Command)
{ 
	NVM.CMD = Command;

	asm volatile(	"movw r30,  %0"	    "\n\t"
					"ldi  r16,  %2"	    "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::
					"r" (&NVM.CTRLA),
					"r" (NVM_CMDEX_bm),
					"M" (CCP_IOREG_gc),
					"i" (&CCP)
					: "r16", "r30", "r31"
	);
 
	NVM.CMD = OldCmd;
}

/** @brief	Flush the EEPROM buffer
 */
static void NVM_FlushEEBuffer(void)
{
	while(NVM.STATUS & NVM_NVMBUSY_bm);

	// Clear the EEPROM page buffer	
	if(NVM.STATUS & NVM_EELOAD_bm) 
	{
		NVM_ExecuteCommand(NVM_CMD_ERASE_EEPROM_BUFFER_gc);
	}
}

void NVM_EEPROMWriteByte(const uint8_t Page, const uint8_t Offset, const uint8_t Data)
{
	// Check the address
	if((Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE)) || (Offset > (EEPROM_PAGE_SIZE - 0x01)))
	{
		return;
	}

	// Wait for the NVM controller
	NVM_WaitBusy();

	// Load the NVM command
	NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;

	// Get the EEPROM address
	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE) | (Offset & (EEPROM_PAGE_SIZE - 0x01));

	// Set the EEPROM address
	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	// Copy the data into the data register
	NVM.DATA0 = Data;

	// Execute the write command
	NVM_ExecuteCommand(NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc);
}

void NVM_EEPROMWritePage(const uint8_t Page, const uint8_t* Data)
{
	// Check the address
	if(Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE))
	{
		return;
	}

	// Wait for the NVM controller
	NVM_WaitBusy();

	// Load the NVM command
	NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;

	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE);

	// Set the EEPROM address
	uint8_t Address0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	// Copy the data into the data register
	for(uint8_t i = 0x00; i < EEPROM_PAGE_SIZE; i++)
	{
		NVM.ADDR0 = Address0 | i;
		NVM.DATA0 = *Data++;
	}

	// Execute the write command
	NVM_ExecuteCommand(NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc);
}

void NVM_EEPROMErase(void)
{
	// Wait for the NVM controller
	NVM_WaitBusy();

	NVM_ExecuteCommand(NVM_CMD_ERASE_EEPROM_gc);

	// Wait for the NVM controller
	NVM_WaitBusy();
}

uint8_t NVM_EEPROMReadByte(const uint8_t Page, const uint8_t Offset)
{
	// Check the address
	if((Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE)) || (Offset > (EEPROM_PAGE_SIZE - 0x01)))
	{
		return -1;
	}

	// Wait for the NVM controller
	NVM_WaitBusy();

	// Get the EEPROM address
	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE) | (Offset & (EEPROM_PAGE_SIZE - 0x01));

	// Set the EEPROM address
	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	// Execute the read command
	NVM_ExecuteCommand(NVM_CMD_READ_EEPROM_gc);

	// Get the data byte
	return NVM.DATA0;
}

void NVM_EEPROReadPage(const uint8_t Page, uint8_t* Data)
{
	// Check the address
	if(Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE))
	{
		return;
	}

	// Wait for the NVM controller
	NVM_WaitBusy();

	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE);

	// Set the EEPROM address
	uint8_t Address0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	// Read the data
	for(uint8_t i = 0x00; i < EEPROM_PAGE_SIZE; i++)
	{
		NVM.ADDR0 = Address0 | i;

		// Execute the write command
		NVM_ExecuteCommand(NVM_CMD_READ_EEPROM_gc);

		*Data++ = NVM.DATA0;
	}
}