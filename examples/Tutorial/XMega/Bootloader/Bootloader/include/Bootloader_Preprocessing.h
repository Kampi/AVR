/*
 * Bootloader_Preprocessing.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration preprocessing for the AVR XMega bootloader.

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

/** @file Common/Services/Bootloader/Bootloader_Preprocessing.h
 *  @brief Configuration preprocessing for the AVR XMega bootloader.
 *
 *  This contains the prototypes and definitions for the bootloader.
 *
 *  @author Daniel Kampert
 */

#ifndef BOOTLOADER_PREPROCESSING_H_
#define BOOTLOADER_PREPROCESSING_H_

 #define CATENATE(Prefix, Name)				Prefix##Name
 #define FIRST_ARG(A, B)					A
 #define SECOND_ARG(A, B)					B

 #define MAKE_USART_NAME(Uart)				CATENATE(USART, Uart)
 #define MAKE_PORT_NAME(Port)				CATENATE(PORT, Port)
 #define USART_NAME(Name)					MAKE_USART_NAME(CATENATE(Name))
 #define PORT_NAME(Name)					MAKE_PORT_NAME(FIRST_ARG(Name))

 #if(BOOTLOADER_BAUD == 9600)
	 #define BOOTLOADER_BRREG_VALUE			12
	 #define BOOTLOADER_SCALE_VALUE			0
 #elif(BOOTLOADER_BAUD == 19200)
	 #define BOOTLOADER_BRREG_VALUE			11
	 #define BOOTLOADER_SCALE_VALUE			-1
 #elif(BOOTLOADER_BAUD == 38400)
	 #define BOOTLOADER_BRREG_VALUE			9
	 #define BOOTLOADER_SCALE_VALUE			-2
 #elif(BOOTLOADER_BAUD == 57600)
	 #define BOOTLOADER_BRREG_VALUE			75
	 #define BOOTLOADER_SCALE_VALUE			-6
 #elif(BOOTLOADER_BAUD == 115200)
	 #define BOOTLOADER_BRREG_VALUE			11
	 #define BOOTLOADER_SCALE_VALUE			-7
 #else
	 #error "Invalid baud rate for bootloader!"
 #endif

#endif /* BOOTLOADER_PREPROCESSING_H_ */