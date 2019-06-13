/*
 * DAC.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega DAC

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

/** @file Arch/XMega/DAC/DAC.c
 *  @brief Driver for XMega DAC module.
 *
 *  This file contains the implementation of the XMega DAC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/NVM/NVM.h"
#include "Arch/XMega/DAC/DAC.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	static float DAC_VoltagePerStep;
#endif

void DAC_Init(DAC_Config_t* Config)
{
	DAC_PowerEnable(Config->Device);
	DAC_EnableChannel(Config->Device, Config->Channel);
	DAC_SetOutputConfig(Config->Device, Config->OutputConfig);
	DAC_SetReference(Config->Device, Config->Reference);
	DAC_SetAdjustment(Config->Device, Config->Adjustment);
	DAC_Calibrate(Config->Device, Config->Channel);
	DAC_Enable(Config->Device);
}

void DAC_Calibrate(DAC_t* Device, DAC_Channel_t Channel)
{
	NVM.CMD  = NVM_CMD_READ_CALIB_ROW_gc;
	
	if(Device == &DACB)
	{
		if(Channel & DAC_CHANNEL_0)
		{
			Device->CH0OFFSETCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACB0OFFCAL));
			Device->CH0GAINCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACB0GAINCAL));
		}
		
		if(Channel & DAC_CHANNEL_1)
		{
			Device->CH1OFFSETCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACB1OFFCAL));
			Device->CH1GAINCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACB1GAINCAL));
		}
	}
	#ifdef DACA
		else if(Device == &DACA)
		{
			if(Channel & DAC_CHANNEL_0)
			{
				Device->CH0OFFSETCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACA0OFFCAL));
				Device->CH0GAINCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACA0GAINCAL));
			}
			
			if(Channel & DAC_CHANNEL_1)
			{
				Device->CH1OFFSETCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACA1OFFCAL));
				Device->CH1GAINCAL = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, DACA1GAINCAL));
			}
		}
	#endif
}

void DAC_EnableLowPower(DAC_t* Device)
{
	Device->CTRLA |= (0x01 << 0x01);
}

void DAC_DisableLowPower(DAC_t* Device)
{
	Device->CTRLA &= ~(0x01 << 0x01);
}

void DAC_GetConfig(DAC_Config_t* Config, DAC_t* Device)
{
	Config->Device = Device;
	Config->Channel = DAC_GetChannel(Device);
	Config->OutputConfig = DAC_GetOutputConfig(Device);
	Config->Reference = DAC_GetReference(Device);
	Config->Adjustment = DAC_GetAdjustment(Device);
}

void DAC_Enable(DAC_t* Device)
{
	Device->CTRLA |= DAC_ENABLE_bm;
}

void DAC_Disable(DAC_t* Device)
{
	Device->CTRLA &= ~DAC_ENABLE_bm;
}

void DAC_EnableChannel(DAC_t* Device, const DAC_Channel_t Channel)
{
	if(Channel & DAC_CHANNEL_0)
	{
		Device->CTRLA |= (0x01 << 0x02);
	}
	else if(Channel & DAC_CHANNEL_1)
	{
		Device->CTRLA |= (0x01 << 0x03);
	}
	else if(Channel & DAC_CHANNEL_INT)
	{
		Device->CTRLA &= ~(0x01 << 0x03);
		Device->CTRLA |= (0x01 << 0x04) | (0x01 << 0x02);
	}
}

void DAC_DisableChannel(DAC_t* Device, const DAC_Channel_t Channel)
{
	if(Channel & DAC_CHANNEL_0)
	{
		Device->CTRLA &= ~(0x01 << 0x02);
	}
	
	if(Channel & DAC_CHANNEL_1)
	{
		Device->CTRLA &= ~(0x01 << 0x03);
	}
	
	if(Channel & DAC_CHANNEL_INT)
	{
		Device->CTRLA &= ~(0x01 << 0x04);
	}
}

DAC_Channel_t DAC_GetChannel(DAC_t* Device)
{
	return ((Device->CTRLA & 0x1C) >> 0x02);
}

void DAC_SetOutputConfig(DAC_t* Device, const DAC_OutputConfig_t Config)
{
	Device->CTRLB = (Device->CTRLC & (~Config)) | Config;
}

DAC_OutputConfig_t DAC_GetOutputConfig(DAC_t* Device)
{
	return (Device->CTRLB & 0x03);
}

void DAC_SetReference(DAC_t* Device, const DAC_Reference_t Reference)
{
	uint16_t ReferenceVoltage = 0x00;
	
	Device->CTRLC = (Device->CTRLC & (~(Reference << 0x03))) | (Reference << 0x03);
	
	if(Reference != DAC_REFERENCE_INT1V)
	{
		ReferenceVoltage = REFERENCE_VOLTAGE;
	}
	else
	{
		ReferenceVoltage = 1000.0;
	}

	DAC_VoltagePerStep = ((float)ReferenceVoltage / (0x01 << DAC_RESOLUTION)) / 1000.0;	
}

DAC_Reference_t DAC_GetReference(DAC_t* Device)
{
	return ((Device->CTRLC & 0x18) >> 0x03);
}

void DAC_SetAdjustment(DAC_t* Device, const DAC_Adjustment_t Adjustment)
{
	Device->CTRLC = (Device->CTRLC & (~Adjustment)) | Adjustment;
}

DAC_Adjustment_t DAC_GetAdjustment(DAC_t* Device)
{
	return Device->CTRLC & 0x01;
}
 
void DAC_WriteChannel(DAC_t* Device, const DAC_Channel_t Channel, const uint16_t Value)
{
	if((Channel & DAC_CHANNEL_0) || (Channel & DAC_CHANNEL_INT))
	{
		while(!(Device->STATUS & DAC_CH0DRE_bm));
		Device->CH0DATA = Value;
	}
	
	if(Channel & DAC_CHANNEL_1)
	{
		while(!(Device->STATUS & DAC_CH1DRE_bm));
		Device->CH1DATA = Value;
	}
}

void DAC_WriteVoltage(DAC_t* Device, const DAC_Channel_t Channel, const float Voltage)
{
	DAC_WriteChannel(Device, Channel, (Voltage / DAC_VoltagePerStep) - 0x01);
}

void DAC_ConfigEvent(DAC_t* Device, const DAC_EventChannel_t EventChannel, const DAC_Channel_t Channel)
{
	if(Channel & DAC_CHANNEL_0)
	{
		Device->EVCTRL = 0x00;
	}
	
	if(Channel & DAC_CHANNEL_1)
	{
		Device->EVCTRL = (0x01 << 0x03);
	}

	Device->EVCTRL |= EventChannel & 0x07;
}