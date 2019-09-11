/*
 * Config_BMP180.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the BMP180 I2C pressure sensor.

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

/** @file Config/Config_BMP180.h
 *  @brief Configuration file for the Bosch BMP180 i2c pressure sensor.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_BMP180_H_
#define CONFIG_BMP180_H_

 #include "Common/Common.h"
 
 #define BMP180_INTERFACE						TWIC 					/**< I2C interface for the BMP180. */
 #define BMP180_CLOCK							100000					/**< I2C clock rate for the BMP180. */

#endif /* CONFIG_BMP180_H_ */