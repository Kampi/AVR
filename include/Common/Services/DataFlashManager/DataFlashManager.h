/*
 * DataFlashManager.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Data flash memory management service for SPI data flash devices.

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

/** @file Common/Services/DataFlashManager/DataFlashManager.h
 *  @brief Data flash memory management service for SPI data flash devices.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DATAFLASHMANAGER_H_
#define DATAFLASHMANAGER_H_

 #include "Definitions.h"
 #include "Arch/SPI.h"

 #if(BOARD == BOARD_NONE)

 #elif(BOARD == BOARD_AT90USBKEY)
	 #include "Peripheral/AT45DB642D/AT45DB642D.h"
	 #include "Common/Board/DataFlash/DataFlash_USBKey.h"
 #endif

 #ifndef DATAFLASH_ID
	 #error "Please define the symbol DATAFLASH_ID!"
 #endif
 
 #ifndef DATAFLASH_PAGE_SIZE
	 #error "No page size defined! Please define the symbol DATAFLASH_PAGE_SIZE!"
 #endif
 
 #ifndef DATAFLASH_COUNT
	 #define DATAFLASH_COUNT			0
 #endif

 //#include "Config_FlashManager.h"

 /*
	Function prototypes used by the data flash manager
 */
 /** @brief			Initialize the flash memory.
  *  @return		Product ID of the flash memory
  */
 extern uint16_t Flash_Init(void);

 /** @brief		Check if the flash memory is busy.
  *  @return	#TRUE when the flash is busy
  */
 extern Bool_t Flash_IsBusy(void);

 /** @brief			Flush the memory buffer and copy the content into the flash memory.
  *  @param Buffer	Buffer index
  *  @param Page	Target memory page
  */
 extern void Flash_Flush(const uint8_t Buffer, const uint16_t Page);

 /** @brief			Read a complete page direct from the flash memory.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @param Data	Pointer to buffer
  */
 extern void Flash_ReadPage(const uint16_t Page, const uint16_t Offset, uint8_t* Data);

 /** @brief			Write data to info the buffer of the flash memory.
  *  @param Buffer	Buffer index
  *  @param Offset	Buffer offset
  *  @param Length	Length of data
  *  @param Data	Pointer to data
  */
 extern void Flash_WriteBuffer(const uint8_t Buffer, uint16_t Offset, const uint16_t Length, uint8_t* Data);

 /** 
  * Flash status states
  */
 typedef enum
 {
	 FLASH_STATE_NO_ERROR = 0x00,								/**< No error */
	 FLASH_ERROR_INVALID_FLASH = 0x01,							/**< Invalid flash memory index */
	 FLASH_ERROR_INVALID_PAGE = 0x02,							/**< Invalid page address */
 } Flash_State_t;

 /** @brief		Initialize the flash memory manager service.
  *	 @return	Status state
  */
 Flash_State_t DataFlashManager_Init(void);

 /** @brief			Read a block of data from the memory.
  *	 @param Page	Memory page
  *  @param Offset	Memory offset
  *  @param Data	Pointer to buffer
  *	 @return		Status state
  */
 Flash_State_t DataFlashManager_ReadBlock(const uint16_t Page, const uint16_t Offset, uint8_t* Data);

 /** @brief			Write a block of data to the memory.
  *	 @param Page	Memory page
  *  @param Offset	Memory offset
  *  @param Data	Pointer to buffer
  *	 @return		Status state
  */
 Flash_State_t DataFlashManager_WriteBlock(const uint16_t Page, const uint16_t Offset, const uint8_t* Data);

#endif /* DATAFLASHMANAGER_H_ */