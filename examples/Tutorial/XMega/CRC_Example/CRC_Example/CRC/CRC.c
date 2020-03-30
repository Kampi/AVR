/*
 * CRC.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: CRC driver for XMega.

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

#include "CRC.h"

void CRC_Reset(void)
{
	// Reset checksum with ones for CCITT and IEEE 802.3 compability
	CRC.CTRL |= CRC_RESET_RESET1_gc;
}

void CRC_Enable(void)
{
	CRC.CTRL |= CRC_CRC32_bm;
}

void CRC_Disable(void)
{
	CRC.CTRL &= ~CRC_SOURCE_gm;
}

uint32_t CRC_GetChecksum(void)
{
	uint32_t Checksum = 0x00;
	
	while((CRC.STATUS & CRC_BUSY_bm));

	if(CRC.CTRL & CRC_CRC32_bm)
	{
		Checksum = ((uint32_t)CRC.CHECKSUM0 & 0x000000FF);
		Checksum |= (((uint32_t)CRC.CHECKSUM1 << 0x08) & 0x0000FF00);
		Checksum |= (((uint32_t)CRC.CHECKSUM2 << 0x10) & 0x00FF0000);
		Checksum |= (((uint32_t)CRC.CHECKSUM3 << 0x18) & 0xFF000000);
	} 
	else 
	{
		Checksum = ((uint16_t)CRC.CHECKSUM0 & 0x00FF);
		Checksum |= (((uint16_t)CRC.CHECKSUM1 << 0x08) & 0xFF00);
	}
	
	return Checksum;
}

uint32_t CRC_GetResult(void)
{
	uint32_t Checksum = CRC_GetChecksum();

	// Disable the CRC module
	CRC_Disable();

	return Checksum;	
}

uint32_t CRC_Data(uint8_t* Data, uint32_t Length, uint8_t CRC32)
{
	CRC_Reset();
	
	if(CRC32)
	{
		CRC.CTRL |= CRC_CRC32_bm;
	}
	else
	{
		CRC.CTRL &= ~CRC_CRC32_bm;
	}
	
	// Set the source to I/O interface
	CRC.CTRL &= ~CRC_SOURCE_gm;
	CRC.CTRL |= CRC_SOURCE_IO_gc;
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		CRC.DATAIN = Data[i];
	}

	CRC.STATUS |= CRC_BUSY_bm;

	return CRC_GetResult();
}

uint32_t CRC_Flash(uint32_t Start, uint32_t Length)
{
	// Reset the CRC module
	CRC_Reset();
	
	// Enable 32 bit CRC
	CRC.CTRL |= CRC_CRC32_bm;
	
	// Set the source to flash memory
	CRC.CTRL &= ~CRC_SOURCE_gm;
	CRC.CTRL |= CRC_SOURCE_FLASH_gc;
	
	// Load the NVM command
	NVM.CMD = NVM_CMD_FLASH_RANGE_CRC_gc;
	
	NVM.ADDR0 = Start & 0xFF;
	NVM.ADDR1 = (Start >> 0x08) & 0xFF;
	NVM.ADDR2 = (Start >> 0x10) & 0xFF;
	
	NVM.DATA0 = (Start + Length - 0x01) & 0xFF;
	NVM.DATA1 = ((Start + Length - 0x01) >> 0x08) & 0xFF;
	NVM.DATA2 = ((Start + Length - 0x01) >> 0x10) & 0xFF;

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
	
	return CRC_GetResult();
}

 void CRC_DMAStart(uint8_t Channel, uint8_t CRC32)
 {
	 CRC_Reset();
	 
	if(CRC32)
	{
		CRC.CTRL |= CRC_CRC32_bm;
	}
	else
	{
		CRC.CTRL &= ~CRC_CRC32_bm;
	}
	
	// Set the source to a DMA channel
	CRC.CTRL &= ~CRC_SOURCE_gm;
	
	switch(Channel)
	{
		case 0:
		{
			CRC.CTRL |= CRC_SOURCE_DMAC0_gc;
			break;
		}
		case 1:
		{
			CRC.CTRL |= CRC_SOURCE_DMAC1_gc;
			break;
		}
		case 2:
		{
			CRC.CTRL |= CRC_SOURCE_DMAC2_gc;
			break;
		}
		case 3:
		{
			CRC.CTRL |= CRC_SOURCE_DMAC3_gc;
			break;
		}
		default:
		{
			break;
		}
	}
 }
 
 uint32_t CRC_DMAStop(void)
 {
	 return CRC_GetResult();
 }
