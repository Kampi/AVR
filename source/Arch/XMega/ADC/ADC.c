/*
 * ADC.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega ADC module.

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

/** @file Arch/XMega/ADC/ADC.c
 *  @brief Driver for Atmel AVR XMega ADC module.
 *
 *  This file contains the implementation of the Atmel AVR XMega ADC driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/ADC/ADC.h"
#include "Arch/XMega/NVM/NVM.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

void ADC_Init(ADC_Config_t* Config)
{
	ADC_PowerEnable(Config->Device);
	ADC_SetConversionMode(Config->Device, Config->ConversionMode);
	ADC_SetPrescaler(Config->Device, Config->Prescaler);
	ADC_SetFormat(Config->Device, Config->Format);
	ADC_SetReference(Config->Device, Config->Reference);

	#if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		ADC_SetDMARequest(Config->Device, Config->DMARequest);
	#endif

	ADC_Calibrate(Config->Device);

	#if(MCU_NAME == MCU_NAME_ATXMEGA384C3)
		ADC_SetSamplingTime(Config->Device, Config->SamplingTime);
	#endif

	ADC_Enable(Config->Device);
	ADC_Flush(Config->Device);
}

void ADC_ConfigEvent(ADC_t* Device, const ADC_EventChannel_t Channel, const ADC_EventMode_t Mode)
{
	Device->EVCTRL = (Device->EVCTRL & (~(0x07 << 0x03))) | (Channel << 0x03);
	Device->EVCTRL = (Device->EVCTRL & (~0x07)) | Mode;
}

void ADC_ConfigSweep(ADC_t* Device, const ADC_Sweep_t SweepOption)
{
	Device->EVCTRL = (Device->EVCTRL & (~(SweepOption << 0x06))) | (SweepOption << 0x06);
}

void ADC_EnableFreeRun(ADC_t* Device)
{
	Device->CTRLB |= ADC_FREERUN_bm;
}

void ADC_DisableFreeRun(ADC_t* Device)
{
	Device->CTRLB &= ~ADC_FREERUN_bm;
}

void ADC_Calibrate(ADC_t* Device)
{
	uint8_t OffsetL = 0x00;
	uint8_t OffsetH = 0x00;	

	if(Device == &ADCA)
	{
		OffsetL = offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0);
		OffsetH = offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1);
	}
	else
	{
		#if(defined ADCB)
			OffsetL = offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL0);
			OffsetH = offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL1);
		#endif
	}

	Device->CALL = NVM_ReadCalibrationByte(OffsetL);
	Device->CALH = NVM_ReadCalibrationByte(OffsetH);
}