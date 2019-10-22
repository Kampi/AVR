/*
 * Config_PCA9685.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the NXP PCA9685 PWM controller.

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

/** @file Config/Config_PCA9685.h
 *  @brief Configuration example for the NXP PCA9685 I2C PWM controller.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_PCA9685_H_
#define CONFIG_PCA9685_H_

 #include "Common/Common.h"
 
 #define PCA9685_INTERFACE						TWIC					/**< I2C interface used by the PCA9685. */
 #define PCA9685_CLOCK							100000					/**< Clock used by the interface. */
 #define PCA9685_A0								0						/**< Voltage level of the address pin A0. */
 #define PCA9685_A1								0						/**< Voltage level of the address pin A1. */
 #define PCA9685_A2								0						/**< Voltage level of the address pin A2. */
 #define PCA9685_A3								0						/**< Voltage level of the address pin A3. */
 #define PCA9685_A4								0						/**< Voltage level of the address pin A4. */
 #define PCA9685_A5								0						/**< Voltage level of the address pin A5. */

 #define PCA9685_OE								PORTE, 0				/**< Pin for PCA9685 OE. */

#endif /* CONFIG_PCA9685_H_ */