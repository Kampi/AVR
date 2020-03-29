/*
 * NVM_Bootloader_XMega.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR NVM bootloader interface for XMega architecture.

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

/** @file Common/Bootloader/Arch/XMega/NVM_Bootloader_XMega.h
 *  @brief AVR NVM bootloader interface for XMega architecture.
 *
 *  This contains the prototypes and definitions for the AVR NVM bootloader interface for XMega architecture.
 *
 *  @author Daniel Kampert
 */

#ifndef NVM_BOOTLOADER_XMEGA_H_
#define NVM_BOOTLOADER_XMEGA_H_

 #include "Common/Common.h"

 /** @brief Lock the SPM command to prevent it´s usage.
  */
 void NVM_LockSPM(void);

 /** @brief Clear the application section of the flash memory.
  */
 void NVM_EraseApplication(void);

 /** @brief Erase the flash buffer.
  */
 void NVM_ClearFlashBuffer(void);

 /** @brief			Add one data word to the NVM page buffer.
  *  @param Offset	Word address
  *  @param Data	Data word
  */
 void NVM_LoadFlashBuffer(const uint16_t Offset, const uint16_t Data);
 
 /** @brief			Flush the page buffer and write the content into the flash memory.
  *  @param Page	Page address
  */
 void NVM_FlushFlash(const uint16_t Page);

#endif /* NVM_BOOTLOADER_XMEGA_H_ */