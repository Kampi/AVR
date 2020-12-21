/*
 * I2C.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 I2C module.

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

/** @file Arch/AVR8/megaAVR/I2C/I2C.c
 *  @brief Driver for AVR8 I2C module.
 *
 *  This file contains the implementation of the common functions for the AVR8 I2C driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/mega32/I2C/I2C.h"

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	extern struct
	{
		I2C_Callback_t TransferCompleteCallback;
		I2C_Callback_t ReceiveCompleteCallback;
	} __I2C_Callbacks[TWI_DEVICES];
#endif

void I2C_InstallCallback(const I2C_InterruptConfig_t* Config)
{
	if(Config->Source & I2C_TXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[0].TransferCompleteCallback = Config->Callback;
	}

	if(Config->Source & I2C_RXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[0].ReceiveCompleteCallback = Config->Callback;
	}

	TWCR |= (0x01 << TWIE);
}

void I2C_RemoveCallback(const I2C_CallbackType_t Callback)
{
	if(Callback & I2C_TXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[0].TransferCompleteCallback = NULL;
	}

	if(Callback & I2C_RXCOMPLETE_INTERRUPT)
	{
		__I2C_Callbacks[0].ReceiveCompleteCallback = NULL;
	}

	TWCR &= ~(0x01 << TWIE);
}