/*
 * BMP180.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for BMP180 I2C pressure sensor.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Peripheral/BMP180/BMP180.c
 *  @brief Driver for the Bosch BMP180 I2C pressure sensor.
 *
 *  This file contains the implementation of the pressure sensor driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/BMP180/BMP180.h"

/** @defgroup BMP180
 *  @{
 */
 	/** @defgroup BMP180-Register
	 *  AD5933 register addresses.
	 *  @{
	 */
		#define BMP180_REGISTER_ID			0xD0
	/** @} */ // end of BMP180-Register
	 
 	/** @defgroup BMP180-Size
	 *  Register sizes.
	 *  @{
	 */

	/** @} */ // end of BMP180-Size
	 
 	/** @defgroup BMP180-Commands
	 *  BMP180 commands.
	 *  @{
	 */

	/** @} */ // end of BMP180-Commands

 	/** @defgroup BMP180-Status
	 *  BMP180 status bits.
	 *  @{
	 */
	 
	/** @} */ // end of BMP180-Status
	 
 	/** @defgroup BMP180-Control
	 *  BMP180 control bits.
	 *  @{
	 */
	 
	/** @} */ // end of BMP180-Control
/** @} */ // end of BMP180

const I2C_Error_t BMP180_Init(I2CM_Config_t* Config)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	// Initialize the i2c interface
	if(Config != NULL)
	{
		BMP180_I2CM_INIT(Config);
	}
	
	return ErrorCode;
}