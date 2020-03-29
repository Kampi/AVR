/*
 * ConsoleService.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USART Console service for AVR microcontroller.

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

/** @file Services/ConsoleService/ConsoleService.h
 *  @brief USART Console service for AVR microcontroller.
 *
 *  This contains the prototypes and definitions for the AVR USART Console.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef CONSOLESERVICE_H_
#define CONSOLESERVICE_H_

 #include "Common/Common.h"

 #include <stdio.h>

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/USART/USART.h"
 #else
	 #error "Architecture not supported for console service!"
 #endif

 #if(!defined(CONSOLE_STDIO))
	 #error "'CONSOLE_STDIO' missing for console service!"
 #endif

 /** @brief	Initialize the console service.
  */
 void ConsoleService_Init(void);

#endif /* CONSOLESERVICE_H_ */