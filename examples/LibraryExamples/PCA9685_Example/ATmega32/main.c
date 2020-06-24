/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: PCA9685 example for ATmega32.

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
 *  @brief PCA9685 example for ATmega32.
 *
 *  Software for the XMega PXA9685 example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/PCA9685/PCA9685.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Prescaler = I2C_PRESCALER_16,
	.Bitrate = PCA9685_CLOCK,
	.EnableInterruptSupport = false,
};

int main(void)
{
	PCA9685_Init(&Config_I2CM, PCA9685_CLOCK_INT);

	// Create a PWM with a duty cycle of 20% on channel 0
	PCA9685_SetChannel(0, 409, 1228);

	// Create a PWM with a duty cycle of 33% on channel 1
	PCA9685_SetDuty(1, 33);

    while(1) 
    {
    }
}