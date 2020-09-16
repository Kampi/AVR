/*
 * USART.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 megaAVR USART module.

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

/** @file Arch/AVR8/megaAVR/mega32/USART/USART.c
 *  @brief Driver for Atmel AVR8 megaAVR USART module.
 *
 *  This contains the implementation of the megaAVR USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/mega32/USART/USART.h"

 /**
  * Rx ring buffer for each USART interface
  */
static RingBuffer_t USART_RxBuffer[USART_DEVICES];

 /**
  * Tx ring buffer for each USART interface
  */
static RingBuffer_t USART_TxBuffer[USART_DEVICES];

#ifndef DOXYGEN
	static bool USART_Echo;

	struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t BufferOverflow;
	} USART_Callbacks[USART_DEVICES];

	static uint8_t RxData[USART_BUFFER_SIZE];
	static uint8_t TxData[USART_BUFFER_SIZE];
#endif

/** @brief			USART interrupt handler.
 *  @param Device	Device ID
 *  @param Callback	Type of interrupt
 */
static inline void USART_InterruptHandler(const uint8_t Device, const USART_CallbackType_t Callback)
{	
	DisableGlobalInterrupts();

    if(Callback == USART_DRE_INTERRUPT)
    {
	    if (USART_Callbacks[Device].EmptyCallback)
	    {
		    USART_Callbacks[Device].EmptyCallback();
	    }
	    
	    if(!(RingBuffer_IsEmpty(&USART_TxBuffer[0])))
	    {
		    UDR = RingBuffer_Load(&USART_TxBuffer[0]);
	    }
	    else
	    {
		    UCSRB &= ~(0x01 << UDRIE);
	    }
    }
	else if(Callback == USART_TXC_INTERRUPT)
	{
		if(USART_Callbacks[Device].TxCallback != NULL)
		{
			USART_Callbacks[Device].TxCallback();
		}
	}
	else if(Callback == USART_RXC_INTERRUPT)
	{
		uint8_t ReceivedByte = UDR;

		// Echo message if enabled
		if(USART_Echo == true)
		{
			UDR = ReceivedByte;
		}

		// Save the data in the buffer
		RingBuffer_Save(&USART_RxBuffer[0], ReceivedByte);

		if(USART_Callbacks[Device].RxCallback != NULL)
		{
			USART_Callbacks[Device].RxCallback();
		}
	}

	EnableGlobalInterrupts();
}

void USART_Init(USART_Config_t* Config)
{
	USART_SwitchEcho(Config->EnableEcho);

	RingBuffer_Init(&USART_RxBuffer[0], RxData, USART_BUFFER_SIZE);
	RingBuffer_Init(&USART_TxBuffer[0], TxData, USART_BUFFER_SIZE);

	USART_SetBaudrate(Config->Baudrate, SysClock_GetCPUClock(), Config->EnableDoubleSpeed);
	USART_SetDirection(Config->Direction);

	if(Config->Size == USART_SIZE_9)
	{
		UCSRB |= (0x01 << UCSZ2);
	}

	// Configure the USART interface
	UCSRC = (0x01 << URSEL) | (Config->DeviceMode << UMSEL) | (Config->Stop << USBS) | (Config->Parity << UPM0) | ((Config->Size & 0x03) << UCSZ0) | Config->ClockPolarity;
}

void USART_InstallCallback(USART_InterruptConfig_t* Config)
{
	if(Config->Source & USART_DRE_INTERRUPT)
	{
		if(Config->Callback != NULL)
		{
			USART_Callbacks[0].EmptyCallback = Config->Callback;
		}
		
		UCSRB |= (0x01 << UDRIE); 
	}

	if(Config->Source & USART_TXC_INTERRUPT)
	{
		if(Config->Callback != NULL)
		{
			USART_Callbacks[0].TxCallback = Config->Callback;
		}

		UCSRB |= (0x01 << TXCIE); 
	}

	if(Config->Source & USART_RXC_INTERRUPT)
	{
		if(Config->Callback != NULL)
		{
			USART_Callbacks[0].RxCallback = Config->Callback;
		}

		UCSRB |= (0x01 << RXCIE);
	}
}

void USART_RemoveCallback(const USART_CallbackType_t Callback)
{
	if(Callback & USART_DRE_INTERRUPT)
	{
		USART_Callbacks[0].EmptyCallback = NULL;

		UCSRB &= ~(0x01 << UDRIE);
	}

	if(Callback & USART_TXC_INTERRUPT)
	{
		USART_Callbacks[0].TxCallback = NULL;

		UCSRB &= ~(0x01 << TXCIE);
	}

	if(Callback & USART_RXC_INTERRUPT)
	{
		USART_Callbacks[0].RxCallback = NULL;

		UCSRB &= ~(0x01 << RXCIE);
	}
}

void USART_Flush(void)
{
	UCSRB |= (0x01 << UDRIE);
}

void USART_Print(char* Data)
{
	// Write data to buffer
	while(*Data)
	{
		RingBuffer_Save(&USART_TxBuffer[0], *Data++);

		if(RingBuffer_IsFull(&USART_TxBuffer[0]))
		{
			// Clear the buffer if it is full
			USART_Flush();
		}
	}

	USART_Flush();
}

void USART_Write(char* Data)
{
	while(*Data)
	{
		USART_SendChar(*Data++);
	}
}

void USART_WriteLine(char* Data)
{
	while(*Data)
	{
		USART_SendChar(*Data++);
	}

	USART_SendChar(LF);
	USART_SendChar(CR);
}

void USART_WriteDecimal(const uint32_t Number)
{
	uint8_t Temp = Number / 10;
	char Buffer[2];

	if(Temp)
	{
		USART_WriteDecimal(Temp);
	}

	Buffer[0] = 0x30 + (Number % 10);
	Buffer[1] = '\0';

	USART_Write(Buffer);
}

void USART_SwitchEcho(const bool Enable)
{
	if(Enable == true)
	{
		USART_Echo = true;
	}
	else
	{
		USART_Echo = false;
	}
}

void USART_SetBaudrate(const uint32_t Baudrate, const uint32_t Clock, const bool DoubleSpeed)
{
	uint16_t BaudValue = ((Clock << DoubleSpeed) / (Baudrate << 0x04)) - 0x01;

	UBRRH &= ~(0x01 << URSEL);
	UBRRH = (BaudValue >> 0x08);
	UBRRL = (BaudValue & 0xFF);
	UCSRA = (UCSRA & ~(0x01 << U2X)) | (DoubleSpeed << U2X);
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(USART_RXC_vect)
	{   
		USART_InterruptHandler(0, USART_RXC_INTERRUPT);
	}

	ISR(USART_TXC_vect)
	{		
		USART_InterruptHandler(0, USART_TXC_INTERRUPT);
	}

	ISR(USART_UDRE_vect)
	{	
		USART_InterruptHandler(0, USART_DRE_INTERRUPT);
	}
#endif