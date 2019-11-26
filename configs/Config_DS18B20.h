/*
 * Config_OneWire.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Maxim Integrated DS18B20 1-Wire temperature sensor.

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

/** @file configs/Config_OneWire.h
 *  @brief Configuration example for the Maxim Integrated DS18B20 1-Wire temperature sensor.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_1WIRE_H_
#define CONFIG_1WIRE_H_

 /*
	 Console service configuration
 */
 #define CONSOLE_STDIO								USARTE, 0				/**< USART interface for the console service. */

 /*
	 1-Wire configuration
 */
 #undef ONEWIRE_USE_EXT_PULL												/**< Use the DS18B20 in parasite mode. */
 #undef ONEWIRE_EXT_PULL													/**< Port where switchable external pull-up resistor is connected to. \n
																				 NOTE: You only need this when you define the symbol #ONEWIRE_USE_EXT_PULL. */
 #undef ONEWIRE_EXT_PULL_ACTIVE_LOW											/**< Define this when the external pull-up is active low. \n
																				 NOTE: You only need this when you define the symbol #ONEWIRE_USE_EXT_PULL. */
 #undef ONEWIRE_USE_OVERDRIVE												/**< Use the overdrive mode. */
 #define ONEWIRE_DQ									PORTE, 0				/**< DQ pin for the 1-Wire driver. */
 #define F_CPU										2000000UL				/**< CPU frequency. */

#endif /* CONFIG_1WIRE_H_ */