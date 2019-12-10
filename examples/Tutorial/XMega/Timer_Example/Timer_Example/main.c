/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Timer example for XMega.

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
 *  @brief Timer example for XMega. 
 *
 *	This file contains several examples:
 *		1) Use Timer 0 to create an interrupt every second. Set symbol EXAMPLE = 1.
 *		   f = f_Clk / (N * (PER + 1))
 *		2) Use Timer 0 to create a single slope PWM on OC0A with a frequency of 50 Hz and a duty cycle of 20%.
 *		   Set symbol EXAMPLE = 2.
 *		   f_Out = f_Clk / (N * (PER + 1))
 *		   CCx = PER * Duty
 *		3) Use Timer 0 to create a dual slope PWM on OC0A with a frequency of 50 Hz and a duty cycle of 20%.
 *		   Use the overflow interrupt to toggle a GPIO with f_Out / 2. Set symbol EXAMPLE = 3.
 *		   f_Out = f_Clk / (2 * N * PER)
 *		   CCx = PER * Duty
 *		4) Use Timer 0 of Port D to create a signal and Timer 0 of Port C for a frequency capture over the event system. 
 *		   NOTE: You have to bridge D.0 and C.7
 *		   Set symbol EXAMPLE = 4.
 *		5) Use Timer 0 for a pulse width measurement over the event system. Set symbol EXAMPLE = 5.
 *		6) Use Timer 0 for a frequency and duty cycle measurement over the event system. Set symbol EXAMPLE = 6.
 *
 *  Software for the XMega TWI tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-timer/
 *
 *  @author Daniel Kampert
 */

#define F_CPU						2000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t Frequency_Cap = 0x00;
float Frequency = 0x00;

volatile uint16_t PulseWidth_Cap = 0x00;
float PulseWidth = 0.0;
float DutyCycle = 0.0;

