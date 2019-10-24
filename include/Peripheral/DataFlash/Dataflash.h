/*
 * Dataflash.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Data flash driver.

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

/** @file Peripheral/Dataflash/Dataflash.h
 *  @brief Data flash driver.
 *
 *  This file contains the prototypes and definitions for the data flash driver.
 *
 *  @author Daniel Kampert
 */

#ifndef DATAFLASH_H_
#define DATAFLASH_H_

 #include "Common/Common.h"

 #ifdef USE_AT45DB642D
	 #include "Peripheral/Dataflash/AT45DB642D/AT45DB642D.h"
 #else
	 #error "Unknown flash memory!"
 #endif

 /*
	Function prototypes used for the data flash interface. The flash driver has to implement this functions.
 */
 
 /** @brief			Initialize the flash memory.
  *  @param Config	Pointer to SPI master configuration object
  *					NOTE: Set to #NULL when the SPI is already initialized!
  *  @return		Product ID of the flash memory or
  */
  uint16_t Flash_Init(SPIM_Config_t* Config);

 /** @brief		Check if the flash memory is busy.
  *  @return	#TRUE when the flash is busy
  */
 Bool_t Flash_IsBusy(void);

 /** @brief			Flush the memory buffer and copy the content into the flash memory.
  *  @param Buffer	Buffer index
  *  @param Page	Target memory page
  */
 void Flash_Flush(const uint8_t Buffer, const uint16_t Page);

 /** @brief			Read a complete page direct from the flash memory.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @param Data	Pointer to buffer
  */
 void Flash_ReadPage(const uint16_t Page, const uint16_t Offset, uint8_t* Data);

 /** @brief			Write data to info the buffer of the flash memory.
  *  @param Buffer	Buffer index
  *  @param Offset	Buffer offset
  *  @param Length	Length of data
  *  @param Data	Pointer to data
  */
 void Flash_WriteBuffer(const uint8_t Buffer, uint16_t Offset, const uint16_t Length, uint8_t* Data);

 /** 
  * Flash status states
  */
 typedef enum
 {
	 FLASH_STATE_NO_ERROR = 0x00,								/**< No error */
	 FLASH_ERROR_INVALID_FLASH = 0x01,							/**< Invalid flash memory index */
	 FLASH_ERROR_INVALID_PAGE = 0x02,							/**< Invalid page address */
 } Flash_State_t;

#endif /* DATAFLASH_H_ */