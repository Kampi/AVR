/*
 * DataFlash_USBKey.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Data flash driver for the AT90USBKey.

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

/** @file Common/Board/DataFlash/DataFlash_USBKey.h
 *  @brief Data flash driver for the AT90USBKey.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DATAFLASH_USBKEY_H_
#define DATAFLASH_USBKEY_H_

 #define DATAFLASH_COUNT						2
 
 /*
	 Flash memory interface connections
 */
 #define FLASH1_SS_PORT							&PORTE
 #define FLASH1_SS_PIN							0
 #define FLASH2_SS_PORT							&PORTE
 #define FLASH2_SS_PIN							1

 /** @brief	Initialize the chip select pins for the AT90USBKey flash memories.
  */
 static inline void DataFlash_InitCS(void) __attribute__ ((always_inline));
 static inline void DataFlash_InitCS(void)
 {
	// Set Flash SS as Output in High State
	GPIO_SetDirection(FLASH1_SS_PORT, FLASH1_SS_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(FLASH1_SS_PORT, FLASH1_SS_PIN);
	
	// Set Flash SS as Output in High State
	GPIO_SetDirection(FLASH2_SS_PORT, FLASH2_SS_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(FLASH2_SS_PORT, FLASH2_SS_PIN);
 }

 /** @brief			Select a flash memory.
  *  @param Index	Memory index
  */
 static inline void DataFlash_Select(const uint8_t Index) __attribute__ ((always_inline));
 static inline void DataFlash_Select(const uint8_t Index)
 {
	 if(Index == 0)
	 {
		 GPIO_Clear(FLASH1_SS_PORT, FLASH1_SS_PIN);
	 }
	 else if(Index == 1)
	 {
		 GPIO_Clear(FLASH2_SS_PORT, FLASH2_SS_PIN);
	 }
 }

 /** @brief			Deselect a flash memory.
  *  @param Index	Memory index
  */
 static inline void DataFlash_Deselect(const uint8_t Index) __attribute__ ((always_inline));
 static inline void DataFlash_Deselect(const uint8_t Index)
 {
	 if(Index == 0)
	 {
		 GPIO_Set(FLASH1_SS_PORT, FLASH1_SS_PIN);
	 }
	 else if(Index == 1)
	 {
		 GPIO_Set(FLASH2_SS_PORT, FLASH2_SS_PIN);
	 }
 }

#endif /* DATAFLASH_USBKEY_H_ */