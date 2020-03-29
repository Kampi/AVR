/*
 * CRC.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega CRC module.

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

/** @file Arch/XMega/CRC/CRC.h
 *  @brief Driver for Atmel AVR XMega CRC module.
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega CRC driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef CRC_H_
#define CRC_H_

 #include "Common/Common.h"

 /** @brief	CRC preset options.
  */
 typedef enum
 {
	 CRC_CHECKSUM_ZERO = 0x02,		/**< Reset with zeros */ 
	 CRC_CHECKSUM_ONES = 0x03		/**< Reset with ones */ 
 } CRC_ResetOptions_t;

 /** @brief	CRC DMA channels.
  */
 typedef enum
 {
	 CRC_DMA_0 = 0x04,				/**< Use DMA channel 0 */ 
	 CRC_DMA_1 = 0x05,				/**< Use DMA channel 1 */ 
	 CRC_DMA_2 = 0x06,				/**< Use DMA channel 2 */ 
	 CRC_DMA_3 = 0x07,				/**< Use DMA channel 3 */ 
 } CRC_DMA_t;

 /** @brief	Memory segments for CRC calculation.
  */
 typedef enum
 {
	 CRC_SEGMENT_BOOT = 0x00,		/**< Bootloader section */ 
	 CRC_SEGMENT_APP = 0x01,		/**< Application section */ 
 } CRC_MemorySegment_t;

 /** @brief	Length of the CRC checksum..
  */
 typedef enum
 {
	 CRC_LENGTH_16 = 0x00,			/**< 16 bit CRC */ 
	 CRC_LENGTH_32 = 0x01			/**< 32 bit CRC */ 
 } CRC_ChecksumLength_t;

 /** @brief	Disable the CRC module.
  */
 static inline void CRC_Disable(void) __attribute__((always_inline));
 static inline void CRC_Disable(void)
 {
	 CRC.CTRL &= ~0x0F;
 }

 /** @brief					Reset the CRC module.
  *  @param Options			Reset mode for the module
  */
 static inline void CRC_Reset(const CRC_ResetOptions_t Options) __attribute__((always_inline));
 static inline void CRC_Reset(const CRC_ResetOptions_t Options)
 {
	 CRC.CTRL |= (CRC.CTRL & (~(0x03 << 0x06))) | (Options << 0x06);
 }

 /** @brief					Enable the DMA mode of the CRC module.
  *  @param Channel			DMA channel
  *  @param ChecksumLength	Length of the checksum
  */
 void CRC_EnableDMA(const CRC_DMA_t Channel, const CRC_ChecksumLength_t ChecksumLength);
 
 /** @brief		Disable the DMA mode of the CRC module.
  *  @return	CRC Checksum 
  */
 const uint32_t CRC_DisableDMA(void);

 /** @brief		Get the result from the CRC module.
  *  @return	CRC Checksum 
  */
 const uint32_t CRC_GetResult(void);

 /** @brief				Calculate a IEEE 802.3 CRC32 of a memory region.
  *  @param StartAddr	Memory Start address
  *  @param Length		Length of the region
  *  @return			Checksum of the memory region
  */
 const uint32_t CRC_MemoryRegion(const uint32_t StartAddr, const uint32_t Length);
 
 /** @brief			Calculate a IEEE 802.3 CRC32 of a memory segment.
  *  @param Segment	Memory Segment
  *  @return		Checksum of the memory region
  */
 const uint32_t CRC_MemorySegment(const CRC_MemorySegment_t Segment);
 
 /** @brief					Calculate a CCITT or IEEE 802.3 CRC of a data array.
  *  @param Data			Pointer to data
  *  @param Length			Length of the array
  *  @param ChecksumLength	Length of the checksum
  *  @return				Checksum
  */
 const uint32_t CRC_Data(const uint8_t* Data, const uint32_t Length, const CRC_ChecksumLength_t ChecksumLength);

 /** @brief					Calculate a custom CRC of a data array.
  *  @param Data			Pointer to data
  *  @param Length			Length of the array
  *  @param ChecksumLength	Length of the checksum
  *  @param Polynomial		CRC Polynomial	
  *  @return				Checksum
  */
 const uint32_t CRC_CustomData(const uint8_t* Data, const uint32_t Length, const CRC_ChecksumLength_t ChecksumLength, const uint32_t Polynomial);

#endif /* CRC_H_ */ 