/*
 * SPIM.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: SPI master driver for XMega SPI

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

/** @file XMega/SPI/SPIM.c
 *  @brief Driver for XMega SPI master mode.
 *
 *  This file contains the implementation of the XMega SPI master driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/SPI/SPI.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	extern SPI_Message_t _SPIM_Messages[SPI_DEVICES];
	SPI_DeviceMode_t _SPI_DeviceModes[SPI_DEVICES];
#endif

void SPIM_Init(SPIM_Config_t* Config)
{
	uint8_t ID = 0x00;
	
	if(Config->Device == &SPIC)
	{
		ID = SPIC_ID;
	}
	else if(Config->Device == &SPID)
	{
		ID = SPID_ID;
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

	_SPI_DeviceModes[ID] = SPI_MASTER;
	
	if(ID == SPIC_ID)
	{
		GPIO_SetDirection(&PORTC, SPI_SS_PIN, GPIO_DIRECTION_OUT);
		GPIO_SetDirection(&PORTC, SPI_SCK_PIN, GPIO_DIRECTION_OUT);

		if((Config->Mode == SPI_MODE_2) || (Config->Mode == SPI_MODE_3))
		{
			GPIO_Set(&PORTC, SPI_SCK_PIN);
		}

		GPIO_SetDirection(&PORTC, SPI_MOSI_PIN, GPIO_DIRECTION_OUT);
	}
	else if(ID == SPID_ID)
	{
		GPIO_SetDirection(&PORTD, SPI_SS_PIN, GPIO_DIRECTION_OUT);
		GPIO_SetDirection(&PORTD, SPI_SCK_PIN, GPIO_DIRECTION_OUT);

		if((Config->Mode == SPI_MODE_2) || (Config->Mode == SPI_MODE_3))
		{
			GPIO_Set(&PORTD, SPI_SCK_PIN);
		}

		GPIO_SetDirection(&PORTD, SPI_MOSI_PIN, GPIO_DIRECTION_OUT);
	}
	#if(defined SPIE)
		else if(ID == SPID_IE)
		{
			GPIO_SetDirection(&PORTE, SPI_SS_PIN, GPIO_DIRECTION_OUT);
			GPIO_SetDirection(&PORTE, SPI_SCK_PIN, GPIO_DIRECTION_OUT);

			if((Config->Mode == SPI_MODE_2) || (Config->Mode == SPI_MODE_3))
			{
				GPIO_Set(&PORTE, SPI_SCK_PIN);
			}

			GPIO_SetDirection(&PORTE, SPI_MOSI_PIN, GPIO_DIRECTION_OUT);
		}
	#endif
	#if(defined SPIE)
		else if(ID == SPID_IF)
		{
			GPIO_SetDirection(&PORTF, SPI_SS_PIN, GPIO_DIRECTION_OUT);
			GPIO_SetDirection(&PORTF, SPI_SCK_PIN, GPIO_DIRECTION_OUT);

			if((Config->Mode == SPI_MODE_2) || (Config->Mode == SPI_MODE_3))
			{
				GPIO_Set(&PORTF, SPI_SCK_PIN);
			}

			GPIO_SetDirection(&PORTF, SPI_MOSI_PIN, GPIO_DIRECTION_OUT);
		}
	#endif

	_SPI_DeviceModes[ID] = SPI_MASTER;

	Device->CTRL = (Device->CTRL & (~(0x01 << 0x04))) | (SPI_MASTER << 0x04);
	SPI_SetMode(Device, Config->Mode);
	SPI_SetDataOrder(Device, Config->DataOrder);
	SPIM_SetPrescaler(Device, Config->Prescaler);

	SPI_Enable(Device);
}

void SPIM_SetClock(SPI_t* Device, const uint32_t SPIClock, const uint32_t Clock)
{
	uint8_t Prescaler = Clock / SPIClock;

	// Clear Prescaler bits
	Device->CTRL &= ~0x03;

	if(Prescaler < 2)
	{
		Device->CTRL |= SPI_PRESCALER_2;
	}
	else if((Prescaler > 2) && (Prescaler < 4))
	{
		Device->CTRL |= SPI_PRESCALER_4;
	}
	else if((Prescaler > 4) && (Prescaler < 8))
	{
		Device->CTRL |= SPI_PRESCALER_8;
	}
	else if((Prescaler > 8) && (Prescaler < 16))
	{
		Device->CTRL |= SPI_PRESCALER_16;
	}
	else if((Prescaler > 16) && (Prescaler < 32))
	{
		Device->CTRL |= SPI_PRESCALER_32;
	}
	else if((Prescaler > 32) && (Prescaler < 64))
	{
		Device->CTRL |= SPI_PRESCALER_64;
	}
	else if((Prescaler > 64) && (Prescaler < 128))
	{
		Device->CTRL |= SPI_PRESCALER_128;
	}
}

const uint32_t SPIM_GetClock(SPI_t* Device, const uint32_t Clock)
{
	uint8_t Clk2x = ((Device->CTRL & (0x01 << 0x07)) >> 0x07);

	switch(Device->CTRL & 0x03)
	{
		case(0x01):
		{
			return (Clock >> (0x04 >> Clk2x));
		}
		case(0x02):
		{
			return (Clock >> (0x06 >> Clk2x));
		}
		case(0x03):
		{
			return (Clock >> (0x07 >> Clk2x));
		}
		default:
		{
			return (Clock >> (0x02 >> Clk2x));
		}
	}
}

const uint8_t SPIM_SendData(SPI_t* Device, const uint8_t Data)
{
	Device->DATA = Data;

	while(!(Device->STATUS & SPI_IF_bm));

	return Device->DATA;
}

SPI_Status_t SPIM_Transmit(SPI_t* Device, const uint8_t Bytes, uint8_t* WriteBuffer, uint8_t* ReadBuffer, PORT_t* Port, const uint8_t Pin)
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

	if(_SPIM_Messages[ID].Status != SPI_MESSAGE_COMPLETE)
	{
		return _SPIM_Messages[ID].Status;
	}

	_SPIM_Messages[ID].Device = Device;
	_SPIM_Messages[ID].BufferIn = ReadBuffer;
	_SPIM_Messages[ID].BufferOut = WriteBuffer;
	_SPIM_Messages[ID].Length = Bytes;
	_SPIM_Messages[ID].BytesProcessed = 0x00;
	_SPIM_Messages[ID].Status = SPI_MESSAGE_PENDING;
	_SPIM_Messages[ID].Port = Port;
	_SPIM_Messages[ID].Pin = Pin;

	SPIM_SelectDevice(_SPIM_Messages[ID].Port, _SPIM_Messages[ID].Pin);

	return SPI_MESSAGE_PENDING;
}

const SPI_Status_t SPIM_Status(const SPI_t* Device)
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

	return _SPIM_Messages[ID].Status;
}