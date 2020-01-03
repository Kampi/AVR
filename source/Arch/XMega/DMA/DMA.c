/*
 * DMA.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega DMA controller.

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

/** @file Arch/XMega/DMA/DMA.c
 *  @brief Driver for Atmel AVR XMega DMA controller.
 *
 *  This file contains the implementation of the Atmel AVR XMega DMA driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/DMA/DMA.h"

#ifndef DOXYGEN
	static struct
	{
		DMA_Callback_t Error;
		DMA_Callback_t TransactionComplete;
	} _DMA_Callbacks[DMA_CHANNEL];
#endif

/** @brief			DMA interrupt handler
 *  @param Channel	DMA channel
 */
static void _DMA_Channel_InterruptHandler(const uint8_t Channel)
{	
	uint8_t Status = DMA_ReadStatus();
	
	// Check for transaction complete interrupt
	if(Status & (0x01 << Channel))
	{
		if(_DMA_Callbacks[Channel].TransactionComplete)
		{
			DMA_WriteStatus(Status | (0x01 << Channel));
			_DMA_Callbacks[Channel].TransactionComplete(Channel);
		}
	}
	// Check for error interrupt
	else if((Status >> 0x04) & (0x01 << Channel))
	{
		if(_DMA_Callbacks[Channel].Error)
		{
			DMA_WriteStatus(Status | (0x0A << Channel));
			_DMA_Callbacks[Channel].Error(Channel);
		}
	}
}

void DMA_Channel_Reset(DMA_CH_t* Channel)
{
	DMA_Channel_Disable(Channel);

	Channel->CTRLA |= DMA_CH_RESET_bm;
	
	DMA_Channel_Enable(Channel);
}

void DMA_Channel_Enable(DMA_CH_t* Channel)
{
	Channel->CTRLA |= DMA_CH_ENABLE_bm;
}

void DMA_Channel_SwitchRepeatMode(DMA_CH_t* Channel, Bool_t RepeatMode)
{
	if(RepeatMode)
	{
		Channel->CTRLA |= DMA_CH_REPEAT_bm;
	}
	else
	{
		Channel->CTRLA &= ~DMA_CH_REPEAT_bm;
	}
}

void DMA_Channel_Disable(DMA_CH_t* Channel)
{
	Channel->CTRLA &= ~DMA_CH_ENABLE_bm;
}

void DMA_Channel_ChangeInterruptLevel(DMA_CH_t* Channel, DMA_CallbackType_t Callback, Interrupt_Level_t InterruptLevel)
{
	if(Callback && DMA_TRANSACTION_INTERRUPT)
	{
		Channel->CTRLB = (Channel->CTRLB & (~InterruptLevel)) | InterruptLevel;
	}
	
	if(Callback && DMA_ERROR_INTERRUPT)
	{
		Channel->CTRLB = (Channel->CTRLB & (~(InterruptLevel << 0x02))) | (InterruptLevel << 0x02);
	}
}

void DMA_Channel_InstallCallback(DMA_InterruptConfig_t* Config)
{
	uint8_t Channel = 0x00;
	
	if(Config->Channel == &DMA.CH0)
	{
		Channel = 0x00;
	}
	#if(DMA_CHANNEL > 2)
		else if(Config->Channel == &DMA.CH1)
		{
			Channel = 0x01;
		}
	#elif(DMA_CHANNEL > 2)
		else if(Config->Channel == &DMA.CH2)
		{
			Channel = 0x02;
		}
	#elif(DMA_CHANNEL > 3)
		else if(Config->Channel == &DMA.CH3)
		{
			Channel = 0x03;
		}
	#endif
	
	if(Config->Source & DMA_TRANSACTION_INTERRUPT)
	{
		Config->Channel->CTRLB |= (Config->Channel->CTRLB & (~Config->InterruptLevel)) | Config->InterruptLevel;
		_DMA_Callbacks[Channel].TransactionComplete = Config->Callback;
	}
	
	if(Config->Source & DMA_ERROR_INTERRUPT)
	{
		Config->Channel->CTRLB |= (Config->Channel->CTRLB & (~(Config->InterruptLevel << 0x02))) | (Config->InterruptLevel << 0x02);
		_DMA_Callbacks[Channel].Error = Config->Callback;
	}
}

void DMA_Channel_RemoveCallback(DMA_CH_t* Channel, DMA_CallbackType_t Callback)
{
	Channel->CTRLB &= ~(Callback << 0x02);
}

