/*
 * AC.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega analog comparator

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

/** @file Arch/XMega/AC/AC.c
 *  @brief Driver for XMega AC module.
 * 
 *  This file contains the implementation of the XMega AC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/AC/AC.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	struct
	{
		AC_Callback_t AC0;
		AC_Callback_t AC1;
		AC_Callback_t Window;
	} AC_Callbacks[AC_DEVICES];
#endif

/** @brief			AC interrupt handler
 *  @param Device	AC device
 *  @param Callback	Type of interrupt
 */
static void __AC_InterruptHandler(const uint8_t Device, const AC_CallbackType_t Type)
{	
	if(Type == AC_COMP0_INTERRUPT)
	{
		if(AC_Callbacks[Device].AC0)
		{
			AC_Callbacks[Device].AC0();
		}
	}
	else if(Type == AC_COMP1_INTERRUPT)
	{
		if(AC_Callbacks[Device].AC1)
		{
			AC_Callbacks[Device].AC1();
		}		
	}
	else
	{
		if(AC_Callbacks[Device].Window)
		{
			AC_Callbacks[Device].Window();
		}		
	}	
}

void AC_Init(AC_Config_t* Config)
{
	AC_PowerEnable(Config->Device);
	AC_Calibrate(Config->Device);
	AC_SetSpeed(Config->Device, Config->Comparator, Config->EnableHighSpeed);
	AC_SetHysteresis(Config->Device, Config->Comparator, Config->Hysteresis);
	AC_SetMux(Config->Device, Config->Comparator, Config->Positive, Config->Negative);
	AC_Enable(Config->Device, Config->Comparator);
}

void AC_GetConfig(AC_Config_t* Config, AC_t* Device, AC_Comparator_t Comparator)
{
	Config->Device = Device;
	Config->Comparator = Comparator;
	Config->EnableHighSpeed = AC_GetSpeed(Config->Device, Config->Comparator);
	Config->Hysteresis = AC_GetHysteresis(Config->Device, Config->Comparator);
	Config->Negative = AC_GetMuxNegative(Config->Device, Config->Comparator);
	Config->Positive = AC_GetMuxPositive(Config->Device, Config->Comparator);
}

void AC_Calibrate(AC_t* Device)
{
	// ToDo
}

void AC_Enable(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_0)
	{
		Device->AC0CTRL |= 0x01;
	}
	
	if(Comparator & AC_COMPARATOR_1)
	{
		Device->AC1CTRL |= 0x01;
	}
}

void AC_Disable(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_0)
	{
		Device->AC0CTRL &= ~0x01;
	}
	
	if(Comparator & AC_COMPARATOR_1)
	{
		Device->AC1CTRL &= ~0x01;
	}
}

void AC_EnableOutput(AC_t* Device, AC_Comparator_t Comparator)
{	
	// Set the outputs
	if(Device == &ACB)
	{
		if(Comparator == AC_COMPARATOR_1)
		{
			GPIO_SetDirection(&PORTB, 6, GPIO_DIRECTION_OUT);
		}
		else
		{
			GPIO_SetDirection(&PORTB, 7, GPIO_DIRECTION_OUT);
		}
	}
	else
	{
		if(Comparator == AC_COMPARATOR_1)
		{
			GPIO_SetDirection(&PORTA, 6, GPIO_DIRECTION_OUT);
		}
		else
		{
			GPIO_SetDirection(&PORTA, 7, GPIO_DIRECTION_OUT);
		}
	}
	
	Device->CTRLA |= Comparator;
}

void AC_DisableOutput(AC_t* Device, AC_Comparator_t Comparator)
{
	Device->CTRLA &= ~Comparator;
}

