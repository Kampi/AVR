/*
 * Config_Nunchuk.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Nintendo Nunchuk.

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

/** @file configs/Config_Nunchuk.h
 *  @brief Configuration example for the Nintendo Nunchuk.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_NUNCHUK_H_
#define CONFIG_NUNCHUK_H_

 #include "Common/Common.h"

 /*
	 TWI configuration
 */
 #define TWI_BUFFER_SIZE						32							/**< Size of TWI buffer in bytes. */

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #define NUNCHUK_INTERFACE					TWIC						/**< I2C interface used by the Nunchuk. */
 #endif

 #define NUNCHUK_CLOCK							100000						/**< Clock used by the I2C interface. */

#endif /* CONFIG_NUNCHUK_H_ */