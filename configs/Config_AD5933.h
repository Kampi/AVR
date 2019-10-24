/*
 * Config_AD5933.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Analog Devices AD5933 I2C impedance converter.

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

/** @file Config_AD5933.h
 *  @brief Configuration example for the Analog Devices AD5933 I2C impedance converter.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_AD5933_H_
#define CONFIG_AD5933_H_

 #include "Common/Common.h"
 
 #define AD5933_INTERFACE						TWIC 					/**< I2C interface used by the AD5933. */
 #define AD5933_CLOCK							100000					/**< Clock rate used by the interface. */

 #define AD5933_CALIBRATIONIMPEDANCE			1000					/**< Calibration impedance for the AD5933. */
 #define AD5933_SWEEP_LENGTH					100						/**< Length of frequency sweep. */

#endif /* CONFIG_AD5933_H_ */