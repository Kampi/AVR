/*
 * Config_Bootloader.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Config/Config_Bootloader.h
 *  @brief Configuration file for the AVR bootloader.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_BOOTLOADER_H_
#define CONFIG_BOOTLOADER_H_
 
 #define BOOTLOADER_INTERFACE					E,0						/**< USART interface used by the bootloader. */
 #define BOOTLOADER_BAUD						9600					/**< USART baudrate used by the bootloader. */
 #define BOOTLOADER_TX							3						/**< USART Tx pin used by the bootloader. */
 #define BOOTLOADER_RX							2						/**< USART Rx pin used by the bootloader. */

#endif /* CONFIG_BOOTLOADER_H_ */