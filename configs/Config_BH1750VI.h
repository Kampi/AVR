/*
 * Config_DS1307.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Example configuration file for the Rohm BH1750VI I2C ambient light sensor.

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

/** @file configs/Config_DS1307.h
 *  @brief Example configuration file for the Rohm BH1750VI I2C ambient light sensor.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_BH1750VI_H_
#define CONFIG_BH1750VI_H_

 #include "Common/Common.h"

 /*
	 TWI configuration
 */
 #define TWI_BUFFER_SIZE						32							/**< Size of TWI buffer in bytes. */

 #define BH1750VI_INTERFACE						TWIC						/**< I2C interface used by the sensor. */
 #define BH1750VI_CLOCK							100000						/**< Clock rate used by the interface. */

 #define BH1750VI_USE_ADDR0													/**< Define this symbol to use address 0. */
 #undef BH1750VI_USE_ADDR1													/**< Define this symbol to use address 1. */
 #define BH1750VI_ADDR							PORTC, 7					/**< BH1750VI address pin. */

 #define F_CPU									2000000UL					/**< MCU clock frequency. */

#endif /* CONFIG_BH1750VI_H_ */