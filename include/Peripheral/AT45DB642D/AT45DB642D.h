/*
 * AT45DB642D.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the AT45DB642 SPI flash memory.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Peripheral/AT45DB642D/AT45DB642D.h
 *  @brief Driver for the AT45DB642 SPI flash memory.
 *
 *  This file contains the  prototypes and definitions for the flash memory driver.
 *
 *  @author Daniel Kampert
 */

#ifndef AT45DB642D_H_
#define AT45DB642D_H_

 #include "Common/Common.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #if(AT45DB642D_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
	 #elif(AT45DB642D_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
	 #else
		 #error "Interface not supported for AT45DB642D!"
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/SPI/SPI.h"
 #else
	  #error "Architecture not supported for AT45DB642D!"
 #endif

 void AT45DB642D_Init(SPIM_Config_t* Config);

#endif /* AT45DB642D_H_ */