/*
 * USART_SPI.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *  Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 XMega USART-SPI interface.
 
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

/** @file Arch/XMega/USART/USART_SPI.c
 *  @brief Driver for Atmel AVR8 XMega USART-SPI module..
 *
 *  This contains the implementation of the Atmel AVR8 XMega USART-SPI driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/USART/USART.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	extern SPI_Message_t _SPI_Messages[USART_DEVICES][USART_CHANNEL];
	extern bool _USART_IsSPI[USART_DEVICES][USART_CHANNEL];

	struct
	{
		SPI_Callback_t CompleteInterrupt;
	} _USART_SPI_Callbacks[USART_DEVICES][USART_CHANNEL];
#endif

void USART_SPI_Init(SPIM_Config_t* Config)
{
	uint8_t ID = 0x00;
	uint8_t Channel = 0x00;
	
	// Get the USART device for USART-SPI mode
	USART_t* Device = (USART_t*)Config->Device;

	USART_PowerEnable(Config->Device);

	// Disable Rx
	Device->CTRLB &= ~USART_RXEN_bm;
	
	if(Device == &USARTC0)
	{
		ID = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		ID = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		ID = USARTE_ID;
		Channel = 0x00;
	}
	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			ID = USARTC_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			ID = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			ID = USARTF_ID;
			Channel = 0x00;
		}
	#endif

	_USART_IsSPI[ID][Channel] = true;
	
	if(Config->Device == &USARTD0)
	{
		GPIO_SetDirection(&PORTD, USART_TX0_PIN, GPIO_DIRECTION_OUT);
		GPIO_Set(&PORTD, USART_TX0_PIN);
		GPIO_SetDirection(&PORTD, USART_XCK0_PIN, GPIO_DIRECTION_OUT);
		GPIO_Set(&PORTD, USART_XCK0_PIN);
		GPIO_SetDirection(&PORTD, USART_RX0_PIN, GPIO_DIRECTION_IN);
		
		// Setup INVEN
		if((Config->Mode == SPI_MODE_2) || (Config->Mode == SPI_MODE_3))
		{
			GPIO_SwitchInvert(&PORTD, (0x01 << SPI_SCK_PIN), true);
		}
		else
		{
			GPIO_SwitchInvert(&PORTD, (0x01 << SPI_SCK_PIN), false);
		}
	}
	
	// Enable USART master SPI
	USART_SetDeviceMode(Device, 0x03);
	
	// Setup Clock Phase
	if((Config->Mode == SPI_MODE_1) || (Config->Mode == SPI_MODE_3))
	{
		Device->CTRLC |= 0x02;
	}
	else
	{
		Device->CTRLC &= ~0x02;
	}
	
	// Setup data order
	if(Config->DataOrder == SPI_DATAORDER_LSB_FIRST)
	{
		Device->CTRLC |= 0x04;
	}
	else
	{
		Device->CTRLC &= ~0x04;
	}

	USART_SPI_SetClockRate(Device, Config->SPIClock, SysClock_GetClock(), Config->EnableDoubleSpeed);
	
	// Enable Rx and Tx
	Device->CTRLB |= (USART_RXEN_bm | USART_TXEN_bm);
}

void USART_SPI_EnableInterruptSupport(USART_t* Device, const Interrupt_Level_t Level)
{
	uint8_t ID = 0x00;
	uint8_t Channel = 0x00;
	
	if(Device == &USARTC0)
	{
		ID = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		ID = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		ID = USARTE_ID;
		Channel = 0x00;
	}

	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			ID = USARTC_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			ID = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			ID = USARTF_ID;
			Channel = 0x00;
		}
	#endif
	
	_SPI_Messages[ID][Channel].Device = (USART_t*)Device;
	
	Device->CTRLA = (Level << 0x04) | (Level << 0x02);
}

void USART_SPI_DisableInterruptSupport(USART_t* Device)
{
	Device->CTRLA &= ~(0x03 << 0x04);
}

const uint8_t USART_SPI_SendData(USART_t* Device, const uint8_t Data)
{
	while(!(Device->STATUS & USART_DREIF_bm));
	Device->DATA = Data;
	
	while(!(Device->STATUS & USART_TXCIF_bm));
	Device->STATUS = USART_TXCIF_bm;
	return Device->DATA;
}

SPI_Status_t USART_SPI_Transmit(SPI_Message_t* Message)
{
	uint8_t ID = 0x00;
	uint8_t Channel = 0x00;
	
	// Get the USART device for USART-SPI mode
	USART_t* Device = (USART_t*)Message->Device;
	
	if(Device == &USARTC0)
	{
		ID = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		ID = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		ID = USARTE_ID;
		Channel = 0x00;
	}

	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			ID = USARTC_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			ID = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			ID = USARTF_ID;
			Channel = 0x00;
		}
	#endif

	// Save the message object
	_SPI_Messages[ID][Channel] = *Message;
	
	// Reset the read and write counter
	_SPI_Messages[ID][Channel].BytesProcessed = 0x00;

	// Wait for active transmissions
	if(_SPI_Messages[ID][Channel].Status != SPI_MESSAGE_COMPLETE)
	{
		return _SPI_Messages[ID][Channel].Status;
	}

	_SPI_Messages[ID][Channel].Status = 0x00;
	
	// Select the device
	USART_SPI_SelectDevice(_SPI_Messages[ID][Channel].Port, _SPI_Messages[ID][Channel].Pin);
	
	// Send the first byte
	((USART_t*)(_SPI_Messages[ID][Channel].Device))->DATA = _SPI_Messages[ID][Channel].BufferOut[_SPI_Messages[ID][Channel].BytesProcessed];

	return SPI_MESSAGE_PENDING;
}

void USART_SPI_InstallCallback(const SPI_InterruptConfig_t* Config)
{
	uint8_t ID = 0x00;
	uint8_t Channel = 0x00;
	
	// Get the USART device for USART-SPI mode
	USART_t* Device = (USART_t*)Config->Device;
	
	if(Device == &USARTC0)
	{
		ID = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		ID = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		ID = USARTE_ID;
		Channel = 0x00;
	}

	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			ID = USARTC_ID;
			Channel = 0x00;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			ID = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			ID = USARTF_ID;
			Channel = 0x00;
		}
	#endif
	
	if(Config->Source & SPI_COMPLETE_INTERRUPT)
	{
		Device->CTRLA = (Config->InterruptLevel << 0x04) | (Config->InterruptLevel << 0x02);
		_USART_SPI_Callbacks[ID][Channel].CompleteInterrupt = Config->Callback;
	}
}

void USART_SPI_RemoveCallback(USART_t* Device, const SPI_CallbackType_t Callback)
{
	uint8_t USART = 0x00;
	uint8_t Channel = 0x00;
	
	if(Device == &USARTC0)
	{
		USART = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		USART = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		USART = USARTE_ID;
		Channel = 0x00;
	}

	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			USART = USARTC_ID;
			Channel = 0x00;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			USART = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			USART = USARTE_ID;
			Channel = 0x00;
		}
	#endif
	
	if(Callback & SPI_COMPLETE_INTERRUPT)
	{
		_USART_SPI_Callbacks[USART][Channel].CompleteInterrupt = NULL;
	}
}

void USART_SPI_ChangeInterruptLevel(USART_t* Device, const USART_CallbackType_t Callback, const Interrupt_Level_t Level)
{
	if(Callback & SPI_COMPLETE_INTERRUPT)
	{
		Device->CTRLA = (Level << 0x04) | (Level << 0x02);
	}
}

SPI_Status_t USART_SPI_Status(const USART_t* Device)
{
	uint8_t ID = 0x00;
	uint8_t Channel = 0x00;
	
	if(Device == &USARTC0)
	{
		ID = USARTC_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTD0)
	{
		ID = USARTD_ID;
		Channel = 0x00;
	}
	else if(Device == &USARTE0)
	{
		ID = USARTE_ID;
		Channel = 0x00;
	}

	#if(defined USARTC1)
		else if(Device == &USARTC1)
		{
			ID = USARTC_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTD1)
		else if(Device == &USARTD1)
		{
			ID = USARTD_ID;
			Channel = 0x01;
		}
	#endif

	#if(defined USARTF0)
		else if(Device == &USARTF0)
		{
			ID = USARTF_ID;
			Channel = 0x00;
		}
	#endif

	return _SPI_Messages[ID][Channel].Status;
}

void USART_SPI_SelectDevice(PORT_t* Port, const uint8_t Pin)
{
	GPIO_Clear(Port, Pin);
}

void USART_SPI_DeselectDevice(PORT_t* Port, const uint8_t Pin)
{
	GPIO_Set(Port, Pin);
}

void USART_SPI_SetClockRate(USART_t* Device, const uint32_t Baudrate, const uint32_t Clock, const bool DoubleSpeed)
{
	uint16_t BaudValue = 0x00;
	
	if(Baudrate < (Clock >> 0x01))
	{
		BaudValue = (Clock / (Baudrate << 0x01)) - 0x01;
	}
	
	Device->CTRLB = (Device->CTRLB & ~((0x01 << 0x02))) | (DoubleSpeed << 0x02);
	Device->BAUDCTRLB = (uint8_t)((~USART_BSCALE_gm) & (BaudValue >> 0x08));
	Device->BAUDCTRLA = (uint8_t)(BaudValue);
}

const uint32_t USART_SPI_GetClockRate(const USART_t* Device, const uint32_t Clock)
{
	return Clock / (((Device->BAUDCTRLB & 0x0F) + Device->BAUDCTRLA + 1) << 0x01);
}