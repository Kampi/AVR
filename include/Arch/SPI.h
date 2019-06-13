/*
 * SPI.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Wrapper for AVR SPI driver.

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

/** @file Arch/SPI.h
 *  @brief Wrapper for AVR SPI driver.
 *		   NOTE: Use this for peripheral devices such as flash memories to support different interfaces and architectures.
 *
 *  @author Daniel Kampert
 */
 
#ifndef SPI_WRAPPER_H_
#define SPI_WRAPPER_H_

 #include "Definitions.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "XMega/ClockManagement/SysClock.h"

	 #if(SPI_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "XMega/USART/USART.h"

		 #define SPIM_INIT(Config)											USART_SPI_Init(Config)
		 #define SPIM_TRANSMIT(Interface, Data)								USART_SPI_SendData(Interface, Data)
		 #define SPIM_SET_CLOCK(Interface, SPIClock, Clock)					USART_SPI_SetClockRate(Interface, SPIClock, Clock, FALSE)
		 #define SPIM_GET_CLOCK(Interface, Clock)							USART_SPI_GetClockRate(Interface, Clock)
		 #define SPIM_CHIP_SELECT(Port, Pin)								USART_SPI_SelectDevice(Port, Pin)
		 #define SPIM_CHIP_DESELECT(Port, Pin)								USART_SPI_DeselectDevice(Port, Pin)
	 #elif(SPI_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "XMega/SPI/SPI.h"

		 #define SPIM_INIT(Config)											SPIM_Init(Config)
		 #define SPIM_TRANSMIT(Interface, Data)								SPIM_SendData(Interface, Data)
		 #define SPIM_SET_CLOCK(Interface, SPIClock, Clock)					SPIM_SetClock(Interface, SPIClock, Clock)
		 #define SPIM_GET_CLOCK(Interface, Clock)							SPIM_GetClock(Interface, Clock)
		 #define SPIM_CHIP_SELECT(Port, Pin)								SPIM_SelectDevice(Port, Pin)
		 #define SPIM_CHIP_DESELECT(Port, Pin)								SPIM_DeselectDevice(Port, Pin)
	 #endif

 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "AVR8/SPI/SPI.h"

     #define SPIM_INIT(Config)												SPIM_Init(Config)
     #define SPIM_TRANSMIT(Interface, Data)									SPIM_SendData(Data)
     #define SPIM_SET_CLOCK(Interface, SPIClock, Clock)						SPIM_SetClock(SPIClock, Clock)
     #define SPIM_GET_CLOCK(Interface, Clock)								SPIM_GetClock(Clock)
     #define SPIM_CHIP_SELECT(Port, Pin)									SPIM_SelectDevice(Port, Pin)
     #define SPIM_CHIP_DESELECT(Port, Pin)									SPIM_DeselectDevice(Port, Pin)
 #else
	  #error "Architecture not supported"
 #endif

#endif /* SPI_WRAPPER_H_ */