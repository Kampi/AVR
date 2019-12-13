/*
 * I2C.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega I2C module.

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

/** @file Arch/XMega/I2C/I2C.c
 *  @brief Driver for XMega I2C module.
 *
 *  This file contains the implementation of the common functions for the XMega I2C driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/I2C/I2C.h"

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	extern struct
	{
		I2C_Callback_t TxCompleteInterrupt;
		I2C_Callback_t RxCompleteInterrupt;
		I2C_Callback_t ErrorInterrupt;
		I2C_Callback_t DataRdyInterrupt;
	} __I2C_Callbacks[TWI_DEVICES];
#endif

void I2C_InstallCallback(const I2C_InterruptConfig_t* Config)
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

	if(Config->Source & I2C_TXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[ID].TxCompleteInterrupt = Config->Callback;
	}

	if(Config->Source & I2C_RXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[ID].RxCompleteInterrupt = Config->Callback;
	}

	if(Config->Source & I2C_ERROR_INTERRUPT)
	{
		__I2C_Callbacks[ID].ErrorInterrupt = Config->Callback;
	}
	
	if(Config->Source & I2C_DATA_RDY_INTERRUPT)
	{
		__I2C_Callbacks[ID].DataRdyInterrupt = Config->Callback;
	}

	if((Config->Source & I2C_TXCOMPLETE_INTERRUPT) || (Config->Source & I2C_RXCOMPLETE_INTERRUPT) || (Config->Source & I2C_ERROR_INTERRUPT))
	{
		Config->Device->MASTER.CTRLA = (Config->Device->MASTER.CTRLA & (~(0x03 << 0x06))) | (Config->InterruptLevel << 0x06);
	}
	else
	{
		Config->Device->SLAVE.CTRLA = (Config->Device->SLAVE.CTRLA & (~(0x03 << 0x06))) | (Config->InterruptLevel << 0x06);
	}
}

void I2C_RemoveCallback(TWI_t* Device, const I2C_CallbackType_t Callback)
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
	
	if(Callback & I2C_TXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[ID].TxCompleteInterrupt = NULL;
	}
	
	if(Callback & I2C_RXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[ID].TxCompleteInterrupt = NULL;
	}
	
	if(Callback & I2C_ERROR_INTERRUPT)
	{
		__I2C_Callbacks[ID].ErrorInterrupt = NULL;
	}
	
	if(Callback & I2C_DATA_RDY_INTERRUPT)
	{
		__I2C_Callbacks[ID].DataRdyInterrupt = NULL;
	}
}

void I2C_ChangeInterruptLevel(TWI_t* Device, const I2C_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel)
{
	if((Callback & I2C_TXCOMPLETE_INTERRUPT) || (Callback & I2C_RXCOMPLETE_INTERRUPT) ||(Callback & I2C_ERROR_INTERRUPT))
	{
		Device->MASTER.CTRLA = (Device->MASTER.CTRLA & (~(0x03 << 0x06))) | (InterruptLevel << 0x06);
	}
	else
	{
		Device->SLAVE.CTRLA = (Device->SLAVE.CTRLA & (~(0x03 << 0x06))) | (InterruptLevel << 0x06);
	}
}
