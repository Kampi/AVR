/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: DMA example for XMega.

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
 *  @brief DMA example for XMega.
 *
 *	This file contains several examples:
 *		1) Transmit the result of an ADC conversion to the DAC. Set symbol EXAMPLE = 1.
 *		2) Transmit 10 bytes from 5 ADC conversions into an SRAM buffer. Set symbol EXAMPLE = 2.
 *
 *  Software for the XMega DMA tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-dma/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define SAMPLES								5

uint8_t Finish = 0x00;
uint8_t Destination[SAMPLES * 2];

int main(void)
{
	#if(EXAMPLE == 1)
		/*
			Initialize the DAC
				-> DACB
				-> 1 V reference
				-> Right-adjust
				-> Channel 0 (PB2)
		*/
		DACB.CTRLC &= ~(DAC_REFSEL1_bm | DAC_REFSEL0_bm | DAC_LEFTADJ_bm);
		DACB.CTRLB &= ~(DAC_CH1TRIG_bm | DAC_CH0TRIG_bm | DAC_CHSEL1_bm | DAC_CHSEL0_bm);
		DACB.CTRLA &= DAC_CH0EN_bm | DAC_ENABLE_bm;
	
		/*
			Initialize the ADC
				-> Internal 1 V reference
				-> Free run
				-> 12-bit
				-> Unsigned mode
				-> Prescaler 32
		*/
		ADCA.CTRLB = ADC_FREERUN_bm;
		ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
		ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
		ADCA.CTRLA = ADC_ENABLE_bm;
	
		/*
			Initialize the ADC channel
				-> Channel 0 (PA0 - Light sensor)
				-> Gain 1x
				-> Single-ended
		*/
		PORTA.DIR &= ~(0x01 << 0x00);
		ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
		ADCA.CH0.MUXCTRL = 0x00;

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
				-> Source reload after each burst
				-> Destination reload after each burst
				-> Source and Destination address fixed
				-> Trigger source ADCA.CH0
				-> Source address ADCA.CH0.RES
				-> Destination address DACB.CH0DATA
		*/
		DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_LO_gc;
	
		DMA.CH0.TRFCNT = 2;
		DMA.CH0.CTRLA = DMA_CH_BURSTLEN_2BYTE_gc;
		DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
	
		DMA.CH0.SRCADDR0 = ((uintptr_t)&ADCA.CH0.RES) & 0xFF;
		DMA.CH0.SRCADDR1 = (((uintptr_t)&ADCA.CH0.RES) >> 0x08) & 0xFF;
		DMA.CH0.SRCADDR2 = (((uintptr_t)&ADCA.CH0.RES) >> 0x0F) & 0xFF;

		DMA.CH0.DESTADDR0 = ((uintptr_t)&DACB.CH0DATA) & 0xFF;
		DMA.CH0.DESTADDR1 = ((uintptr_t)&DACB.CH0DATA >> 0x08) & 0xFF;
		DMA.CH0.DESTADDR2 = ((uintptr_t)&DACB.CH0DATA >> 0x0F) & 0xFF;
	
		DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc;

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
	
		// Activate the DMA channel
		DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	#elif(EXAMPLE == 2)	
		/*
			Initialize the ADC
				-> Internal 1 V reference
				-> Free run
				-> 12-bit
				-> Unsigned mode
				-> Prescaler 32
		*/
		ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
		ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
		ADCA.CTRLA = ADC_ENABLE_bm;
	
		/*
			Initialize the ADC channel
				-> Channel 0 (PA0 - Light sensor)
				-> Gain 1x
				-> Single-ended
		*/
		PORTA.DIR &= ~(0x01 << 0x00);
		ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
		ADCA.CH0.MUXCTRL = 0x00;

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
				-> Block size 10 byte
				-> No repeat
				-> Single shot
				-> Source reload after each burst
				-> Destination reload after each transaction
				-> Source and Destination address increase
				-> Trigger source ADCA.CH0
				-> Source address ADCA.CH0.RES
				-> Destination address SRAM
		*/
		DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_LO_gc;
	
		DMA.CH0.TRFCNT = 10;
		DMA.CH0.CTRLA = DMA_CH_BURSTLEN_2BYTE_gc | DMA_CH_SINGLE_bm;
		DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
	
		DMA.CH0.SRCADDR0 = ((uintptr_t)&ADCA.CH0.RES) & 0xFF;
		DMA.CH0.SRCADDR1 = (((uintptr_t)&ADCA.CH0.RES) >> 0x08) & 0xFF;
		DMA.CH0.SRCADDR2 = (((uintptr_t)&ADCA.CH0.RES) >> 0x0F) & 0xFF;

		DMA.CH0.DESTADDR0 = ((uintptr_t)Destination) & 0xFF;
		DMA.CH0.DESTADDR1 = ((uintptr_t)Destination >> 0x08) & 0xFF;
		DMA.CH0.DESTADDR2 = ((uintptr_t)Destination >> 0x0F) & 0xFF;
	
		DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc;

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
	
		// Activate the DMA channel
		DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
		
		for(uint8_t i = 0x00; i < SAMPLES; i++)
		{
			ADCA.CH0.CTRL |= ADC_CH_START_bm;
			
			// Wait some time for the next conversion
			for(uint16_t i = 0x00; i < 0xFFFF; i++);
		}

	#endif

	while(!Finish);

    while(1)
    {
    }
}

ISR(DMA_CH0_vect)
{
	#if(EXAMPLE == 1)
		// Disable global interrupts
		cli();
	
		// Clear the status flag
		DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;
	
		// Enable the channel again
		DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	
		// Enable global interrupts
		sei();
	#elif(EXAMPLE == 2)
		Finish = 0x01;
	#endif
}

