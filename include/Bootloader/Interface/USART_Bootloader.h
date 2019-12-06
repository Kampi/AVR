/*
 * USART_Bootloader.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USART bootloader interface definitions.

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

/** @file Common/Bootloader/Interface/USART_Bootloader.h
 *  @brief USART bootloader interface definitions.
 *
 *  @author Daniel Kampert
 */

#ifndef USART_BOOTLOADER_H_
#define USART_BOOTLOADER_H_
 
 #include "Common/Common.h"
 
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "../Arch/XMega/USART_Bootloader_XMega.h"
 #else
	 #error "MCU architecture not supported by bootloader!"
 #endif
 
 #define XON                     0x11
 #define XOFF                    0x13

#endif /* USART_BOOTLOADER_H_ */