int main(void)
{
	#if(EXAMPLE == 1)
		/*
			Initialize the LED
				-> Output
				-> PR0 (LED0 active low)
		*/
		PORTR.DIRSET = (0x01 << 0x00);
		PORTR.OUTCLR = (0x01 << 0x00);
		
		/*
			Initialize C.0
				-> Output
		*/
		PORTC.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCC0
				-> Waveform mode 0
				-> Clock Prescaler 64
				-> Overflow interrupt (low level)
				-> PER = 31249 @ 2 MHz
		*/
		TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
		TCC0.PER = 31249;
		
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
		
	#elif(EXAMPLE == 2)
		/*
			Initialize OC0A pin
				-> Output
				-> Pin 0
		*/
		PORTC.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCC0
				-> Waveform mode 3
				-> Use CCA
				-> Clock Prescaler 64
				-> PER = 624 @ 2 MHz
				-> CCA = 125 @ 2 MHz
		*/
		TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCC0.PER = 624;
		TCC0.CCA = 125;	

	#elif(EXAMPLE == 3)
		/*
			Initialize C.5
				-> Output
		*/
		PORTC.DIRSET = (0x01 << 0x05);
		
		/*
			Initialize OC0A pin
				-> Output
				-> Pin 0
		*/
		PORTC.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCC0
				-> Waveform mode 5
				-> Clock Prescaler 64
				-> PER = 312 @ 2 MHz
				-> CCA = 62 @ 2 MHz
				-> Overflow interrupt (low level)
		*/
		TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_DSTOP_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
		TCC0.PER = 312;
		TCC0.CCA = 62;
	
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
	
	#elif(EXAMPLE == 4)
		/*
			Initialize OC0A pin
				-> Output
				-> Pin 0
		*/
		PORTD.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCD0
				-> Waveform mode 3
				-> Clock Prescaler 64
				-> PER = 947 @ 2 MHz
				-> CCA = PER / 2
		*/
		TCD0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;
		TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCD0.PER = 947;
		TCD0.CCA = TCD0.PER >> 0x01;	
	
		/*
			Initialize C.7
				-> Input
				-> Falling edge detection
		*/
		PORTC.DIRCLR = (0x01 << 0x07);
		PORTC.PIN7CTRL = PORT_ISC_RISING_gc;
		
		/*
			Initialize event system
				-> Channel 0
				-> Use pin C.7 as input for the event system
		*/
		EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN7_gc;
		
		/*
			Initialize TCC0
				-> Frequency capture
				-> Channel 0
				-> Waveform mode 0
				-> Clock Prescaler 1
				-> CCA
		*/
		TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_NORMAL_gc;
		TCC0.CTRLD = TC_EVACT_FRQ_gc | TC_EVSEL_CH0_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
	
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
		
	#elif(EXAMPLE == 5)
		/*
			Initialize OC0A pin
				-> Output
				-> Pin 0
		*/
		PORTD.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCD0
				-> Waveform mode 3
				-> Clock Prescaler 64
				-> PER = 624 @ 2 MHz
				-> CCA = 312
		*/
		TCD0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;
		TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCD0.PER = 624;
		TCD0.CCA = 312;	
	
		/*
			Initialize C.7
				-> Input
				-> Falling edge detection
		*/
		PORTC.DIRCLR = (0x01 << 0x07);
		PORTC.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
		
		/*
			Initialize event system
				-> Channel 0
				-> Use pin C.7 as input for the event system
		*/
		EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN7_gc;
		
		/*
			Initialize TCC0
				-> Waveform mode 0
				-> Clock Prescaler 1
				-> CCA
				-> pulse width capture
				-> Channel 0
		*/
		TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_NORMAL_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		TCC0.CTRLD = TC_EVACT_PW_gc | TC_EVSEL_CH0_gc;
		TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
	
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
	
	#elif(EXAMPLE == 6)

		/*
			Initialize OC0A pin
				-> Output
				-> Pin 0
		*/
		PORTD.DIRSET = (0x01 << 0x00);
		
		/*
			Initialize TCC0
				-> Waveform mode 3
				-> Clock Prescaler 64
				-> PER = 624 @ 2 MHz
				-> CCA = 124 @ 2 MHz
		*/
		TCD0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;
		TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
		TCD0.PER = 624;
		TCD0.CCA = 124;	
	
		/*
			Initialize C.7
				-> Input
				-> Falling edge detection
		*/
		PORTC.DIRCLR = (0x01 << 0x07);
		PORTC.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
		
		/*
			Initialize event system
				-> Channel 0
				-> Use pin C.7 as input for the event system
		*/
		EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN7_gc;
		
		/*
			Initialize TCC0
				-> Waveform mode 0
				-> Clock Prescaler 2
				-> CCA
				-> Input capture
				-> Channel 0
		*/
		TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_NORMAL_gc;
		TCC0.CTRLA = TC_CLKSEL_DIV2_gc;
		TCC0.CTRLD = TC_EVACT_CAPT_gc | TC_EVSEL_CH0_gc;
		TCC0.PER = 0x7FFF;
		TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
	
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
		#if(EXAMPLE == 4)
			Frequency = (float)F_CPU / Frequency_Cap;
			
		#elif(EXAMPLE == 5)
			// Get the pulse width (in seconds)
			PulseWidth = 1.0 / F_CPU * PulseWidth_Cap;
		
		#elif(EXAMPLE == 6)
			DutyCycle = (float)PulseWidth_Cap / (float)Frequency_Cap;
			Frequency = ((float)F_CPU / 2 / Frequency_Cap);
	
		#endif
		
		asm volatile (
			"nop"
		);
    }
}

ISR(TCC0_OVF_vect)
{
	#if(EXAMPLE == 1)
		// Toggle LED0
		PORTR.OUTTGL = (0x01 << 0x00);
	
		// Toggle clock output
		PORTC.OUTTGL = (0x01 << 0x00);
	
	#elif(EXAMPLE == 3)
		// Toggle output
		PORTC.OUTTGL = (0x01 << 0x05);
	
	#endif
}

ISR(TCC0_CCA_vect)
{
	#if(EXAMPLE == 4)
		Frequency_Cap = TCC0.CCA;

	#elif(EXAMPLE == 5)
		PulseWidth_Cap = TCC0.CCA;
	
	#elif(EXAMPLE == 6)
		uint16_t Capture = TCC0.CCA;
	
		if(Capture & 0x8000)
		{
			Frequency_Cap = Capture & 0x7FFF;
			TCC0.CTRLFSET = TC_CMD_RESTART_gc;
		}
		else
		{
			PulseWidth_Cap = Capture;
		}
	
	#endif
}

