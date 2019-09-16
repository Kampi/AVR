/*
 * I2CS.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega I2C slave mode.

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

/** @file Arch/XMega/I2C/I2CS.c
 *  @brief Driver for XMega I2C slave mode.
 *
 *  This file contains the implementation of the XMega I2C slave driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/I2C/I2C.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	extern I2C_Buffer_t __I2CS_Buffer[TWI_DEVICES];
#endif

void I2CS_Init(I2CS_Config_t* Config)
{
	uint8_t ID = 0x00;
	
	if(Config->Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Config->Device == &TWIE)
	{
		ID = TWIE_ID;
	}
	
	I2C_PowerEnable(Config->Device);

	I2C_SlaveEnable(Config->Device);
	I2CS_SetAddress(Config->Device, Config->Address);
	I2CS_SetMask(Config->Device, Config->Mask);
	I2CS_SwitchSmartMode(Config->Device, Config->EnableSmartMode);
	I2CS_SwitchPromiscuousMode(Config->Device, Config->EnablePromiscuousMode);
	
	__I2CS_Buffer[ID].Buffer = Config->Buffer;
	__I2CS_Buffer[ID].Device = Config->Device;

	Config->Device->SLAVE.CTRLA |= TWI_SLAVE_DIEN_bm | TWI_SLAVE_APIEN_bm | TWI_SLAVE_PIEN_bm;
	Config->Device->SLAVE.CTRLA = (Config->Device->SLAVE.CTRLA & (~(0x03 << 0x06))) | (Config->InterruptLevel << 0x06);
}

I2C_SlaveStatus_t I2CS_Status(TWI_t* Device)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}

	return __I2CS_Buffer[ID].Status;
}