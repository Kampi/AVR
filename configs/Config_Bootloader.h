/*
 * Config_Bootloader.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the AVR bootloader.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file configs/Config_Bootloader.h
 *  @brief Configuration file for the AVR bootloader.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_BOOTLOADER_H_
#define CONFIG_BOOTLOADER_H_
 
 #include "Common/Common.h"
 
 #define BOOTLOADER_INTERFACE_TYPE				INTERFACE_USART
 #define BOOTLOADER_INTERFACE					C, 0					/**< USART interface used by the bootloader. */
 #define BOOTLOADER_BAUD						19200					/**< USART baud rate used by the bootloader. */

 #define BOOTLOADER_FILE_FORMAT					HEX_FORMAT_INTEL		/**< Use the Intel Hex-Format as input file. */

#endif /* CONFIG_BOOTLOADER_H_ */