/*
 * Bootloader.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR bootloader definitions.

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

/** @file Common/Bootloader/Bootloader.h
 *  @brief AVR bootloader definitions.
 *
 *  @author Daniel Kampert
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
 
 #include "Common/Common.h"
 
 #include "Config_Bootloader.h"
 
 #if(BOOTLOADER_INTERFACE_TYPE == INTERFACE_USART)
	 #include "Interface/USART_Bootloader.h"
	 #include "Interface/NVM_Bootloader.h"
 #else
	 #error "Bootloader interface not supported!"
 #endif 

 #if(BOOTLOADER_FILE_FORMAT == HEX_FORMAT_INTEL)
	 #include "Parser/IntelHexParser.h"
 #else
	 #error "File format not supported by bootloader!"
 #endif

 /*
	Function prototypes used by the bootloader.
 */

 /** @brief	Initialize all necessary peripherals and the bootloader.
  */
 void Bootloader_Init(void);
 
 /** @brief		Enter the bootloader mode and process the input.
  *  @return	#true when successfully
  */
 bool Bootloader_Enter(void);
 
 /** @brief	Leave the bootloader and jump to the main application.
  */
 void Bootloader_Exit(void);

#endif /* BOOTLOADER_H_ */