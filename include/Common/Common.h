/*
 * Common.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Common definitions for AVR library.

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

/** @file Common/Common.h
 *  @brief Common definitions for AVR library.
 *
 *  @author Daniel Kampert
 */

#ifndef COMMON_H_
#define COMMON_H_

 #ifndef CONFIG
	 #error "No configuration file specified!"
 #endif

 #include "Definitions.h"
 
 #include STR(CONFIG)
 
 #include "Common/types.h"
 #include "Common/Endianness.h"
 
 #include <avr/io.h>
 #include <avr/pgmspace.h>
 #include <avr/interrupt.h>

 #include <util/delay.h>
 #include <util/atomic.h>

 #include <stdio.h>
 #include <stdlib.h>

 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	 #include "Common/Board/MCU/ATmega32.h"
 #elif(MCU_NAME == MCU_NAME_AT90USB1287)
	 #include "Common/Board/MCU/AT90USB1287.h"
 #elif(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
	 #include "Common/Board/MCU/XMega256A3BU.h"
 #elif(MCU_NAME == MCU_NAME_ATXMEGA384C3)
	 #include "Common/Board/MCU/XMega384C3.h"
 #elif(MCU_NAME == MCU_NAME_ATTINY406)
	 #include "Common/Board/MCU/ATtiny406.h"
 #endif

#endif /* COMMON_H_ */