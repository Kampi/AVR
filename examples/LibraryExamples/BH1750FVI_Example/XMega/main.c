/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: BH1750VFI example for XMega.

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
 *  @brief BH1750VFI example for XMega.
 *
 *  Software for the XMega BH1750VFI example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/BH1750FVI/BH1750FVI.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &BH1750VI_INTERFACE,
	.Bitrate = BH1750VI_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

int main(void)
{
	uint16_t Result = 0x00;
	
	BH1750VI_Init(&Config_I2CM);
	BH1750VI_SingleMeasurement(BH1750VI_RES_LOW, &Result);
	
    while(1) 
    {
		BH1750VI_StartMeasurement(BH1750VI_RES_HIGH);

		for(uint8_t i = 0x00; i < 10; i++)
		{
			// Wait a few seconds
			for(uint32_t i = 0x00; i < 0x1E8480; i++);
			
			BH1750VI_GetResult(&Result);
		}
		
		BH1750VI_StopMeasurement();
    }
}