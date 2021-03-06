/*
 * USART.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *  Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega USART interface.
 
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

/** @file Arch/XMega/USART/USART.c
 *  @brief Driver for Atmel AVR XMega USART module.
 *
 *  This contains the implementation of the Atmel AVR XMega USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/USART/USART.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

/** @brief Tx ring buffer for each USART interface.
 */
static RingBuffer_t _USART_TxRingBuffer[USART_DEVICES][USART_CHANNEL];

/** @brief Data buffer for Tx ring buffer.
 */
static uint8_t _TxData[USART_DEVICES][USART_CHANNEL][USART_BUFFER_SIZE];

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	USART_Message_t _USART_Messages[USART_DEVICES][USART_CHANNEL];
	bool _USART_Echo[USART_DEVICES][USART_CHANNEL];
	struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t BufferOverflow;
	} _USART_Callbacks[USART_DEVICES][USART_CHANNEL];
#endif

void USART_InstallCallback(const USART_InterruptConfig_t* Config)
{
	uint8_t Device = 0x00;
	uint8_t Channel = 0x00;
	
	if(Config->Device == &USARTC0)
	{
		Device = USARTC_ID;
		Channel = 0x00;
	}
	else if(Config->Device == &USARTD0)
	{
		Device = USARTD_ID;
		Channel = 0x00;
	}
	else if(Config->Device == &USARTE0)
	{
		Device = USARTE_ID;
		Channel = 0x00;
	}
	#if(defined USARTC1)
		else if(Config->Device == &USARTC1)
		{
			Device = USARTC_ID;
			Channel = 0x00;
		}
	#endif

	#if(defined USARTD1)
		else if(Config->Device == &USARTD1)
		{
			Device = USARTD_ID;
			Channel = 0x01;
		}
	#endif
	
	#if(defined USARTF0)
		else if(Config->Device == &USARTF0)
		{
			Device = USARTF_ID;
			Channel = 0x00;
		}
	#endif
	
	if(Config->Source & USART_DRE_INTERRUPT)
	{
		Config->Device->CTRLA = (Config->Device->CTRLA & (~0x03)) | Config->InterruptLevel;
		_USART_Callbacks[Device][Channel].EmptyCallback = Config->Callback;
	}
	
	if(Config->Source & USART_TXC_INTERRUPT)
	{
		Config->Device->CTRLA = (Config->Device->CTRLA & (~(0x03 << 0x02))) | (Config->InterruptLevel << 0x02);
		_USART_Callbacks[Device][Channel].TxCallback = Config->Callback;
	}
	
	if(Config->Source & USART_RXC_INTERRUPT)
	{
		Config->Device->CTRLA = (Config->Device->CTRLA & (~(0x03 << 0x04))) | (Config->InterruptLevel << 0x04);
		_USART_Callbacks[Device][Channel].RxCallback = Config->Callback;
	}
	
	if(Config->Source & USART_BUFFER_OVERFLOW)
	{
		_USART_Callbacks[Device][Channel].BufferOverflow = Config->Callback;
	}
}

void USART_RemoveCallback(USART_t* Device, const USART_CallbackType_t Callback)
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
			Device = USARTC_ID;
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
			USART = USARTF_ID;
			Channel = 0x00;
		}
	#endif
	
	if(Callback & USART_DRE_INTERRUPT)
	{
		_USART_Callbacks[USART][Channel].EmptyCallback = NULL;
	}
	
	if(Callback & USART_TXC_INTERRUPT)
	{
		_USART_Callbacks[USART][Channel].TxCallback = NULL;
	}
	
	if(Callback & USART_RXC_INTERRUPT)
	{
		_USART_Callbacks[USART][Channel].RxCallback = NULL;
	}
	
	if(Callback & USART_BUFFER_OVERFLOW)
	{
		_USART_Callbacks[USART][Channel].BufferOverflow = NULL;
	}
}

