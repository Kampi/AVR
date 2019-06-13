/*
 * Definitions.h
 *
 *  Copyright (C) Daniel Kampert, 2018
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

 #include "Doxygen.h"

 /** @defgroup Interfaces
  *  @ingroup Configuration
  *  Interfaces for peripheral devices such as LCDs
  *  @{
  */
	#define INTERFACE_USART_SPI									0		/**< USART/SPI interface */ 
	#define INTERFACE_SPI										1		/**< SPI interface */
	#define INTERFACE_I2C										2		/**< I2C interface */
 /** @} */ // end of Interfaces

 /** @defgroup Board
  *  @ingroup Configuration
  *  Board definitions for the AVR library
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
  *  MCU definitions for the AVR library
  *  @{
  */
	#define MCU_NAME_ATXMEGA256A3BU								1		/**< XMega256A3BU micro controller */
	#define MCU_NAME_ATXMEGA384C3								2		/**< XMega384C3 micro controller */
	#define MCU_NAME_AT90USB1287								3		/**< AT90USB1287 micro controller  */ 
	#define MCU_NAME_ATMEGA32									4		/**< ATmega32 micro controller */ 
 /** @} */ // end of MCU

 /** @defgroup Architecture
  *  @ingroup Configuration
  *  Architecture definitions for the AVR library
  *  @{
  */
	#define MCU_ARCH_XMEGA										0		/**< AVR XMega architecture */ 
	#define MCU_ARCH_AVR8										1		/**< AVR8 architecture */ 
	#define MCU_ARCH_UC3										2		/**< AVR UC3 architecture */ 
 /** @} */ // end of Architecture

#endif /* DEFINITIONS_H_ */