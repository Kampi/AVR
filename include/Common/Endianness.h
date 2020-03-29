/*
 * Endianess.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Endianess specific macros.

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

/** @file Common/Endianess.h
 *  @brief Endianess specific macros.
 *
 *  @author Daniel Kampert
 */

#ifndef ENDIANESS_H_
#define ENDIANESS_H_

 #include "Definitions.h"

 #if!(defined(MCU_BIG_ENDIAN) || defined(MCU_LITTLE_ENDIAN))
	 #error "MCU_BIG_ENDIAN or MCU_LITTLE_ENDIAN missing!"
 #endif

 /** 
  * Swap the endianess
  */
 #define ENDIAN_SWAP_16(x)            (uint16_t)((((x) & 0xFF00) >> 0x08) | (((x) & 0x00FF) << 0x08))
 
#endif /* ENDIANESS_H_ */