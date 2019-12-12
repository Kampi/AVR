/*
 * ADC.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AT90 ADC.

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

/** @file ADC/ADC.c
 *  @brief Driver for Atmel AT90 ADC.
 *
 *  This file contains the implementation of the AT90 ADC driver.
 *
 *  @author Daniel Kampert
 */

#include "ADC/ADC.h"

static ADC_Callback_t __ADC_Callback;

void ADC_Init(ADC_Callback_t Callback)
{
	ADMUX = (0x01 << REFS1) | (0x01 << REFS0);
	ADCSRA = (0x01 << ADEN) | (0x01 << ADSC) | (0x01 << ADATE) | (0x01 << ADIE) | (0x01 << ADPS2) | (0x01 << ADPS1) | (0x01 << ADPS0);

	__ADC_Callback = Callback;
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(ADC_vect)
	{
		if(__ADC_Callback != NULL)
		{
			__ADC_Callback(ADMUX & 0x07, ADC);
		}
	}
#endif
