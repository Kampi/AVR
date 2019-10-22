/*
 * Config_ADXL345.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Analog Devices ADXL345 I2C accelerometer.

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

/** @file Config/Config_ADXL345.h
 *  @brief Configuration example for the Analog Devices ADXL345 I2C accelerometer.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_ADXL345_H_
#define CONFIG_ADXL345_H_

 #include "Common/Common.h"
 
 #define ADXL345_INTERFACE_TYPE					INTERFACE_I2C			/**< Interface type for the ADXL345. */
 #define ADXL345_INTERFACE						&TWIC					/**< I2C interface for the ADXL345. */
 #define ADXL345_CLOCK							100000					/**< I2C clock rate for the ADXL345. */
 
 /*
	 Not supported yet
 */
 //#define ADXL345_SS_PORT						&PORTE					/**< Chip select port for the ADXL345. */
 //#define ADXL345_SS_PIN						4						/**< Chip select pin for the ADXL345. */
 
 #define ADXL345_CALIBRATION_LENGTH				100						/**< Number of samples for the self test and the offset calibration. */

#endif /* CONFIG_ADXL345_H_ */