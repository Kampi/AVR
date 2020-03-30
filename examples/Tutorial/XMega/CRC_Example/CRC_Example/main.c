/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: CRC example for XMega.

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
 *  @brief CRC example for XMega.
 *
 *  Software for the XMega CRC tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-crc/ 
 *
 *	This file contains several examples:
 *		a) CRC over a given data array. Set symbol EXAMPLE = 1.
 *		   The results are:
 *		       -> 0x178D for CRC16
 *			   -> 0x88AA689F for CRC32
 *		b) CRC32 over the first 8 bytes of the flash memory. Set symbol EXAMPLE = 2.
 *		   The result is 0xB9AD0FB1
 *		   Perform a CRC32 computation over a given flash range.
 *		   NOTE: You have to choose an even start address and length!
 *		c) CRC16 with DMA support. Set symbol EXAMPLE = 3.
 *		   Perform a CRC computation with data from a DMA channel.
 *		   The result is 0x178D for CRC16
 *
 *  You can also check the computation online https://crccalc.com/
 *
 *  @author Daniel Kampert
 */

#include "CRC/CRC.h"

#include <avr/interrupt.h>

#define	DATA_LEGNTH			8

uint8_t Ready = 0x00;
uint16_t CRC16 = 0x00;
uint32_t CRC32 = 0x00;

uint8_t Data_In[DATA_LEGNTH];

int main(void)
{
	#if(EXAMPLE == 1)
		// Fill the buffer
		for(uint8_t i = 0x00; i < DATA_LEGNTH; i++)
		{
			Data_In[i] = i;
		}	
	
		CRC16 = CRC_Data(Data_In, DATA_LEGNTH, 0);
		CRC32 = CRC_Data(Data_In, DATA_LEGNTH, 1);

	#elif(EXAMPLE == 2)
		CRC32 = CRC_Flash(0x00, 0x08);

	#elif(EXAMPLE == 3)
		// Fill the buffer
		for(uint8_t i = 0x00; i < DATA_LEGNTH; i++)
		{
			Data_In[i] = i;
		}
		
		/*
			Initialize the DMA
				-> Round robin
				-> No double buffer
		*/
		DMA.CTRL = DMA_ENABLE_bm;
		
		/*
			Initialize the DMA channel
				-> Channel 0
				-> Transaction complete interrupt (low level)
				-> Burst length 2 byte
				-> Block size 2 byte
				-> No repeat
				-> No single shot
				-> Source reload after each transaction
				-> Destination reload after each transaction
				-> Source and Destination address fixed
				-> Trigger source software
				-> Source address data array
				-> Destionation address CRC.DATAIN
		*/
		DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_LO_gc;
	
		DMA.CH0.TRFCNT = DATA_LEGNTH;
		DMA.CH0.CTRLA = DMA_CH_BURSTLEN0_bm;
		DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_OFF_gc;
	
		DMA.CH0.SRCADDR0 = ((uintptr_t)&Data_In) & 0xFF;
		DMA.CH0.SRCADDR1 = (((uintptr_t)&Data_In) >> 0x08) & 0xFF;
		DMA.CH0.SRCADDR2 = (((uintptr_t)&Data_In) >> 0x0F) & 0xFF;

		DMA.CH0.DESTADDR0 = ((uintptr_t)&CRC.DATAIN) & 0xFF;
		DMA.CH0.DESTADDR1 = ((uintptr_t)&CRC.DATAIN >> 0x08) & 0xFF;
		DMA.CH0.DESTADDR2 = ((uintptr_t)&CRC.DATAIN >> 0x0F) & 0xFF;
	
		DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc;
		
		// Activate the DMA channel
		DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;		

		/*
			Configure CRC module
				-> 16 bit CRC
				-> DMA channel 0
		*/
		CRC_DMAStart(0, 0);
		
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();

		// Start the DMA transfer
		DMA.CH0.CTRLA |= DMA_CH_TRFREQ_bm;

		while(!Ready);

	#endif
	
    while(1) 
    {
    }
}

ISR(DMA_CH0_vect)
{
	// Clear the status flag
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;
	
	CRC16 = CRC_DMAStop();
	Ready = 0x01;
}

