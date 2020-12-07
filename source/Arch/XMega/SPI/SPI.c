/*
 * SPI.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega SPI

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

/** @file XMega/SPI/SPI.c
 *  @brief Driver for XMega SPI module.
 *
 *  This file contains the implementation of the common functions for the XMega SPI driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/SPI/SPI.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	struct
	{
		SPI_Callback_t CompleteInterrupt;
		SPI_Callback_t ErrorCallback;
	} _SPI_Callbacks[SPI_DEVICES];

	extern SPI_DeviceMode_t _SPI_DeviceModes[SPI_DEVICES];
#endif

void SPI_InstallCallback(const SPI_InterruptConfig_t* Config)
{
	uint8_t ID = 0x00;

	if(Config->Device == &SPIC)
	{
		ID = SPIC_ID;
	}
	else if(Config->Device == &SPID)
	{
		ID = SPIE_ID;
	}

	#if(defined SPIE)
		else if(Config->Device == &SPIE)
		{
			ID = SPIE_ID;
		}
	#endif

	#if(defined SPIF)
		else if(Config->Device == &SPIF)
		{
			ID = SPIF_ID;
		}
	#endif

	if(Config->Source & SPI_COMPLETE_INTERRUPT)
	{
		_SPI_Callbacks[ID].CompleteInterrupt = Config->Callback;
	}

	if(Config->Source & SPI_ERROR_INTERRUPT)
	{
		_SPI_Callbacks[ID].ErrorCallback = Config->Callback;
	}
}

void SPI_RemoveCallback(SPI_t* Device, const SPI_CallbackType_t Callback)
{
	uint8_t ID = 0x00;

	if(Device == &SPIC)
	{
		ID = SPIC_ID;
	}
	else if(Device == &SPID)
	{
		ID = SPIE_ID;
	}

	#if(defined SPIE)
		else if(Device == &SPIE)
		{
			ID = SPIE_ID;
		}
	#endif

	#if(defined SPIF)
		else if(>Device == &SPIF)
		{
			ID = SPIF_ID;
		}
	#endif

	if(Callback & SPI_COMPLETE_INTERRUPT)
	{
		_SPI_Callbacks[ID].CompleteInterrupt = NULL;
	}

	if(Callback & SPI_ERROR_INTERRUPT)
	{
		_SPI_Callbacks[ID].ErrorCallback = NULL;
	}
}

void SPI_ChangeInterruptLevel(SPI_t* Device, const SPI_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel)
{
	// ToDo
}