void USART_ChangeInterruptLevel(USART_t* Device, const USART_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel)
{
	if(Callback & USART_DRE_INTERRUPT)
	{
		Device->CTRLA = (Device->CTRLA & (~0x03)) | InterruptLevel;
	}
	
	if(Callback & USART_TXC_INTERRUPT)
	{
		Device->CTRLA = (Device->CTRLA & (~(0x03 << 0x02))) | (InterruptLevel << 0x02);
	}
	
	if(Callback & USART_RXC_INTERRUPT)
	{
		Device->CTRLA = (Device->CTRLA & (~(0x03 << 0x04))) | (InterruptLevel << 0x04);
	}
}
 
void USART_Init(USART_Config_t* Config)
{
    PORT_t* Port = 0x00;
	uint8_t RxPin = 0x00;
	uint8_t TxPin = 0x00;

	USART_SwitchEcho(Config->Device, Config->EnableEcho);
    USART_PowerEnable(Config->Device);
    USART_SetBaudrate(Config->Device, Config->Baudrate, SysClock_GetClockPer(), Config->BSCALE, Config->EnableDoubleSpeed);
    USART_SetDirection(Config->Device, Config->Direction);
    USART_SetDeviceMode(Config->Device, Config->DeviceMode);
    USART_SetDataSize(Config->Device, Config->Size);
    USART_SetStopbits(Config->Device, Config->Stop);
    USART_SetParity(Config->Device, Config->Parity);
 
	if(Config->EnableInterruptSupport == true)
	{
		USART_EnableInterruptSupport(Config->Device, Config->InterruptLevel);
	}
 
    if(Config->Device == &USARTC0)
	{
		Port = &PORTC;
		RxPin = USART_RX0_PIN;
		TxPin = USART_TX0_PIN;
	}
	else if(Config->Device == &USARTD0)
	{
		Port = &PORTD;
		RxPin = USART_RX0_PIN;
		TxPin = USART_TX0_PIN;
	}
	else if(Config->Device == &USARTE0)
	{
		Port = &PORTE;
		RxPin = USART_RX0_PIN;
		TxPin = USART_TX0_PIN;
	}
	#if(defined USARTC1)
		else if(Config->Device == &USARTC1)
		{
			Port = &PORTC;
			RxPin = USART_RX1_PIN;
			TxPin = USART_TX1_PIN;
		}
	#endif
	
	#if(defined USARTD1)
		else if(Config->Device == &USARTD1)
		{
			Port = &PORTD;
			RxPin = USART_RX1_PIN;
			TxPin = USART_TX1_PIN;
		}
	#endif
	
	#if(defined USARTF0)
		else if(Config->Device == &USARTD1)
		{
			Port = &PORTF;
			RxPin = USART_RX0_PIN;
			TxPin = USART_TX0_PIN;
		}
	#endif

    if(Config->Direction == USART_DIRECTION_BOTH)
    {
        GPIO_SetDirection(Port, RxPin, GPIO_DIRECTION_IN);
        GPIO_SetDirection(Port, TxPin, GPIO_DIRECTION_OUT);
    }
    else if(Config->Direction == USART_DIRECTION_RX)
    {
        GPIO_SetDirection(Port, RxPin, GPIO_DIRECTION_IN);
    }
    else if(Config->Direction == USART_DIRECTION_TX)
    {
        GPIO_SetDirection(Port, TxPin, GPIO_DIRECTION_OUT);
    }
}

void USART_EnableInterruptSupport(USART_t* Device, const Interrupt_Level_t Level)
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

	RingBuffer_Init(&_USART_TxRingBuffer[ID][Channel], _TxData[ID][Channel], USART_BUFFER_SIZE);
	
	_USART_Messages[ID][Channel].Device = Device;
	_USART_Messages[ID][Channel].Ptr_TxRingBuffer = &_USART_TxRingBuffer[ID][Channel];
	
	Device->CTRLA = (Device->CTRLA & (~(0x03 << 0x04))) | (Level << 0x04);
}

