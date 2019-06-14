/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Bootloader example for XMega.

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

/** @file main.c
 *  @brief Bootloader example for XMega.
 *
 *  Software for the XMega bootloader example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/NVM/NVM.h"
#include "Arch/XMega/USART/USART.h"

void USART_Rx_Callback(void);

RingBuffer_t RxRing;
uint8_t RxData[32];

/*
	USART configuration
 */
USART_Config_t Config_USART = {
	.Device = &USARTE0,
	.DeviceMode = USART_MODE_ASYNCH,
	.Direction = USART_DIRECTION_BOTH,
	.Baudrate = 19200,
	.Parity = USART_PARITY_NONE,
	.Stop = USART_STOP_1,
	.Size = USART_SIZE_8, 
	.BSCALE = 3, 
	.EnableDoubleSpeed = FALSE,
	.EnableInterruptSupport = TRUE,
	.InterruptLevel = INT_LVL_HI
};

USART_InterruptConfig_t USART_Callback = {
	.Device = &USARTE0,
	.InterruptLevel = INT_LVL_HI,
	.Source = USART_RXC_INTERRUPT,
	.Callback = USART_Rx_Callback,
};

int main(void)
{
	/*
		Initialize main clock
	*/
	SysClock_Init();

	/*
		Initialize the USART
	*/
	USART_Init(&Config_USART);
	USART_InstallCallback(&USART_Callback);
	RingBuffer_Init(&RxRing, RxData, 32);

	/*
		Enable global interrupts
	*/
	EnableGlobalInterrupts();
	PMIC_EnableAllInterruptLevel();

	USART_Write(&USARTE0, "Start...\n\r");
	
    while(1) 
    {
    }
}

void USART_Rx_Callback(void)
{
	RingBuffer_Save(&RxRing, USART_GetChar(&USARTE0));
	
	if(RingBuffer_IsFull(&RxRing))
	{
		USART_SendChar(&USARTE0, XOFF);
		
		while(!RingBuffer_IsEmpty(&RxRing))
		{
			USART_SendChar(&USARTE0, RingBuffer_Load(&RxRing));
		}
		
		USART_SendChar(&USARTE0, '\n');
		USART_SendChar(&USARTE0, '\r');
		
		USART_SendChar(&USARTE0, XON);
	}
}

