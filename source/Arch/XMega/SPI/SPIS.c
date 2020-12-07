/*
 * SPIS.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: SPI slave driver for XMega TWI

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

/** @file XMega/SPI/SPIS.c
 *  @brief Driver for XMega SPI slave mode.
 *
 *  This file contains the implementation of the XMega SPI slave driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/SPI/SPI.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	extern SPI_Buffer_t _SPIS_Buffer[SPI_DEVICES];
	SPI_DeviceMode_t _SPI_DeviceModes[SPI_DEVICES];
#endif

void SPIS_Init(SPIS_Config_t* Config)
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

	SPI_t* Device = (SPI_t*)Config->Device;
	SPI_PowerEnable(Device);

	_SPI_DeviceModes[ID] = SPI_SLAVE;

	if(ID == SPIC_ID)
	{
		GPIO_SetDirection(&PORTC, SPI_MISO_PIN, GPIO_DIRECTION_OUT);
	}
	else if(ID == SPID_ID)
	{
		GPIO_SetDirection(&PORTD, SPI_MISO_PIN, GPIO_DIRECTION_OUT);
	}
	else if(ID == SPIE_ID)
	{
		GPIO_SetDirection(&PORTE, SPI_MISO_PIN, GPIO_DIRECTION_OUT);
	}
	else if(ID == SPIF_ID)
	{
		GPIO_SetDirection(&PORTF, SPI_MISO_PIN, GPIO_DIRECTION_OUT);
	}

	SPI_Enable(Device);
}

const SPI_Status_t SPIS_Status(const SPI_t* Device)
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
		else if(Device == &SPIF)
		{
			ID = SPIF_ID;
		}
	#endif

	return _SPIS_Buffer[ID].Status;
}