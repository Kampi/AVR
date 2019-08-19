/*
 * CRC.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega CRC.

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

/** @file Arch/XMega/CRC/CRC.c
 *  @brief Driver for XMega CRC.
 *
 *  This file contains the implementation of the XMega CRC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/CRC/CRC.h"
#include "Arch/XMega/NVM/NVM.h"

/** @brief	Read the result from the CRC module.
 *  @return	CRC Checksum 
 */
static uint32_t CRC_GetChecksum(void)
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

/** @brief				Write a new polynomial to the checksum registers.
 *  @param Polynomial	CRC polynomial 
 */
static void CRC_WritePolynomial(const uint32_t Polynomial)
{
	CRC_Disable();
	
	CRC.CHECKSUM0 = Polynomial & 0x000000FF;
	CRC.CHECKSUM1 = (Polynomial & 0x0000FF00) >> 0x08;
	CRC.CHECKSUM2 = (Polynomial & 0x00FF0000) >> 0x10;
	CRC.CHECKSUM3 = (Polynomial & 0xFF000000) >> 0x18;
}

void CRC_EnableDMA(const CRC_DMA_t Channel, const CRC_ChecksumLength_t ChecksumLength)
{
	CRC_Reset(CRC_CHECKSUM_ONES);
	
	// Clear the busy flag
	CRC.STATUS |= CRC_BUSY_bm;
	
	// Set the length of the checksum
	if(ChecksumLength == CRC_LENGTH_32)
	{
		CRC.CTRL |= CRC_CRC32_bm;
	}
	else
	{
		CRC.CTRL &= ~CRC_CRC32_bm;
	}
	
	// Clear the source
	CRC.CTRL &= ~0x0F;
	
	if(Channel == CRC_DMA_0) 
	{
		CRC.CTRL |= CRC_SOURCE_DMAC0_gc;
	}
	else if(Channel == CRC_DMA_1)
	{
		CRC.CTRL |= CRC_SOURCE_DMAC1_gc;
	}
	
	#if defined(CRC_SOURCE_DMAC2_gc)
		else if(Channel == CRC_DMA_2)
		{
			CRC.CTRL |= CRC_SOURCE_DMAC2_gc;
		}
	#endif
	
	#if defined(CRC_SOURCE_DMAC3_gc)
		else if(Channel == CRC_DMA_3)
		{
			CRC.CTRL |= CRC_SOURCE_DMAC3_gc;
		}
	#endif
}

const uint32_t CRC_DisableDMA(void)
{
	return CRC_GetResult();
}

const uint32_t CRC_GetResult(void)
{
	uint32_t Checksum = CRC_GetChecksum();

	CRC_Disable();

	return Checksum;
}

const uint32_t CRC_MemoryRegion(const uint32_t StartAddr, const uint32_t Length)
{	
	// Reset the module
	CRC_Reset(CRC_CHECKSUM_ONES);
	
	// Clear the source
	CRC.CTRL &= ~0x0F;
	
	// Wait until the module has finished
	while(CRC.STATUS & CRC_BUSY_bm);
	
	// Enable 32-bit mode
	CRC.CTRL |= CRC_CRC32_bm;
	
	// Set the source to flash memory
	CRC.CTRL |= CRC_SOURCE_FLASH_gc;
	
	NVM_FlashRangeCRC(StartAddr, Length);
	
	return CRC_GetResult();
}

const uint32_t CRC_MemorySegment(const CRC_MemorySegment_t Segment)
{
	// Reset the module
	CRC_Reset(CRC_CHECKSUM_ONES);
	
	// Wait until the module has finished
	while(CRC.STATUS & CRC_BUSY_bm);
	
	// Enable 32-bit mode
	CRC.CTRL |= CRC_CRC32_bm;

	// Clear the source
	CRC.CTRL &= ~0x0F;

	// Set the source to boot region of flash memory
	CRC.CTRL |= CRC_SOURCE_FLASH_gc;

	if(Segment == CRC_SEGMENT_BOOT)
	{
		NVM_ExecuteCommand(NVM_CMD_BOOT_CRC_gc);
	}
	else
	{
		NVM_ExecuteCommand(NVM_CMD_APP_CRC_gc);
	}
	
	return CRC_GetResult();
}

const uint32_t CRC_Data(const uint8_t* Data, const uint32_t Length, const CRC_ChecksumLength_t ChecksumLength)
{	
	// Reset the module
	CRC_Reset(CRC_CHECKSUM_ONES);
	
	// Clear the busy flag
	CRC.STATUS |= CRC_BUSY_bm;
	
	if(ChecksumLength == CRC_LENGTH_32)
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

const uint32_t CRC_CustomData(const uint8_t* Data, const uint32_t Length, const CRC_ChecksumLength_t ChecksumLength, const uint32_t Polynomial)
{
	// Reset the module
	CRC_Reset(CRC_CHECKSUM_ONES);
	
	CRC_WritePolynomial(Polynomial);
	
	// Clear the busy flag
	CRC.STATUS |= CRC_BUSY_bm;
	
	if(ChecksumLength == CRC_LENGTH_32)
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