void DMA_Channel_Config(DMA_TransferConfig_t* Config)
{
	DMA_Channel_SwitchSingleShot(Config->Channel, Config->EnableSingleShot);
	DMA_Channel_SwitchRepeatMode(Config->Channel, Config->EnableRepeatMode);
	DMA_Channel_SetBurstLength(Config->Channel, Config->BurstLength);
	DMA_Channel_SetTransferCount(Config->Channel, Config->TransferCount);
	DMA_CHannel_SetRepeatCount(Config->Channel, Config->RepeatCount);
	DMA_Channel_SetSrcReloadMode(Config->Channel, Config->SrcReload);
	DMA_Channel_SetDestReloadMode(Config->Channel, Config->DstReload);
	DMA_Channel_SetDestAddressingMode(Config->Channel, Config->DstAddrMode);
	DMA_Channel_SetSrcAddressingMode(Config->Channel, Config->SrcAddrMode);
	DMA_Channel_SetSrcAddress(Config->Channel, Config->SrcAddress);
	DMA_Channel_SetDestAddress(Config->Channel, Config->DstAddress);
	DMA_Channel_SetTriggerSource(Config->Channel, Config->TriggerSource);
}

void DMA_Channel_SetBurstLength(DMA_CH_t* Channel, DMA_BurstLength_t Burstlength)
{
	Channel->CTRLA = (Channel->CTRLA & (~DMA_CH_BURSTLEN_gm)) | Burstlength;
}

void DMA_Channel_SetTransferCount(DMA_CH_t* Channel, uint16_t TransferCount)
{
	Channel->TRFCNT = TransferCount;
}

void DMA_CHannel_SetRepeatCount(DMA_CH_t* Channel, uint8_t RepeatCount)
{
	 Channel->REPCNT = RepeatCount;
}
 
void DMA_Channel_SwitchSingleShot(DMA_CH_t* Channel, Bool_t SingleShot)
{
	if(SingleShot == TRUE)
	{
		Channel->CTRLA |= DMA_CH_SINGLE_bm;
	}
	else if(SingleShot == FALSE)
	{
		Channel->CTRLA &= ~DMA_CH_SINGLE_bm;
	}
}

void DMA_Channel_SetSrcReloadMode(DMA_CH_t* Channel, DMA_AddressReload_t Mode)
{
	Channel->ADDRCTRL = (Channel->ADDRCTRL & (~DMA_CH_SRCRELOAD_gm)) | (Mode << 0x06);
}

void DMA_Channel_SetDestReloadMode(DMA_CH_t* Channel, DMA_AddressReload_t Mode)
{
	Channel->ADDRCTRL = (Channel->ADDRCTRL & (~DMA_CH_DESTRELOAD_gm)) | (Mode << 0x02);
}

void DMA_Channel_SetDestAddressingMode(DMA_CH_t* Channel, DMA_AddressMode_t Mode)
{
	Channel->ADDRCTRL = (Channel->ADDRCTRL & (~DMA_CH_DESTDIR_gm)) | Mode;
}

void DMA_Channel_SetSrcAddressingMode(DMA_CH_t* Channel, DMA_AddressMode_t Mode)
{
	Channel->ADDRCTRL = (Channel->ADDRCTRL & (~DMA_CH_SRCDIR_gm)) | (Mode << 0x04);
}

void DMA_Channel_SetSrcAddress(DMA_CH_t* Channel, uintptr_t Address)
{
	Channel->SRCADDR0 = (uintptr_t)Address;
	Channel->SRCADDR1 = (uintptr_t)Address >> 0x08;
	Channel->SRCADDR2 = (uintptr_t)Address >> 0x0F;
}

void DMA_Channel_SetDestAddress(DMA_CH_t* Channel, uintptr_t Address)
{
	Channel->DESTADDR0 = (uintptr_t)Address;
	Channel->DESTADDR1 = (uintptr_t)Address >> 0x08;
	Channel->DESTADDR2 = (uintptr_t)Address >> 0x0F;
}

void DMA_Channel_SetTriggerSource(DMA_CH_t* Channel, DMA_TriggerSource_t TriggerSource)
{
	Channel->TRIGSRC = TriggerSource;
}

void DMA_Channel_StartTransfer(DMA_CH_t* Channel)
{
	DMA_Channel_Enable(Channel);

	Channel->CTRLA |= DMA_CH_TRFREQ_bm;
}

void DMA_Channel_RepeatTransfer(DMA_CH_t* Channel)
{	
	Channel->CTRLA |= DMA_CH_REPEAT_bm;
}

/*
    Interrupt vectors
*/
ISR(DMA_CH0_vect)
{
	_DMA_Channel_InterruptHandler(0);
}

#if(DMA_CHANNEL_COUNT > 1)
	ISR(DMA_CH1_vect)
	{
		_DMA_Channel_InterruptHandler(1);
	}
#endif

#if(DMA_CHANNEL_COUNT > 2)
	ISR(DMA_CH2_vect)
	{
		_DMA_Channel_InterruptHandler(2);
	}
#endif

#if(DMA_CHANNEL_COUNT > 3)
	ISR(DMA_CH3_vect)
	{
		_DMA_Channel_InterruptHandler(3);
	}
#endif