void AC_SetSpeed(AC_t* Device, AC_Comparator_t Comparator, Bool_t Speed)
{	
	if(Comparator & AC_COMPARATOR_0)
	{
		Device->AC0CTRL = (Device->AC0CTRL & (~(0x01 << 0x02))) | (Speed << 0x02);
	}
	
	if(Comparator & AC_COMPARATOR_1)
	{
		Device->AC1CTRL = (Device->AC1CTRL & (~(0x01 << 0x02))) | (Speed << 0x02);
	}
}

Bool_t AC_GetSpeed(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		return ((Device->AC1CTRL & 0x08) >> 0x03);
	}
	
	return ((Device->AC1CTRL & 0x08) >> 0x03);
}

void AC_SetHysteresis(AC_t* Device, AC_Comparator_t Comparator, AC_Hysteresis_t Hysteresis)
{
	if(Comparator & AC_COMPARATOR_0)
	{
		Device->AC0CTRL = (Device->AC0CTRL & (~(0x03 << 0x01))) | (Hysteresis << 0x01);
	}
	
	if(Comparator & AC_COMPARATOR_1)
	{
		Device->AC1CTRL = (Device->AC1CTRL & (~(0x03 << 0x01))) | (Hysteresis << 0x02);
	} 
}

AC_Hysteresis_t AC_GetHysteresis(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		return ((Device->AC1CTRL & 0x06) >> 0x02);
	}
	
	return ((Device->AC1CTRL & 0x06) >> 0x02);
}

void AC_SetMux(AC_t* Device, AC_Comparator_t Comparator, AC_MuxP_t Positive, AC_MuxN_t Negative)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		Device->AC1MUXCTRL = (Device->AC1MUXCTRL & (~(Positive << 0x03))) | (Positive << 0x03);
		Device->AC1MUXCTRL = (Device->AC1MUXCTRL & (~Negative)) | Negative;
	}
	
	Device->AC0MUXCTRL = (Device->AC0MUXCTRL & (~(Positive << 0x03))) | (Positive << 0x03);
	Device->AC0MUXCTRL = (Device->AC0MUXCTRL & (~Negative)) | Negative;
} 

AC_MuxP_t AC_GetMuxPositive(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		return (Device->AC1MUXCTRL >> 0x03);
	}

	return (Device->AC0MUXCTRL >> 0x03);
}

AC_MuxN_t AC_GetMuxNegative(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		return (Device->AC1MUXCTRL & 0x07);
	}

	return (Device->AC1MUXCTRL & 0x07);
}

void AC_WaitForComparator(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_0)
	{
		while(!(Device->STATUS & AC_AC0STATE_bm));
	}
	
	if(Comparator & AC_COMPARATOR_1)
	{
		while(!(Device->STATUS & AC_AC1STATE_bm));
	}
}

void AC_InstallCallback(AC_InterruptConfig_t* Config)
{
	if(Config->Source & AC_COMP0_INTERRUPT)
	{
		Config->Device->AC0CTRL = (Config->Device->AC0CTRL & (~(0x03 << 0x06))) | (Config->Mode << 0x06);
		Config->Device->AC0CTRL = (Config->Device->AC0CTRL & (~(0x03 << 0x04))) | (Config->InterruptLevel << 0x04);
		
		if(Config->Device == &ACB)
		{
			AC_Callbacks[ACB_ID].AC0 = Config->Callback;
		}
		else
		{
			AC_Callbacks[ACA_ID].AC0 = Config->Callback;
		}
		
		Config->Device->STATUS |= AC_AC0IF_bm;
	}
	
	if(Config->Source & AC_COMP1_INTERRUPT)
	{
		Config->Device->AC1CTRL = (Config->Device->AC1CTRL & (~(0x03 << 0x06))) | (Config->Mode << 0x06);
		Config->Device->AC1CTRL = (Config->Device->AC1CTRL & (~(0x03 << 0x04))) | (Config->InterruptLevel << 0x04);
		
		if(Config->Device == &ACB)
		{
			AC_Callbacks[ACA_ID].AC1 = Config->Callback;
		}
		else
		{
			AC_Callbacks[ACA_ID].AC1 = Config->Callback;
		}

		Config->Device->STATUS |= AC_AC0IF_bm;
	}
	
	if(Config->Source & AC_WINDOW_INTERRUPT)
	{
		Config->Device->WINCTRL = (Config->Device->WINCTRL & (~(0x03 << 0x02))) | ((Config->Mode >> 0x01) << 0x02);
		Config->Device->WINCTRL = (Config->Device->WINCTRL & (~0x03)) | Config->InterruptLevel;
		
		if(Config->Device == &ACB)
		{
			AC_Callbacks[ACB_ID].Window = Config->Callback;
		}
		else
		{
			AC_Callbacks[ACA_ID].Window = Config->Callback;
		}
		
		Config->Device->STATUS |= AC_WIF_bm;
	}
}

