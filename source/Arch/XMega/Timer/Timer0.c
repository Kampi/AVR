/*
 * Timer0.c
 *
 * Created: 11.05.2017 21:33:35
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the Atmel AVR XMega Timer type 0.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Arch/XMega/Timer/Timer0.c
 *  @brief Driver for the Atmel AVR XMega Timer type 0.
 *
 *  This contains the implementation of the Atmel AVR XMega Timer0 driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/Timer/Timer.h"

#ifndef DOXYGEN
	struct
	{
		Timer_Callback_t OverflowCallback;
		Timer_Callback_t ErrorCallback;
		Timer_Callback_t CCACallback;
		Timer_Callback_t CCBCallback;
		Timer_Callback_t CCCCallback;
		Timer_Callback_t CCDCallback;
	} Timer0_Callbacks[TIMER0_DEVICES];
#endif

/** @brief				Timer0 interrupt handler.
 *  @param Device		Device ID
 *  @param Callback		Type of interrupt
 */
static void _Timer0_InterruptHandler(const uint8_t Device, const Timer_CallbackType_t Callback)
{
	switch(Callback)
	{
		case(TIMER_OVERFLOW_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].OverflowCallback)
			{
				Timer0_Callbacks[Device].OverflowCallback();
			}
			
			break;
		}
		case(TIMER_ERROR_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].ErrorCallback)
			{
				Timer0_Callbacks[Device].ErrorCallback();
			}
			
			break;
		}
		case(TIMER_CCA_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].CCACallback)
			{
				Timer0_Callbacks[Device].CCACallback();
			}
			
			break;
		}
		case(TIMER_CCB_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].CCBCallback)
			{
				Timer0_Callbacks[Device].CCBCallback();
			}
			
			break;
		}
		case(TIMER_CCC_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].CCCCallback)
			{
				Timer0_Callbacks[Device].CCCCallback();
			}
			
			break;
		}
		case(TIMER_CCD_INTERRUPT):
		{
			if(Timer0_Callbacks[Device].CCDCallback)
			{
				Timer0_Callbacks[Device].CCDCallback();
			}
			
			break;
		}
		default:
		{
			break;
		}
	}
}

void Timer0_Init(Timer0_Config_t* Config)
{	
	Timer0_PowerEnable(Config->Device);
	Timer0_SetMode(Config->Device, TIMER_MODE_0);
	Timer0_SetPeriod(Config->Device, Config->Period);
	Timer0_SetPrescaler(Config->Device, Config->Prescaler);
}

void Timer0_WaveInit(Timer0_WaveConfig_t* Config)
{
	uint8_t Pin = Config->Channel & 0x0F;
	
	Timer0_PowerEnable(Config->Device);
	Timer0_SetMode(Config->Device, Config->Mode);
	Timer0_SetPrescaler(Config->Device, Config->Prescaler);
	
	if(Config->Device == &TCC0)
	{
		GPIO_SetDirection(&PORTC, Pin, GPIO_DIRECTION_OUT);
		GPIO_SwitchInvert(&PORTC, Pin, Config->EnableInvert);
	}
	else if(Config->Device == &TCD0)
	{
		GPIO_SetDirection(&PORTD, Pin, GPIO_DIRECTION_OUT);
		GPIO_SwitchInvert(&PORTD, Pin, Config->EnableInvert);
	}
	else if(Config->Device == &TCE0)
	{
		GPIO_SetDirection(&PORTE, Pin, GPIO_DIRECTION_OUT);
		GPIO_SwitchInvert(&PORTE, Pin, Config->EnableInvert);
	}
	else if(Config->Device == &TCF0)
	{
		GPIO_SetDirection(&PORTF, Pin, GPIO_DIRECTION_OUT);
		GPIO_SwitchInvert(&PORTF, Pin, Config->EnableInvert);
	}

	Timer0_EnableCC(Config->Device, Config->Channel);

	if(Config->Mode == TIMER_MODE_1)
	{
		Timer0_SetCC(Config->Device, TIMER_CCA, Config->Period);
	}
	else
	{
		Timer0_SetPeriod(Config->Device, Config->Period);
		Timer0_SetCC(Config->Device, Config->Channel, Config->DutyCycle);
	}
}

