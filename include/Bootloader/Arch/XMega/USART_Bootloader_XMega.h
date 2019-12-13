/*
 * USART_Bootloader_XMega.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR USART bootloader interface for XMega architecture.

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

/** @file Common/Bootloader/Arch/XMega/USART_Bootloader_XMega.h
 *  @brief AVR USART bootloader interface for XMega architecture.
 *
 *  This contains the prototypes and definitions for the AVR USART bootloader interface for XMega architecture.
 *
 *  @author Daniel Kampert
 */

#ifndef USART_BOOTLOADER_XMEGA_H_
#define USART_BOOTLOADER_XMEGA_H_

 #include "Common/Common.h"
 #include "USART_Bootloader_Preprocessing.h"
 
 #define BOOTLOADER_TX							3						/**< USART Tx pin used by the bootloader. */
 #define BOOTLOADER_RX							2						/**< USART Rx pin used by the bootloader. */

#endif /* USART_BOOTLOADER_XMEGA_H_ */