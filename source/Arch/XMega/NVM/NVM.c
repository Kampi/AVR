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

/** @brief	Flush the EEPROm buffer
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
	if(Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE))
	{
		return;
	}

	while(NVM.STATUS & NVM_NVMBUSY_bm);

	NVM_FlushEEBuffer();
	NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;

	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE) | (Offset & (EEPROM_PAGE_SIZE - 0x01));

	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	NVM.DATA0 = Data;

	NVM_ExecuteCommand(NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc);
}

void NVM_EEPROMWritePage(const uint8_t Page, const uint8_t Length, const uint8_t* Data)
{	
	// Check the address
	if(Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE))
	{
		return;
	}

	while(NVM.STATUS & NVM_NVMBUSY_bm);

	NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;

	NVM.ADDR1 = 0x00;
	NVM.ADDR2 = 0x00;

	for(uint8_t i = 0x00; i < EEPROM_PAGE_SIZE; i++) 
	{
		NVM.ADDR0 = i;

		if(i < Length)
		{
			NVM.DATA0 = *Data;
			++Data;
		}
		else
		{
			NVM.DATA0 = 0x00;
		}
	}
	
	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE);

	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	NVM_ExecuteCommand(NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc);
}

uint8_t NVM_EEPROMReadByte(const uint8_t Page, const uint8_t Offset)
{
	// Check the address
	if(Page > (EEPROM_SIZE / EEPROM_PAGE_SIZE))
	{
		return -1;
	}

	while(NVM.STATUS & NVM_NVMBUSY_bm);

	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE) | (Offset & (EEPROM_PAGE_SIZE - 0x01));

	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	NVM_ExecuteCommand(NVM_CMD_READ_EEPROM_gc);

	return NVM.DATA0;
}

void NVM_EEPROMErasePage(const uint8_t Page)
{
	while(NVM.STATUS & NVM_NVMBUSY_bm);

	uint16_t Address = (uint16_t)(Page * EEPROM_PAGE_SIZE);

	NVM.ADDR0 = Address & 0xFF;
	NVM.ADDR1 = (Address >> 0x08) & 0x1F;
	NVM.ADDR2 = 0x00;

	NVM_ExecuteCommand(NVM_CMD_ERASE_EEPROM_PAGE_gc);
}

void NVM_EEPROMErase(void)
{
	while(NVM.STATUS & NVM_NVMBUSY_bm);

	NVM_ExecuteCommand(NVM_CMD_ERASE_EEPROM_gc);
}

void NVM_FlashRangeCRC(const uint32_t Start, const uint32_t Length)
{
	// Load the start byte address in the NVM Address Register
	NVM.ADDR0 = Start & 0xFF;
	NVM.ADDR1 = (Start >> 0x08) & 0xFF;
	NVM.ADDR2 = (Start >> 0x10) & 0xFF;

	// Load the end byte address in NVM Data Register
	uint32_t EndAddr = Start + Length - 0x01;
	NVM.DATA0 = EndAddr & 0xFF;
	NVM.DATA1 = (EndAddr >> 0x08) & 0xFF;
	NVM.DATA2 = (EndAddr >> 0x010) & 0xFF;
	
	NVM_ExecuteCommand(NVM_CMD_FLASH_RANGE_CRC_gc);
}

const uint8_t NVM_ReadCalibrationByte(const uint8_t Index)
{
	uint8_t Result = 0x00;
	uint8_t Flags = CPU_IRQSave();
	
	NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
	
    asm volatile(	"lpm %0, Z" "\n\t"
					: "=r" (Result)
					: "z" ((uint16_t)Index)
				);
	
	NVM.CMD = NVM_CMD_NO_OPERATION_gc;
	
	CPU_IRQRestore(Flags);
	
	return Result;
}