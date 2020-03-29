/*
 * NVM.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega NVM controller.

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

#ifndef NVM_H_
#define NVM_H_

 #include <avr/io.h>
 
 /*
	 Fuse and lock bit functions
 */
 
 /** @brief			Read one fuse byte.
  *  @param Address	Fuse byte address
  *  @return		Fuse byte
  */
 uint8_t NVM_FuseRead(const uint8_t Address); 
  
 /*
	 EEPROM functions
 */
 
 /** @brief			Write one byte into the EEPROM.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @param Data	Data byte
  */
 void NVM_EEPROMWriteByte(const uint8_t Page, const uint8_t Offset, const uint8_t Data);

 /** @brief			Write a complete EEPROM page into the EEPROM.
  *  @param Page	Page address
  *  @param Data	Pointer to data buffer
  */
 void NVM_EEPROMWritePage(const uint8_t Page, const uint8_t* Data);

 /** @brief	Erase the EEPROM.
  */
 void NVM_EEPROMErase(void);

 /** @brief			Read one byte from the EEPROM.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @return		Data byte
  */
 uint8_t NVM_EEPROMReadByte(const uint8_t Page, const uint8_t Offset);

 /** @brief			Read a complete EEPROM page from the EEPROM.
  *  @param Page	Page address
  *  @param Data	Pointer to data buffer
  */
 void NVM_EEPROReadPage(const uint8_t Page, uint8_t* Data);

 /*
	 Flash functions
 */

 /** @brief			Write one word to the flash buffer.
  *  @param Offset	page offset
  *  @param Data	Data word
  */
 void NVM_FlashWriteWord(const uint16_t Offset, const uint16_t Data);

 /** @brief			Copy a complete page into the flash buffer.
  *  @param Data	Pointer to data buffer
  */
 void NVM_FlashWritePage(const uint16_t* Data);

 /** @brief			Flush the flash buffer and write the content to the given page in flash memory.
  *  @param Page	Page address
  */
 void NVM_FlushFlash(const uint16_t Page);

 /*
	 User Signature Row functions
 */

 /** @brief	Clear the user signature page.
  */
 void NVM_EraseUserSignature(void);
 
 /** @brief	Flush the flash buffer and write the data to the user signature flash page.
  */
 void NVM_FlushUserSignature(void);

 /** @brief			Read one word from the user signature row.
  *  @param Address	Word address
  *  @return		Data word
  */
 uint16_t NVM_UserSignatureReadWord(const uint16_t Address);
 
 /** @brief		Read one page from the user signature row.
  *  @return	Pointer to output buffer word
  */
 void NVM_UserSignatureReadPage(const uint16_t* Data);

 /*
	 Miscellaneous functions
 */

 /** @brief	Wait for NVM controller to becomes ready.
  */
 void NVM_WaitBusy(void);

#endif /* NVM_H_ */