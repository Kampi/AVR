/*
 * SPI.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 SPI interface in master mode.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Arch/AVR8/SPI/SPI.c
 *  @brief Driver for AVR8 SPI interface in master mode.
 *
 *  This contains the implementation of the AVR8 SPI driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/SPI/SPI.h"

#ifndef DOXYGEN
	struct
	{
		SPI_Callback_t TransferCompleteCallback;
	} SPI_Callbacks[SPI_DEVICES];
#endif

/** @brief					SPI interrupt handler.
 *  @param Device			Device ID
 *  @param Callback			Type of interrupt
 */
static inline void SPI_InterruptHandler(const uint8_t Device, const SPI_CallbackType_t Callback)
{
	if(Callback == SPI_COMPLETE_INTERRUPT)
	{
		if(SPI_Callbacks[Device].TransferCompleteCallback != NULL)
		{
			SPI_Callbacks[Device].TransferCompleteCallback();
		}	
	}
}

void SPIM_Init(SPIM_Config_t* Config)
{
	// Enable MOSI and SCK as output
	GPIO_SetDirection(SPI_PORT, SPI_MOSI_PIN, GPIO_DIRECTION_OUT);
	GPIO_SetDirection(SPI_PORT, SPI_SCK_PIN, GPIO_DIRECTION_OUT);

	// Enable MISO as input
	GPIO_SetDirection(SPI_PORT, SPI_MISO_PIN, GPIO_DIRECTION_IN);

	SPI_SetDataOrder(Config->DataOrder);
	SPIM_SetPrescaler(Config->Prescaler);
	SPIM_SetMode(Config->Mode);
	SPI_SetDeviceMode(SPI_MASTER);
	SPI_Enable();
}

uint8_t SPIM_SendData(const uint8_t Data)
{
    SPDR = Data;

	// Wait until transfer is complete
    while(!(SPSR & (0x01 << SPIF)));

    return(SPDR);
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(SPI_STC_vect)
	{
		SPI_InterruptHandler(0, SPI_COMPLETE_INTERRUPT);
	}
#endif