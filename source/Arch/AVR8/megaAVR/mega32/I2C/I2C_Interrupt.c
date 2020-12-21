/*
 * I2C_Interrupt.c
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

/** @file Arch/AVR8/megaAVR/mega32/I2C/I2C_Interrupt.c
 *  @brief Driver for AVR8 I2C module.
 *
 *  This file contains the implementation of the interrupt functions for the AVR8 I2C driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/mega32/I2C/I2C.h"

#ifndef DOXYGEN
	struct
	{
		I2C_Callback_t TransferCompleteCallback;
		I2C_Callback_t ReceiveCompleteCallback;
	} __I2C_Callbacks[TWI_DEVICES];

	I2C_Message_t __I2CM_Messages[TWI_DEVICES];
	I2C_Buffer_t __I2CS_Buffer[TWI_DEVICES];
#endif

/** @brief			I2C interrupt handler.
 *  @param Device	Device ID
 *					NOTE: Only 0 if the device has only one interface
 */
static inline void I2C_InterruptHandler(const uint8_t Device)
{
	// Clear interrupt flag
	TWCR |= 0x01 << TWINT;

	if(__I2C_Callbacks[Device].TransferCompleteCallback != NULL)
	{
		__I2C_Callbacks[Device].TransferCompleteCallback();
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TWI_vect)
	{
		I2C_InterruptHandler(0);
	}
#endif