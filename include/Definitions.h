/*
 * Definitions.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Board and MCU definitions for the AVR library.

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

/** @file Definitions.h
 *  @brief Board and MCU definitions for the AVR library.
 *
 *  @author Daniel Kampert
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

 /** @defgroup Interfaces
  *  @ingroup Configuration
  *  Interfaces for peripheral devices such as LCDs.
  *  @{
  */
	#define INTERFACE_UNKNOWN									0		/**< Unknown interface */
	#define INTERFACE_USART_SPI									1		/**< USART/SPI interface */
	#define INTERFACE_SPI										2		/**< SPI interface */
	#define INTERFACE_I2C										3		/**< I2C interface */
	#define INTERFACE_USART										4		/**< USART interface */
	#define INTERFACE_USB										5		/**< USB interface */
	#define INTERFACE_GPIO										6		/**< GPIO interface */
 /** @} */ // end of Interfaces

 /** @defgroup Board
  *  @ingroup Configuration
  *  Board definitions for the AVR library.
  *  @{
  */
	#define BOARD_NONE											0		/**< No board */
	#define BOARD_XMEGA256A3_XPLAINED							1		/**< Board XMega256A3BU XPlained */
	#define BOARD_XMEGA384C3_XPLAINED							2		/**< Board XMega384C3 XPlained */
	#define BOARD_CAN_NODE										3		/**< My private CAN Node */
	#define BOARD_RN_CONTROL									4		/**< Board RN control */
	#define BOARD_AT90USBKEY									5		/**< Board AT90USBKey */
	#define BOARD_CUSTOM										999		/**< Custom board */
 /** @} */ // end of Board

 /** @defgroup MCU
  *  @ingroup Configuration
  *  MCU definitions for the AVR library.
  *  @{
  */
	#define MCU_NAME_UNKNOWN									0		/**< Unknown microcontroller */
	#define MCU_NAME_ATXMEGA256A3BU								1		/**< XMega256A3BU microcontroller */
	#define MCU_NAME_ATXMEGA384C3								2		/**< XMega384C3 microcontroller */
	#define MCU_NAME_AT90USB1287								3		/**< AT90USB1287 microcontroller  */
	#define MCU_NAME_ATMEGA32									4		/**< ATmega32 microcontroller */
	#define MCU_NAME_ATMEGA328P									5		/**< ATmega328P microcontroller */
	#define MCU_NAME_ATTINY406									6		/**< ATtiny406 microcontroller */
 /** @} */ // end of MCU

 /** @defgroup Architecture
  *  @ingroup Configuration
  *  Architecture definitions for the AVR library.
  *  @{
  */
	#define MCU_ARCH_UNKNOWN									0		/**< Unknown architecture */
	#define MCU_ARCH_XMEGA										1		/**< AVR XMega architecture */
	#define MCU_ARCH_AVR8										2		/**< AVR8 architecture */
	#define MCU_ARCH_UC3										3		/**< AVR UC3 architecture */
 /** @} */ // end of Architecture

 /** @defgroup Family
  *  @ingroup Configuration
  *  AVR8 family definitions for the AVR library.
  *  @{
  */
	#define MCU_FAMILY_UNKNOWN									0		/**< Unknown MCU family */
	#define MCU_FAMILY_MEGA										1		/**< AVR Mega MCU family */
	#define MCU_FAMILY_TINY0									2		/**< AVR Tiny0 MCU family */
 /** @} */ // end of Family

 /** @defgroup Flashmemory
  *  @ingroup Configuration
  *  Flash memory definitions for the AVR library.
  *  @{
  */
	#define FLASH_UNKNOWN										0		/**< Unknown flash */
	#define FLASH_AT45DB642D									1		/**< Atmel AT45DB642D flash memory */ 
 /** @} */ // end of Flashmemory

 /** @defgroup HexFormats
  *  @ingroup Configuration
  *  Hex-File format definitions for the AVR bootloader.
  *  @{
  */
	#define HEX_FORMAT_UNKNOWN									0		/**< Unknown Hex-File format */
	#define HEX_FORMAT_INTEL									1		/**< Intel Hex-File format */ 
 /** @} */ // end of HexFormats

 #include "Doxygen.h"
 #include "Macros.h"

#endif /* DEFINITIONS_H_ */