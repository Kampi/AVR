/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Nunchuk example for XMega.

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

/** @file main.c
 *  @brief Nunchuk example for XMega.
 *
 *  Software for the XMega Nunchuk example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/Nunchuk/Nunchuk.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &NUNCHUK_INTERFACE,
	.Bitrate = NUNCHUK_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

Nunchuk_Data_t Data;

int main(void)
{
	Nunchuk_Init(&Config_I2CM);

    while(1) 
    {
		Nunchuk_Read(&Data);
		_delay_ms(1000);
    }
}