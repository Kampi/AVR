/*
 * USART_Bootloader_Preprocessing.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USART configuration preprocessing for the AVR XMega bootloader.

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

/** @file Common/Bootloader/Arch/XMega/USART_Bootloader_Preprocessing.h
 *  @brief Configuration preprocessing for the AVR XMega USART bootloader.
 *
 *  This contains the prototypes and definitions for the AVR XMega USART bootloader.
 *
 *  @author Daniel Kampert
 */

#ifndef USART_BOOTLOADER_PREPROCESSING_H_
#define USART_BOOTLOADER_PREPROCESSING_H_

 #define CATENATE(A, B)						A ## B
 #define CATENATE3(A, B, C)					A ## B ## C

 #define MAKE_USART_NAME(Usart)				CATENATE(USART, Usart)
 #define MAKE_PORT_NAME(Port)				CATENATE(PORT, Port)
 #define USART_NAME(Name)					MAKE_USART_NAME(CATENATE(Name))
 #define PORT_NAME(Name)					MAKE_PORT_NAME(_FIRST(Name))

 #define MAKE_ISR_NAME(Usart)				CATENATE3(USART, Usart, _RXC_vect)
 #define ISR_NAME(Name)						MAKE_ISR_NAME(CATENATE(Name))

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

#endif /* USART_BOOTLOADER_PREPROCESSING_H_ */