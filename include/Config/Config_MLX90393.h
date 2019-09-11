/*
 * Config_MLX90393.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the MLX90393 magnetic sensor.

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

/** @file Config/Config_MLX90399.h
 *  @brief Configuration file for the Melexis MLX90399 I2C magnetic sensor.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_MLX90393_H_
#define CONFIG_MLX90393_H_

 #include "Common/Common.h"
 
 #define MLX90393_INTERFACE_TYPE				INTERFACE_I2C			/**< Interface type for the MLX90393. */
 #define MLX90393_INTERFACE						&TWIC					/**< I2C interface for the MLX90393. */
 #define MLX90393_CLOCK							100000					/**< I2C clock rate for the MLX90393. */

 #define MLX90393_DRDY_PORT						&PORTC					/**< Port for MLX90393 DRDY pin. */
 #define MLX90393_DRDY_PIN						3						/**< Pin for MLX90393 DRDY pin. */

 #define MLX90393_TRG_PORT						&PORTC					/**< Port for MLX90393 TRG pin. */
 #define MLX90393_TRG_PIN						3						/**< Pin for MLX90393 TRG pin. */

 #define MLX90393_EE_I2C_ADDR					0x03					/**< EE_I2C_ADDR parameter of the device. */
 #define MLX90393_A0							0						/**< Voltage level of the address pin A0. */
 #define MLX90393_A1							0						/**< Voltage level of the address pin A1. */

#endif /* CONFIG_MLX90393_H_ */