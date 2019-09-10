/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: BMP180 example for XMega.

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

/** @file main.c
 *  @brief BMP180 example for XMega.
 *
 *  Software for the XMega BMP180 example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/BMP180/BMP180.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &BMP180_INTERFACE,
	.BitRate = BMP180_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

int main(void)
{
	BMP180_CalibCoef_t Coefficients;
	BMP180_DataPoint_t DataPoint;

	BMP180_Init(&Config_I2CM);
	BMP180_ReadCalibration(&Coefficients);
	BMP180_SingleMeasurement(BMP180_OSS_8, &Coefficients, &DataPoint);

    while(1) 
    {
    }
}