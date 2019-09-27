/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: ADC example for XMega.

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
 *  @brief ADC example for XMega.
 *
 *	This file contains several examples:
 *		a) Polled ADC example. Set symbol EXAMPLE = 1.
 *		   This example start a conversion on pin A0 with channel 0, wait for the result and send the result with the USART.
 *		b) Interrupt example. Set symbol EXAMPLE = 2.
 *		   Same as example 1, but with interrupts.
 *
 *  Software for the XMega ADC tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-adc/
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <stddef.h>

char Result[10];

int main(void)
{
	#if(EXAMPLE == 1)
		/*
			Initialize the USART
				-> 19200 Baud @ 2 MHz with CLK2X = 0, BSCALE = -5
				-> Use Tx only
				-> 8N1
		*/
		PORTC.DIRSET = (0x01 << 0x03);
		USARTC0.BAUDCTRLA = 0xB0 & 0xFF;
		USARTC0.BAUDCTRLB = ((0xB0 & 0xF00) >> 0x08);
		USARTC0.BAUDCTRLB |= ((-5 & 0x0F) << 0x04);
		USARTC0.CTRLB = USART_TXEN_bm;
		USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
		USARTC0.CTRLC &= ~(USART_PMODE0_bm | USART_PMODE1_bm | USART_SBMODE_bm);
	
		/*
			Initialize the ADC
				-> 1 V reference
				-> 12-bit
				-> Unsigned mode
				-> Prescaler 32
		*/
		ADCA.CTRLB = 0x00;
		ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
		ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
		ADCA.CTRLA = ADC_ENABLE_bm;
	
		/*
			Calibrate the ADC
		*/
		uint8_t CalibrationByteL;
		uint8_t CalibrationByteH;
		NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
		CalibrationByteL = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
		CalibrationByteH = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
		NVM_CMD = NVM_CMD_NO_OPERATION_gc;
		ADCA.CALL = CalibrationByteL;
		ADCA.CALH = CalibrationByteH;
	
		/*
			Initialize the ADC channel
				-> Channel 0 (PA0 - Light sensor)
				-> Gain 1x
				-> Single-ended
		*/
		PORTA.DIR &= ~(0x01 << 0x00);
		ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXNEG_PIN0_gc;
	
		// Start a new measurement
		ADCA.CH0.CTRL |= ADC_CH_START_bm;
		while(!(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm));
	
		// Read the result
		ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm;
		uint16_t RawData = ADCA.CH0.RES;

		// Send the result
		itoa(RawData, Result, 10);
		for(uint8_t i = 0x00; i < 0x0A; i++)
		{
			while(!(USARTC0.STATUS & USART_DREIF_bm));
			USARTC0.DATA = Result[i];
		}

		// Send CR + LF
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0A;
			
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0D;	

	#elif(EXAMPLE == 2)
		/*
			Initialize the USART
				-> 19200 Baud @ 2 MHz with CLK2X = 0, BSCALE = -5
				-> Use Tx only
				-> 8N1
		*/
		PORTC.DIRSET = (0x01 << 0x03);
		USARTC0.BAUDCTRLA = 0xB0 & 0xFF;
		USARTC0.BAUDCTRLB = ((0xB0 & 0xF00) >> 0x08);
		USARTC0.BAUDCTRLB |= ((-5 & 0x0F) << 0x04);
		USARTC0.CTRLB = USART_TXEN_bm;
		USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
		USARTC0.CTRLC &= ~(USART_PMODE0_bm | USART_PMODE1_bm | USART_SBMODE_bm);

		/*
			Initialize the ADC
				-> Freerun
				-> 1 V reference
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
		ADCA.CH0.MUXCTRL = ADC_CH_MUXNEG_PIN0_gc;
		ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();

	#endif

    while(1) 
    {
    }
}

ISR(ADCA_CH0_vect)
{
	// Send the result
	itoa(ADCA.CH0.RES, Result, 10);
	for(uint8_t i = 0x00; i < 0x0A; i++)
	{
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = Result[i];

		// Send CR + LF
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0A;
		
		while(!(USARTC0.STATUS & USART_DREIF_bm));
		USARTC0.DATA = 0x0D;
	}
}

