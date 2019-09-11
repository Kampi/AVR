/*
 * AT45DB642D.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AT45DB642D SPI flash memory.

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

/** @file Peripheral/AT45DB642D/AT45DB642D.h
 *  @brief Driver for AT45DB642D flash memory.
 *
 *  This file contains the prototypes and definitions for the flash memory driver.
 *
 *  @author Daniel Kampert
 */

#ifndef AT45DB642D_H_
#define AT45DB642D_H_

 #include "Common/Common.h"

 /** @ingroup Device-ID */
 /*\@{*/
	 #define DATAFLASH_ID								0x281F
 /*\@}*/

 /** 
  * Flash specifications
  */
 #define DATAFLASH_PAGES								8192
 #define DATAFLASH_PAGE_SIZE							1024

 /** 
  * Flash memory page size
  */
 typedef enum
 {
	 AT45DB642D_PAGE_SIZE_1024 = 0x00,								/**< 1024 byte page size */ 
	 AT45DB642D_PAGE_SIZE_1056 = 0x01								/**< 1056 byte page size */ 
 } AT45DB642D_PageSize_t;

 /** 
  * Internal flash memory buffer
  */
 typedef enum
 {
	 AT45DB642D_BUFFER_1 = 0x00,									/**< Flash buffer 1 */ 
	 AT45DB642D_BUFFER_2 = 0x01,									/**< Flash buffer 2 */ 
 } AT45DB642D_Buffer_t;
 
 void AT45DB642D_PowerDown(void);
 void AT45DB642D_Standby(void);
 void AT45DB642D_EnableSectorProtection(void);
 void AT45DB642D_DisableSectorProtection(void);
 
 void AT45DB642D_Flush(AT45DB642D_Buffer_t Buffer, uint16_t PageAddress);
 void AT45DB642D_WriteData(AT45DB642D_Buffer_t Buffer, uint16_t Address, uint8_t Count, uint8_t* Data);

#endif /* AT45DB642_H_ */