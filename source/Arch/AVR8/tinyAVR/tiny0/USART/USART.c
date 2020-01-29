/*
 * USART.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 tiny0 USART module.

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

/** @file Arch/AVR8/tinyAVR/tiny0/USART/USART.c
 *  @brief Driver for Atmel AVR8 tiny0 USART module.
 *
 *  This contains the implementation of the tiny0 USART driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/tinyAVR/tiny0/USART/USART.h"

/** @brief Tx ring buffer for each USART interface.
 */
static RingBuffer_t __USART_TxRingBuffer[USART_DEVICES];

/** @brief Data buffer for Tx ring buffer.
 */
static uint8_t __TxData[USART_DEVICES][USART_BUFFER_SIZE];

#ifndef DOXYGEN
	/*
		Object declaration
	*/
	USART_Message_t __USART_Messages[USART_DEVICES];
	Bool_t __USART_Echo[USART_DEVICES];
	struct
	{
		USART_Callback_t RxCallback;
		USART_Callback_t TxCallback;
		USART_Callback_t EmptyCallback;
		USART_Callback_t StartCallback;
		USART_Callback_t SyncFieldCallback;
	} __USART_Callbacks[USART_DEVICES];
#endif

void USART_Init(USART_Config_t* Config)
{
    PORT_t* Port = 0x00;
    uint8_t RxPin = 0x00;
    uint8_t TxPin = 0x00;
	
	Port = &PORTB;
	RxPin = USART_RX0_PIN;
	TxPin = USART_TX0_PIN;

	USART_SwitchEcho(Config->Device, Config->EnableEcho);

    USART_SetBaudrate(Config->Device, Config->Baudrate, 16000000UL, Config->EnableDoubleSpeed);
    USART_SetDirection(Config->Device, Config->Direction);
    USART_SetDeviceMode(Config->Device, Config->DeviceMode);
    USART_SetDataSize(Config->Device, Config->Size);
    USART_SetStopbits(Config->Device, Config->Stop);
    USART_SetParity(Config->Device, Config->Parity);
	USART_SwitchOpenDrain(Config->Device, Config->EnableOpenDrain);

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

void USART_InstallCallback(const USART_InterruptConfig_t* Config)
{	
	if(Config->Source & USART_DRE_INTERRUPT)
	{
		Config->Device->CTRLA = USART_DREIE_bm;
		__USART_Callbacks[0].EmptyCallback = Config->Callback;
	}
	
	if(Config->Source & USART_TXC_INTERRUPT)
	{
		Config->Device->CTRLA = USART_TXEN_bm;
		__USART_Callbacks[0].TxCallback = Config->Callback;
	}
	
	if(Config->Source & USART_RXC_INTERRUPT)
	{
		Config->Device->CTRLA = USART_RXCIE_bm;
		__USART_Callbacks[0].RxCallback = Config->Callback;
	}
	
	if(Config->Source & USART_RXS_INTERRUPT)
	{
		Config->Device->CTRLA = USART_RXSIE_bm;
		__USART_Callbacks[0].EmptyCallback = Config->Callback;
	}
	
	if(Config->Source & USART_ISF_INTERRUPT)
	{
		__USART_Callbacks[0].SyncFieldCallback = Config->Callback;
	}
}

void USART_RemoveCallback(USART_t* Device, const USART_CallbackType_t Callback)
{
	if(Callback & USART_DRE_INTERRUPT)
	{
		__USART_Callbacks[0].EmptyCallback = NULL;
	}
	
	if(Callback & USART_TXC_INTERRUPT)
	{
		__USART_Callbacks[0].TxCallback = NULL;
	}
	
	if(Callback & USART_RXC_INTERRUPT)
	{
		__USART_Callbacks[0].RxCallback = NULL;
	}
	
	if(Callback & USART_RXS_INTERRUPT)
	{
		__USART_Callbacks[0].StartCallback = NULL;
	}

	if(Callback & USART_ISF_INTERRUPT)
	{
		__USART_Callbacks[0].SyncFieldCallback = NULL;
	}
}

void USART_SwitchEcho(USART_t* Device, const Bool_t Enable)
{
	uint8_t USART = 0x00;
	
	if(Device == &USART0)
	{
		USART = 0;
	}

	if(Enable == TRUE)
	{
		__USART_Echo[USART] = TRUE;
	}
	else
	{
		__USART_Echo[USART] = FALSE;
	}
}

void USART_SetBaudrate(USART_t* Device, const uint32_t Baudrate, const uint32_t Clock, const Bool_t DoubleSpeed)
{
	uint8_t S = 0x10 >> (DoubleSpeed & 0x01);
	uint16_t Baud = (Clock << 0x06) / (S * Baudrate);

	Device->CTRLB = (Device->CTRLB & ~(0x03 << 0x01)) | ((DoubleSpeed & 0x01) << 0x01);
	Device->BAUD = Baud;
}

void USART_Write(USART_t* Device, const char* Data)
{
	while(*Data)
	{
		USART_SendChar(Device, *Data++);
	}
}

void USART_WriteDecimal(USART_t* Device, const uint32_t Value)
{
	uint32_t Temp = Value / 10;
	char Buffer[2];
	
	if(Temp)
	{
		USART_WriteDecimal(Device, Temp);
	}
	
	Buffer[0] = 0x30 + (Value % 10);
	Buffer[1] = '\0';
	
	USART_Write(Device, Buffer);
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