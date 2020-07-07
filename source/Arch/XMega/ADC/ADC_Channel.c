/*
 * ADC_Channel.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega ADC channels.

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

/** @file Arch/XMega/ADC/ADC_Driver.c
 *  @brief Driver for Atmel AVR XMega ADC channels.
 *
 *  This file contains the implementation of the Atmel AVR XMega ADC channel driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/ADC/ADC.h"
#include "Arch/XMega/NVM/NVM.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	struct
	{
		ADC_Callback_t ConversionComplete;
		ADC_Callback_t ThreshBelow;
		ADC_Callback_t ThreshAbove;
	} ADC_Callbacks[ADC_DEVICES][ADC_CHANNEL];
#endif

/** @brief			ADC interrupt handler.
 *  @param Device	ADC device
 *  @param Channel	ADC channel
 */
static void _ADC_Channel_InterruptHandler(const uint8_t Device, const uint8_t Channel)
{	
	if(ADC_Callbacks[Device][Channel].ConversionComplete)
	{
		ADC_Callbacks[Device][Channel].ConversionComplete(Device, Channel);
	}
	else if(ADC_Callbacks[Device][Channel].ThreshAbove)
	{
		ADC_Callbacks[Device][Channel].ThreshAbove(Device, Channel);
	}
	else if(ADC_Callbacks[Device][Channel].ThreshBelow)
	{
		ADC_Callbacks[Device][Channel].ThreshBelow(Device, Channel);
	}
}

void ADC_Channel_Init(ADC_ChannelConfig_t* Config)
{
	if(Config->Device == &ADCA)
	{
		GPIO_SetDirection(&PORTA, Config->Input, GPIO_DIRECTION_IN);
		GPIO_DisableBuffer(&PORTA, Config->Input);
	}

	#if(defined ADCB)
		if(Config->Device == &ADCB)
		{
			GPIO_SetDirection(&PORTB, Config->Input, GPIO_DIRECTION_IN);
			GPIO_DisableBuffer(&PORTA, Config->Input);
		}
	#endif

	Config->Channel->CTRL |= (Config->Channel->CTRL & (~Config->Mode)) | Config->Mode;
	Config->Channel->CTRL |= (Config->Channel->CTRL & (~(Config->Gain << 0x02))) | (Config->Gain << 0x02);
	Config->Channel->MUXCTRL |= (Config->Channel->MUXCTRL & (~(Config->Input << 0x03))) | (Config->Input << 0x03);
}

void ADC_Channel_ChangeInterruptLevel(ADC_CH_t* Channel, const Interrupt_Level_t InterruptLevel)
{
	Channel->INTCTRL = (Channel->INTCTRL & (~0x03)) | InterruptLevel;
}

void ADC_Channel_InstallCallback(ADC_InterruptConfig_t* Config)
{
	uint8_t Device = ADCA_ID;
	uint8_t Channel = 0x00;

	#if(defined ADCB)
		if(Config->Device == &ADCB)
		{
			Device = ADCB_ID;
		}
	#endif

	if(Config->Channel == &Config->Device->CH0)
	{
		Channel = 0x00;
	}
	#if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		else if(Config->Channel == &Config->Device->CH1)
		{
			Channel = 0x01;
		}
		else if(Config->Channel == &Config->Device->CH2)
		{
			Channel = 0x02;
		}
		else if(Config->Channel == &Config->Device->CH3)
		{
			Channel = 0x03;
		}
	#endif

	if(Config->Source & ADC_CONVERSION_COMPLETE_INTERRUPT)
	{
		ADC_Callbacks[Device][Channel].ConversionComplete = Config->Callback;
	}

	if(Config->Source & ADC_THRESH_BELOW_INTERRUPT)
	{
		ADC_Callbacks[Device][Channel].ThreshBelow = Config->Callback;
	}

	if(Config->Source & ADC_THRESH_ABOVE_INTERRUPT)
	{
		ADC_Callbacks[Device][Channel].ThreshAbove = Config->Callback;
	}

	Config->Channel->INTCTRL |= (Config->Channel->INTCTRL & (~Config->InterruptLevel)) | Config->InterruptLevel;
	Config->Channel->INTCTRL |= (Config->Channel->INTCTRL & (~(Config->Source << 0x02))) | (Config->Source << 0x02);
}

void ADC_Channel_RemoveCallback(ADC_CH_t* Channel, const ADC_CallbackType_t Callback)
{
	Channel->INTCTRL &= ~(Callback << 0x02);
}

void ADC_Channel_StartConversion(ADC_CH_t* Channel)
{
	Channel->CTRL |= ADC_CH_START_bm;
}

uint16_t ADC_Channel_ReadResult(ADC_CH_t* Channel)
{
	while(!(Channel->INTFLAGS & ADC_CH_CHIF_bm));
	Channel->INTFLAGS = ADC_CH_CHIF_bm;

	return Channel->RES;
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(ADCA_CH0_vect)
	{
		_ADC_Channel_InterruptHandler(ADCA_ID, 0);
	}

	#if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		ISR(ADCA_CH1_vect)
		{
			_ADC_Channel_InterruptHandler(ADCA_ID, 1);
		}

		ISR(ADCA_CH2_vect)
		{
			_ADC_Channel_InterruptHandler(ADCA_ID, 2);
		}

		ISR(ADCA_CH3_vect)
		{
			_ADC_Channel_InterruptHandler(ADCA_ID, 3);
		}

		ISR(ADCB_CH0_vect)
		{
			_ADC_Channel_InterruptHandler(ADCB_ID, 0);
		}

		ISR(ADCB_CH1_vect)
		{
			_ADC_Channel_InterruptHandler(ADCB_ID, 1);
		}

		ISR(ADCB_CH2_vect)
		{
			_ADC_Channel_InterruptHandler(ADCB_ID, 2);
		}

		ISR(ADCB_CH3_vect)
		{
			_ADC_Channel_InterruptHandler(ADCB_ID, 3);
		}
	#endif
#endif