void Timer0_CaptureInit(Timer0_CaptureConfig_t* Config)
{
	GPIO_SetDirection(Config->Port, Config->Pin, GPIO_DIRECTION_IN);
	Event_SetPinSource(Config->EChannel, Config->Port, Config->Pin);
	
	Timer0_SetMode(Config->Device, TIMER_MODE_0);
	Timer0_EnableCC(Config->Device, Config->Channel);
	Timer0_SetEventChannel(Config->Device, Config->EChannel);

	Timer0_SetCaptureMode(Config->Device, Config->Mode);

	if(Config->Mode == TIMER_FRQ_CAPTURE)
	{
		GPIO_SetInputSense(Config->Port, Config->Pin, GPIO_SENSE_RISING);
	}
	else if(Config->Mode == TIMER_PW_CAPTURE)
	{
		GPIO_SetInputSense(Config->Port, Config->Pin, GPIO_SENSE_BOTH);
	}
	else if(Config->Mode == TIMER_INPUT_CAPTURE)
	{
		Timer0_SetPeriod(Config->Device, 0x7FFF);
		//GPIO_SetInputSense(Config->Port, Config->Pin, Config->Sense);
	}
	
	Timer0_SetPrescaler(Config->Device, Config->Prescaler);
}

void Timer0_InstallCallback(Timer0_InterruptConfig_t* Config)
{
	uint8_t Device = 0x00;
	
	if(Config->Device == &TCD0)
	{
		Device = 0x01;
	}
	else if(Config->Device == &TCE0)
	{
		Device = 0x02;
	}
	else if(Config->Device == &TCF0)
	{
		Device = 0x03;
	}
	else
	{
		Device = 0x00;
	}
	
	if(Config->Source & TIMER_OVERFLOW_INTERRUPT)
	{
		Timer0_Callbacks[Device].OverflowCallback = Config->Callback;
		Config->Device->INTCTRLA = (Config->Device->INTCTRLA & ~0x03) | Config->InterruptLevel;
	}
	
	if(Config->Source & TIMER_ERROR_INTERRUPT)
	{
		Timer0_Callbacks[Device].ErrorCallback = Config->Callback;
		Config->Device->INTCTRLA = (Config->Device->INTCTRLA & ~(0x03 << 0x02)) | (Config->InterruptLevel << 0x02);
	}
	
	if(Config->Source & TIMER_CCA_INTERRUPT)
	{
		Timer0_Callbacks[Device].CCACallback = Config->Callback;
		Config->Device->INTCTRLB = (Config->Device->INTCTRLB & ~0x03) | Config->InterruptLevel;
	}
	
	if(Config->Source & TIMER_CCB_INTERRUPT)
	{
		Timer0_Callbacks[Device].CCBCallback = Config->Callback;
		Config->Device->INTCTRLB = (Config->Device->INTCTRLA & ~(0x03 << 0x02)) | (Config->InterruptLevel << 0x02);
	}
	
	if(Config->Source & TIMER_CCC_INTERRUPT)
	{
		Timer0_Callbacks[Device].CCCCallback = Config->Callback;
		Config->Device->INTCTRLB = (Config->Device->INTCTRLA & ~(0x03 << 0x04)) | (Config->InterruptLevel << 0x04);
	}
	
	if(Config->Source & TIMER_CCD_INTERRUPT)
	{
		Timer0_Callbacks[Device].CCDCallback = Config->Callback;
		Config->Device->INTCTRLB = (Config->Device->INTCTRLA & ~(0x03 << 0x06)) | (Config->InterruptLevel << 0x06);
	}
}

