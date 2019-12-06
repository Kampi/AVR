/*
 * ADC.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 megaAVR ADC module.

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

/** @file Arch/AVR8/megaAVR/ADC/ADC.c
 *  @brief Driver for Atmel AVR8 megaAVR ADC module.
 *
 *  This file contains the implementation of the megaAVR ADC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/ADC/ADC.h"

#ifndef DOXYGEN
	ADC_Callback_t ConversionComplete;
#endif

/** @brief	ADC interrupt handler
 */
static void __ADC_InterruptHandler(void)
{	
	if(ConversionComplete)
	{
		ConversionComplete(0, 0);
	}
}

void ADC_Init(ADC_Config_t* Config)
{
	ADC_SetInput(Config->InputMode);
	ADC_SetFormat(Config->Format);
	ADC_SetReference(Config->Reference);
	ADC_SetPrescaler(Config->Prescaler);
	ADC_Enable();
}

void ADC_Channel_InstallCallback(ADC_Callback_t Callback)
{
	ConversionComplete = Callback;
	ADCSRA |= (0x01 << ADIE);
}
 
void ADC_Channel_RemoveCallback(void)
{
	ConversionComplete = NULL;
	ADCSRA &= ~(0x01 << ADIE);
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(ADC_vect)
	{
		__ADC_InterruptHandler();
	}
#endif