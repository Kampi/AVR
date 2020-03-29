/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Event system example for XMega.

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
 *  @brief Event system example for XMega. In this example a ADC conversation
 *  is triggered with an external push button over the event system. If the conversion
 *  has finished, the conversion result is ransmitted with DMA.
 *
 *  Software for the XMega event system tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-eventsystem/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t DMA_DestinationBuffer[2];

int main(void)
{
	/*
		Initialize E.5
			-> Input
			-> Falling edge detection
	*/
	PORTE.DIRCLR = (0x01 << 0x05);
	PORTE.PIN5CTRL = PORT_ISC_FALLING_gc;
		
	/*
		Initialize the ADC
			-> 1 V reference
			-> Single shot
			-> 12-bit
			-> Unsigned mode
			-> Prescaler 32
			-> Common DMA requests for ADC channels 0 and 1
			-> Event channel 7
				-> Sweep only channel 0
				-> Event channel 7 triggers conversion on channel 0
	*/
	ADCA.CTRLA = ADC_DMASEL0_bm;
	ADCA.REFCTRL = 0x00;
	ADCA.EVCTRL = (ADC_EVSEL2_bm | ADC_EVSEL1_bm | ADC_EVSEL0_bm) | ADC_EVACT0_bm;
	ADCA.PRESCALER = ADC_PRESCALER0_bm | ADC_PRESCALER1_bm;
	ADCA.CTRLA |= ADC_ENABLE_bm;
	
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
			-> Source reload after each transaction
			-> Destination reload after each transaction
			-> Source and Destination address fixed
			-> Trigger source ADCA.CH0
			-> Source address ADCA.CH0.RES
			-> Destionation address DMA_DestinationBuffer
	*/
	DMA.CH0.CTRLB = PMIC_LOLVLEN_bm;
	
	DMA.CH0.TRFCNT = 2;
	DMA.CH0.CTRLA = DMA_CH_BURSTLEN0_bm;
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
	
	DMA.CH0.SRCADDR0 = ((uintptr_t)&ADCA.CH0.RES) & 0xFF;
	DMA.CH0.SRCADDR1 = (((uintptr_t)&ADCA.CH0.RES) >> 0x08) & 0xFF;
	DMA.CH0.SRCADDR2 = (((uintptr_t)&ADCA.CH0.RES) >> 0x0F) & 0xFF;

	DMA.CH0.DESTADDR0 = ((uintptr_t)&DMA_DestinationBuffer) & 0xFF;
	DMA.CH0.DESTADDR1 = ((uintptr_t)&DMA_DestinationBuffer >> 0x08) & 0xFF;
	DMA.CH0.DESTADDR2 = ((uintptr_t)&DMA_DestinationBuffer >> 0x0F) & 0xFF;
	
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc ;
	
	/*
		Configure event system
			-> Use SW0 (Port E, Pin 5) as event trigger
			-> Use event channel 7
			-> Use digital filter to reduce bounces
	*/
	EVSYS.CH7MUX = EVSYS_CHMUX_PORTE_PIN5_gc;
	EVSYS.CH7CTRL = (EVSYS_DIGFILT2_bm | EVSYS_DIGFILT1_bp | EVSYS_DIGFILT0_bm);
	
	/*
		Initialize the interrupts
			-> Enable low level interrupts
			-> Enable global interrupts
	*/
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();
	
	// Activate the DMA channel
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	
    while(1) 
    {
    }
}

ISR(DMA_CH0_vect)
{
	// Disable global interrupts
	cli();
	
	// Clear the status flag
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;
	
	// Enable the channel again
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	
	// Enable global interrupts
	sei();
}