void Timer0_RemoveCallback(TC0_t* Device, const Timer_CallbackType_t Callback)
{
	if(Callback & TIMER_OVERFLOW_INTERRUPT)
	{
		Device->INTCTRLA &= ~(0x03 << 0x02);
	}
	
	if(Callback & TIMER_ERROR_INTERRUPT)
	{
		Device->INTCTRLA &= ~0x03;
	}
	
	if(Callback & TIMER_CCA_INTERRUPT)
	{
		Device->INTCTRLB &= ~0x03;
	}
	
	if(Callback & TIMER_CCB_INTERRUPT)
	{
		Device->INTCTRLA &= ~(0x03 << 0x02);
	}
	
	if(Callback & TIMER_CCC_INTERRUPT)
	{
		Device->INTCTRLB &= ~(0x03 << 0x04);
	}
	
	if(Callback & TIMER_CCD_INTERRUPT)
	{
		Device->INTCTRLB &= ~(0x03 << 0x06);
	}
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TCC0_ERR_vect)
	{
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCD0_ERR_vect)
	{
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCE0_ERR_vect)
	{
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCF0_ERR_vect)
	{
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_ERROR_INTERRUPT);
	}

	ISR(TCC0_OVF_vect)
	{
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCD0_OVF_vect)
	{
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCE0_OVF_vect)
	{
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCF0_OVF_vect)
	{
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_OVERFLOW_INTERRUPT);
	}

	ISR(TCC0_CCA_vect)
	{
		Timer0_ClearFlag(&TCC0, TIMER_CCA_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_CCA_INTERRUPT);
	}

	ISR(TCC0_CCB_vect)
	{
		Timer0_ClearFlag(&TCC0, TIMER_CCB_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_CCB_INTERRUPT);
	}

	ISR(TCC0_CCC_vect)
	{
		Timer0_ClearFlag(&TCC0, TIMER_CCC_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_CCC_INTERRUPT);
	}

	ISR(TCC0_CCD_vect)
	{
		Timer0_ClearFlag(&TCC0, TIMER_CCD_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0C_ID, TIMER_CCD_INTERRUPT);
	}

	ISR(TCD0_CCA_vect)
	{
		Timer0_ClearFlag(&TCD0, TIMER_CCA_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_CCA_INTERRUPT);
	}

	ISR(TCD0_CCB_vect)
	{
		Timer0_ClearFlag(&TCD0, TIMER_CCB_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_CCB_INTERRUPT);
	}

	ISR(TCD0_CCC_vect)
	{
		Timer0_ClearFlag(&TCD0, TIMER_CCC_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_CCC_INTERRUPT);
	}

	ISR(TCD0_CCD_vect)
	{
		Timer0_ClearFlag(&TCD0, TIMER_CCD_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0D_ID, TIMER_CCD_INTERRUPT);
	}

	ISR(TCE0_CCA_vect)
	{
		Timer0_ClearFlag(&TCE0, TIMER_CCA_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_CCA_INTERRUPT);
	}

	ISR(TCE0_CCB_vect)
	{
		Timer0_ClearFlag(&TCE0, TIMER_CCB_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_CCB_INTERRUPT);
	}

	ISR(TCE0_CCC_vect)
	{
		Timer0_ClearFlag(&TCE0, TIMER_CCC_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_CCC_INTERRUPT);
	}

	ISR(TCE0_CCD_vect)
	{
		Timer0_ClearFlag(&TCE0, TIMER_CCD_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0E_ID, TIMER_CCD_INTERRUPT);
	}

	ISR(TCF0_CCA_vect)
	{
		Timer0_ClearFlag(&TCF0, TIMER_CCA_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_CCA_INTERRUPT);
	}

	ISR(TCF0_CCB_vect)
	{
		Timer0_ClearFlag(&TCF0, TIMER_CCB_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_CCB_INTERRUPT);
	}

	ISR(TCF0_CCC_vect)
	{
		Timer0_ClearFlag(&TCF0, TIMER_CCC_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_CCC_INTERRUPT);
	}

	ISR(TCF0_CCD_vect)
	{
		Timer0_ClearFlag(&TCF0, TIMER_CCD_INTERRUPT);
		_Timer0_InterruptHandler(TIMER0F_ID, TIMER_CCD_INTERRUPT);
	}
#endif