void USART_DisableInterruptSupport(USART_t* Device)
{
	Device->CTRLA &= ~(0x03 << 0x04);
}

void USART_Write(USART_t* Device, const char* Data)
{
    while(*Data)
    {
        USART_SendChar(Device, *Data++);
    }
}
 
void USART_WriteDecimal(USART_t* Device, const uint32_t Number, const uint8_t Base) 
{
	char* pBuffer;
	uint32_t Number_Temp = Number;

	// Reserve a transmit buffer. Size depends on the type of 'Number' + array end
	char Buffer[sizeof(Number) + 2] = {0x00};

	// Set the end of a char array
	Buffer[sizeof(Buffer) - 1] = '\0';

	// Pointer to the transmit buffer. Set the pointer to the last element before the
	// array end, because the transformation starts with the ones
	pBuffer = &Buffer[sizeof(Buffer) - 2];
	
	do 
	{
		*--pBuffer = Representation[Number_Temp % Base];
		Number_Temp /= Base;
	}while(Number_Temp != 0);
	
    while(*pBuffer)
    {
	    USART_SendChar(Device, *pBuffer++);
    }
}
 
void USART_WriteLine(USART_t* Device, const char* Data)
{
    while(*Data)
    {
        USART_SendChar(Device, *Data++);
    }
     
    USART_SendChar(Device, LF);
    USART_SendChar(Device, CR);
}

void USART_Print(USART_t* Device, const char* Data)
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
	
	// Write data to buffer
	while(*Data)
	{
		RingBuffer_Save(&_USART_TxRingBuffer[ID][Channel], *Data++);
		
		if(RingBuffer_IsFull(&_USART_TxRingBuffer[ID][Channel]))
		{
			// Clear the buffer if it is full
			USART_Flush(Device);
		}
	}

	USART_Flush(Device);
}

void USART_Flush(USART_t* Device)
{
	Device->CTRLA |= 0x03;
}

void USART_SwitchEcho(USART_t* Device, const bool Enable)
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
	
	if(Enable == true)
	{
		_USART_Echo[ID][Channel] = true;
	}
	else
	{
		_USART_Echo[ID][Channel] = false;
	}
}

void USART_SetBaudrate(USART_t* Device, const uint32_t Baudrate, const uint32_t Clock, const int8_t BSCALE, const bool DoubleSpeed)
{
	float BSEL_Temp = 0x00;
	uint16_t BSEL = 0x00;

	// Calculate ClockRatio = f_Per / (8/16 * f_Baud)
	if(DoubleSpeed == true)
	{
		BSEL_Temp = (float)Clock / (Baudrate << 0x03);
	}
	else
	{
		BSEL_Temp = (float)Clock / (Baudrate << 0x04);
	}

	// Calculate BSEL = 1/(2^BSCALE) * ClockRatio
	if(BSCALE < 0x00)
	{
		int8_t BSCALE_Temp = 0x00;

		// Subtract 1 from 'ClockRatio' when BSCALE < 0
		BSEL_Temp -= 0x01;
		
		BSCALE_Temp = (~BSCALE) + 0x01;
		BSEL_Temp = BSEL_Temp * (float)(0x01 << BSCALE_Temp);
	}
	else
	{
		BSEL_Temp = BSEL_Temp * (0x01 >> BSCALE);

		// Subtract 1 from 'BSEL' when BSCALE >= 0
		BSEL_Temp -= 0x01;
	}

	BSEL = (uint16_t)BSEL_Temp;
    Device->CTRLB = (Device->CTRLB & ~((0x01 << 0x02))) | (DoubleSpeed << 0x02);
    Device->BAUDCTRLB = ((BSCALE & 0x0F) << 0x04) | ((BSEL & 0xF00) >> 0x08);
    Device->BAUDCTRLA = BSEL & 0xFF;
}