void AC_RemoveCallback(AC_t* Device, AC_CallbackType_t Callback)
{
	if(Callback & AC_COMP0_INTERRUPT)
	{
		if(Device == &ACB)
		{
			AC_Callbacks[ACB_ID].AC0 = NULL;
		}
		else
		{
			AC_Callbacks[ACA_ID].AC0 = NULL;
		}
		
		Device->STATUS &= ~AC_AC0IF_bm;
	}
	
	if(Callback & AC_COMP1_INTERRUPT)
	{
		if(Device == &ACB)
		{
			AC_Callbacks[ACB_ID].AC1 = NULL;
		}
		else
		{
			AC_Callbacks[ACA_ID].AC1 = NULL;
		}

		Device->STATUS &= ~AC_AC0IF_bm;
	}
	
	else if(Callback & AC_WINDOW_INTERRUPT)
	{	
		if(Device == &ACB)
		{
			AC_Callbacks[ACB_ID].Window = NULL;
		}
		else
		{
			AC_Callbacks[ACA_ID].Window = NULL;
		}
		
		Device->STATUS &= ~AC_WIF_bm;
	}
}

void AC_ChangeInterruptLevel(AC_t* Device, AC_CallbackType_t Callback, Interrupt_Level_t InterruptLevel)
{
	if(Callback == AC_COMP0_INTERRUPT)
	{
		Device->AC0CTRL = (Device->AC0CTRL & (~(0x03 << 0x04))) | (InterruptLevel << 0x04);
	}
	else if(Callback == AC_COMP1_INTERRUPT)
	{
		Device->AC1CTRL = (Device->AC1CTRL & (~(0x03 << 0x04))) | (InterruptLevel << 0x04);
	}
	else if(Callback == AC_WINDOW_INTERRUPT)
	{
		Device->WINCTRL = (Device->WINCTRL & (~0x03)) | InterruptLevel;
	}
}

uint8_t AC_GetState(AC_t* Device, AC_Comparator_t Comparator)
{
	if(Comparator & AC_COMPARATOR_1)
	{
		return ((Device->STATUS & (0x01 << 0x05)) >> 0x05);
	}
	
	return ((Device->STATUS & (0x01 << 0x04)) >> 0x04);
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(ACA_AC0_vect)
	{
		__AC_InterruptHandler(ACA_ID, AC_COMP0_INTERRUPT);
	}

	ISR(ACA_AC1_vect)
	{
		__AC_InterruptHandler(ACA_ID, AC_COMP1_INTERRUPT);
	}

	ISR(ACA_ACW_vect)
	{
		__AC_InterruptHandler(ACA_ID, AC_WINDOW_INTERRUPT);
	}

	ISR(ACB_AC0_vect)
	{
		__AC_InterruptHandler(ACB_ID, AC_COMP0_INTERRUPT);
	}

	ISR(ACB_AC1_vect)
	{
		__AC_InterruptHandler(ACB_ID, AC_COMP1_INTERRUPT);
	}

	ISR(ACB_ACW_vect)
	{
		__AC_InterruptHandler(ACB_ID, AC_WINDOW_INTERRUPT);
	}
#endif