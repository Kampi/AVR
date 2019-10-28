/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: SPI example for XMega.

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
 *  @brief SPI example for XMega.
 *
 *	This file contains several examples:
 *		a) USART in SPI master mode. Set symbol SPI_MODE = 1.
 *			1) Polled communication with the USART in SPI in master mode. Set symbol EXAMPLE = 1.
 *			   This example writes and receive several bytes from the microcontroller.
 *			   NOTE: You have to bridge MISO and MOSI.
 *			2) Same as example 1, but with interrupt support. Set symbol EXAMPLE = 2.
 *			   NOTE: You have to bridge MISO and MOSI.
 *		b) SPI master mode. Set symbol SPI_MODE = 2.
 *			1) Polled communication with the SPI in master mode. Set symbol EXAMPLE = 1.
 *			   This example writes and receive several bytes from the microcontroller.
 *			   NOTE: You have to bridge MISO and MOSI.
 *			2) Same as example 1, but with interrupt support. Set symbol EXAMPLE = 2.
 *		c) SPI slave mode. Set symbol SPI_MODE = 3.
 *				This example use the USARTD0 interface as SPI master and SPIC as SPI slave to 
 *				transmit data from the master interface to the slave interface.
 *
 *  Software for the XMega SPI tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-spi/
 *
 *  @author Daniel Kampert
 */

#include "SPI/SPI.h"

uint8_t Received = 0x00;
uint8_t SPI_TransmitBuffer[SPI_BUFFER_SIZE];
uint8_t SPI_ReceiveBuffer[SPI_BUFFER_SIZE];

uint8_t SPI_RxSlaveBuffer[SPI_BUFFER_SIZE];
uint8_t SPI_TxSlaveBuffer[SPI_BUFFER_SIZE];

int main(void)
{
	#if(SPI_MODE == 1)
		#if(EXAMPLE == 1)
			/*
				Polled SPI example
			*/
			
			// Fill the buffer
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_TransmitBuffer[i] = i;
			}
		
			/*
				Initialize the USART in master mode SPI
					-> Master mode
					-> Mode 0
					-> MSB first
					-> No Interrupts
					-> 250 kHz clock frequency @ 2 MHz
			*/
			USART_SPI_Init();
			Received = USART_SPI_SendData(0xBB);
			
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_ReceiveBuffer[i] = USART_SPI_SendData(SPI_TransmitBuffer[i]);
			}
		
		#elif(EXAMPLE == 2)
			/*
				Interrupt driven USART-SPI example
			*/

			/*
				Initialize SS pin
					-> Pin E.0 as output
			*/
			PORTE.DIR |= (0x01 << 0x00);
			PORTE.OUTSET = (0x01 << 0x00);

			// Fill the buffer
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_TransmitBuffer[i] = i + 0x01;
			}
			
			/*
				Initialize the USART in master mode SPI
					-> Master mode
					-> Mode 0
					-> MSB first
					-> Rx & Tx interrupts (low level)
					-> 250 kHz clock frequency @ 2 MHz
			*/
			USART_SPI_InitInterrupt();

			/*
				Initialize the interrupts
					-> Enable low level interrupts
					-> Enable global interrupts
			*/
			PMIC.CTRL = PMIC_LOLVLEN_bm;
			sei();
			
			SPI_Message_t Message = {
				.BufferIn = SPI_ReceiveBuffer,
				.BufferOut = SPI_TransmitBuffer,
				.Length = SPI_BUFFER_SIZE,
				.Port = &PORTE,
				.Pin = 0
			};

			USART_SPI_Transmit(&Message);
			while(!USART_SPI_Status());
		
		#endif
	#elif(SPI_MODE == 2)
		#if(EXAMPLE == 1)
			/*
				Polled SPI example
			*/
			
			// Fill the buffer
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_TransmitBuffer[i] = i + 0x02;
			}
			
			/*
				Initialize master mode SPI
					-> Normal SPI
					-> Master mode
					-> Mode 0
					-> MSB first
					-> No Interrupts
					-> 500 kHz clock frequency @ 2 MHz
			*/
			SPIM_Init();
			Received = SPIM_SendData(0xAA);
			
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_ReceiveBuffer[i] = SPIM_SendData(SPI_TransmitBuffer[i]);
			}
		
		#elif(EXAMPLE == 2)
			/*
				Interrupt driven SPI example
			*/
			
			/*
				Initialize SS pin
					-> Pin C.4 as output
			*/
			PORTC.DIR |= (0x01 << 0x04);
			PORTC.OUTSET = (0x01 << 0x04);

			// Fill the buffer
			for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
			{
				SPI_TransmitBuffer[i] = i + 0x03;
			}
			
			/*
				Initialize master mode SPI
					-> Normal SPI
					-> Master mode
					-> Mode 0
					-> MSB first
					-> Data interrupt (low level)
					-> 500 kHz clock frequency @ 2 MHz
			*/
			SPIM_InitInterrupt();
			
			/*
				Initialize the interrupts
					-> Enable low level interrupts
					-> Enable global interrupts
			*/
			PMIC.CTRL = PMIC_LOLVLEN_bm;
			sei();
			
			SPI_Message_t Message = {
				.BufferIn = SPI_ReceiveBuffer,
				.BufferOut = SPI_TransmitBuffer,
				.Length = SPI_BUFFER_SIZE,
				.Port = &PORTC,
				.Pin = 4
			};
			
			SPIM_Transmit(&Message);
			while(!SPIM_Status());
		
		#endif
	#elif(SPI_MODE == 3)
		/*
			Interrupt driven SPI slave example
		*/
			
		/*
			Initialize SS pin for the USART/SPI master
				-> Pin E.0 as output
		*/
		PORTE.DIR |= (0x01 << 0x00);
		PORTE.OUTSET = (0x01 << 0x00);

		// Fill the master transmit buffer
		for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
		{
			SPI_TransmitBuffer[i] = i + 0x03;
		}

		// Fill the slave transmit buffer
		for(uint8_t i = 0x00; i < SPI_BUFFER_SIZE; i++)
		{
			SPI_TxSlaveBuffer[i] = i + 0x03;
		}

		/*
			Initialize the USART in master mode SPI
				-> Master mode
				-> Mode 0
				-> MSB first
				-> Rx & Tx interrupts (low level)
				-> 250 kHz clock frequency @ 2 MHz
		*/
		USART_SPI_InitInterrupt();

		SPI_Message_t Message = {
			.BufferIn = SPI_ReceiveBuffer,
			.BufferOut = SPI_TransmitBuffer,
			.Length = SPI_BUFFER_SIZE,
			.Port = &PORTE,
			.Pin = 0
		};
		
		/*
			Initialize slave mode SPI
				-> Data interrupt (low level)
		*/
		SPIS_Init(SPI_RxSlaveBuffer, SPI_TxSlaveBuffer);

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
		
		// Send and wait for transmission end
		USART_SPI_Transmit(&Message);
		while(!USART_SPI_Status());

		// Wait until the master release the slave
		while(SPIS_ActiveTransmission());

	#endif

    while(1) 
    {
    }
}

