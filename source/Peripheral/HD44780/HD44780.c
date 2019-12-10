/*
 * HD44780.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for HD44780 LCD controller.

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

/** @file Peripheral/HD44780/HD44780.c
 *  @brief Driver for Hitachi HD44780 LCD controller.
 *
 *  This file contains the implementation for the HD44780 LCD controller.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/HD44780/HD44780.h"

/** @defgroup HD44780
 *  @{
 */
	/** @defgroup HD44780-Address
	 *  HD44780 memory addresses.
	 *  @{
	 */
		#define HD44780_ROW_0					0x00
		#define HD44780_ROW_1					0x40
		#define HD44780_ROW_2					(ROW_0 + HD44780_COLUMNS)
		#define HD44780_ROW_3					(ROW_1 + HD44780_COLUMNS)
	 /** @} */ // end of HD44780-Address


	/** @defgroup HD44780-Entry
	 *  HD44780 entry masks.
	 *  @{
	 */
		#define HD44780_ENTRY_INCR				(0x01 << 0x01)
		#define HD44780_ENTRY_DECR				0x00
		#define HD44780_ENTRY_SHIFT_ON			(0x01 << 0x00)
		#define HD44780_ENTRY_SHIFT_OFF			0x00
	 /** @} */ // end of HD44780-Entry

	/** @defgroup HD44780-Display
	 *  HD44780 display masks.
	 *  @{
	 */
		#define HD44780_DISPLAY_ON				(0x01 << 0x02)
		#define HD44780_DISPLAY_OFF				0x00
		#define HD44780_DISPLAY_CURSOR_ON		(0x01 << 0x01)
		#define HD44780_DISPLAY_CURSOR_OFF		0x00
		#define HD44780_DISPLAY_BLINK_ON		(0x01 << 0x00)
		#define HD44780_DISPLAY_BLINK_OFF		0x00
	 /** @} */ // end of HD44780-Display

	/** @defgroup HD44780-Cursor
	 *  HD44780 cursor masks.
	 *  @{
	 */
		#define HD44780_CURSOR_SHIFT_DISPLAY	(0x01 << 0x03)
		#define HD44780_CURSOR_MOVE				0x00
		#define HD44780_CURSOR_SHIFT_RIGHT		(0x01 << 0x02)
		#define HD44780_CURSOR_SHIFT_LEFT		0x00
		#define HD44780_CURSOR_GRAFIC_MODE		(0x01 << 0x03)
		#define HD44780_CURSOR_CHARCTER_MODE	0x00
		#define HD44780_CURSOR_POWER_ON			(0x01 << 0x02)
		#define HD44780_CURSOR_POWER_OFF		0x00
		#define HD44780_CURSOR_MODE_MASK		((0x01 << 0x01) | (0x01 << 0x00))
	 /** @} */ // end of HD44780-Cursor

	/** @defgroup HD44780-Functions
	 *  HD44780 function masks.
	 *  @{
	 */
		#define HD44780_FUNC_4BIT				0x00
		#define HD44780_FUNC_8BIT				(0x01 << 0x04)
		#define HD44780_FUNC_1LINE				0x00
		#define HD44780_FUNC_2LINE				(0x01 << 0x03)
		#define HD44780_FUNC_5X7				0x00
		#define HD44780_FUNC_5X10				(0x01 << 0x02)
	 /** @} */ // end of HD44780-Functions

	/** @defgroup HD44780-Commands
	 *  HD44780 commands.
	 *  @{
	 */
		#define HD44780_CMD_CLEAR				0x01
		#define HD44780_CMD_RETURN_HOME			0x02
		#define HD44780_CMD_ENTRY				0x04
		#define HD44780_CMD_DISPLAY				0x08
		#define HD44780_CMD_CURSOR				0x10
		#define HD44780_CMD_FUNC				0x20
		#define HD44780_CMD_RESET				0x30
	/** @} */ // end of HD44780-Commands
/** @} */ // end of HD44780

/** @brief Local variable to store the entry configuration.
 */
static uint8_t __HD44780_Entry = 0x00;

/** @brief Local variable to store the display configuration.
 */
static uint8_t __HD44780_Display = 0x00;

/** @brief Local variable to store the cursor configuration.
 */
static uint8_t __HD44780_Cursor = 0x00;

/** @brief Local variable to store the function configuration.
 */
static uint8_t __HD44780_Func = 0x00;

/** @brief Strobe the ENABLE pin.
 */
static void HD44780_Strobe(void)
{
	GPIO_Set(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
	_delay_us(1);
	GPIO_Clear(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
	_delay_us(1);
}

/** @brief	Clear the data bus.
 */
static void HD44780_ClearData(void)
{
	#if(defined(HD44780_USE_4BIT))
		GPIO_ClearPort(GET_PERIPHERAL(HD44780_DATA), 0x0F << GET_INDEX(HD44780_DATA));
	#elif(defined(HD44780_USE_8BIT))
		GPIO_ClearPort(GET_PERIPHERAL(HD44780_DATA), 0xFF);
	#endif
}

/** @brief		Send a data byte to the LCD.
 *  @param Data	Data byte
 */
static void HD44780_WriteByte(const uint8_t Data)
{
	HD44780_ClearData();

	#if(defined(HD44780_USE_4BIT))
		// Send the upper nibble
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), (Data & 0xF0) >> (0x04 - GET_INDEX(HD44780_DATA)));

		// Strobe the ENABLE pin
		HD44780_Strobe();

		// Send the lower nibble
		HD44780_ClearData();
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), (Data & 0x0F) << GET_INDEX(HD44780_DATA));

		// Strobe the ENABLE pin
		HD44780_Strobe();
	#elif(defined(HD44780_USE_8BIT))
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), Data);

		// Strobe the ENABLE pin
		HD44780_Strobe();
	#endif
}

/** @brief	Reset the display.
 */
static void HD44780_Reset(void)
{
	// Wait until the display is ready
	_delay_ms(15);

	GPIO_Clear(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));

	HD44780_ClearData();

	// You have to write 0x30 three times to reset the display. 
	// NOTE: The display use 8 bit mode after the reset!
	__HD44780_Func |= HD44780_FUNC_8BIT;
	#if(defined(HD44780_USE_4BIT))
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), 0x03 << GET_INDEX(HD44780_DATA));
	#elif(defined(HD44780_USE_8BIT))
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), 0x30);
	#endif

	for(uint8_t i = 0x00; i < 0x03; i++)
	{
		HD44780_Strobe();
		_delay_ms(5);
	}

	#if(defined(HD44780_USE_4BIT))
		HD44780_ClearData();
		// Switch the display into 4 bit mode
		__HD44780_Func &= ~HD44780_FUNC_8BIT;
		GPIO_SetPort(GET_PERIPHERAL(HD44780_DATA), 0x02 << GET_INDEX(HD44780_DATA));
	#endif

	HD44780_Strobe();

	_delay_us(5);
}

#if(defined(HD44780_USE_BUSY))
	/** @brief	Check if the BUSY flag is set.
	 *  @return	#TRUE when BUSY is set
	 */
	static Bool_t HD44780_IsBusy(void)
	{
		volatile uint8_t Status = 0x00;

		// Switch DB7 to input
		#if(defined(HD44780_USE_8BIT))
			GPIO_SetDirection(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x07, GPIO_DIRECTION_IN);
		#elif(defined(HD44780_USE_4BIT))
			GPIO_SetDirection(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x03, GPIO_DIRECTION_IN);
		#endif

		GPIO_Clear(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
		GPIO_Set(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));
		_delay_us(1);

		// Read the BUSY bit
		GPIO_Set(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
		_delay_us(1);
		#if(defined(HD44780_USE_8BIT))
			Status = GPIO_Read(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x07) >> 0x07;
		#elif(defined(HD44780_USE_4BIT))
			Status = GPIO_Read(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x03) >> 0x03;
		#endif
		GPIO_Clear(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));

		#if(defined(HD44780_USE_4BIT))
			// Read the lower nibble
			GPIO_Set(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
			_delay_us(1);
			GPIO_Clear(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
		#endif

		GPIO_Set(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
		GPIO_Clear(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));

		// Switch DB7 back to output
		#if(defined(HD44780_USE_8BIT))
			GPIO_SetDirection(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x07, GPIO_DIRECTION_OUT);
		#elif(defined(HD44780_USE_4BIT))
			GPIO_SetDirection(GET_PERIPHERAL(HD44780_DATA), GET_INDEX(HD44780_DATA) + 0x03, GPIO_DIRECTION_OUT);
		#endif

		return Status;
	}
#endif

/** @brief			Send a command byte to the LCD controller.
 *  @param Command	Command byte
 */
static void HD44780_SendCommand(const uint8_t Command)
{
	#if(defined(HD44780_USE_BUSY))
		while(HD44780_IsBusy());
	#else
		_delay_ms(1);
	#endif

	GPIO_Clear(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));
	HD44780_WriteByte(Command);
};

/** @brief		Send a data byte to the LCD controller.
 *  @param Data	Data byte
 */
static void HD44780_SendData(const uint8_t Data)
{
	#if(defined(HD44780_USE_BUSY))
		while(HD44780_IsBusy());
	#else
		_delay_ms(1);
	#endif

	GPIO_Set(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));
	HD44780_WriteByte(Data);
}

void HD44780_Init(void)
{
	__HD44780_Entry = HD44780_CMD_ENTRY;
	__HD44780_Display = HD44780_CMD_DISPLAY;
	__HD44780_Cursor = HD44780_CMD_CURSOR;
	__HD44780_Func = HD44780_CMD_FUNC;

	GPIO_SetDirection(GET_PERIPHERAL(HD44780_BACKLIGHT), GET_INDEX(HD44780_BACKLIGHT), GPIO_DIRECTION_OUT);
	GPIO_SetDirection(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE), GPIO_DIRECTION_OUT);
	GPIO_SetDirection(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS), GPIO_DIRECTION_OUT);
	GPIO_SetDirection(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW), GPIO_DIRECTION_OUT);

	// Configure the data pins as output
	#if(defined(HD44780_USE_8BIT))
		GPIO_SetPortDirection(GET_PERIPHERAL(HD44780_DATA), 0xFF, GPIO_DIRECTION_OUT);
	#else
		GPIO_SetPortDirection(GET_PERIPHERAL(HD44780_DATA), 0x0F << GET_INDEX(HD44780_DATA), GPIO_DIRECTION_OUT);
	#endif

	HD44780_ClearData();
	GPIO_Clear(GET_PERIPHERAL(HD44780_ENABLE), GET_INDEX(HD44780_ENABLE));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RS), GET_INDEX(HD44780_RS));
	GPIO_Clear(GET_PERIPHERAL(HD44780_RW), GET_INDEX(HD44780_RW));

	// Reset the display
	HD44780_Reset();

	// Set the display lines
	#if(HD44780_LINES > 1)
		__HD44780_Func |= HD44780_FUNC_2LINE;
	#else
		__HD44780_Func &= ~HD44780_FUNC_1LINE;
	#endif
	HD44780_SendCommand(__HD44780_Func);

	// Set the entry options
	__HD44780_Entry |= HD44780_ENTRY_INCR | HD44780_ENTRY_SHIFT_OFF;
	HD44780_SendCommand(__HD44780_Entry);

	// Set the cursor options
	__HD44780_Cursor |= HD44780_CURSOR_CHARCTER_MODE | HD44780_CURSOR_POWER_ON | HD44780_CURSOR_MODE_MASK;
	HD44780_SendCommand(__HD44780_Cursor);

	// Set the display options
	__HD44780_Display |= HD44780_DISPLAY_ON;
	HD44780_SendCommand(__HD44780_Display);
	
	// Move the cursor to the home position and clear the display
	HD44780_ClearHome();
}

void HD44780_Clear(void)
{
	HD44780_SendCommand(HD44780_CMD_CLEAR);
	
	// The clear command needs a minimum of 6.2 ms.
	_delay_ms(7);
}

void HD44780_ClearHome(void)
{
	HD44780_Clear();
	HD44780_SendCommand(HD44780_CMD_RETURN_HOME);
}

void HD44780_Position(const uint8_t Line, const uint8_t Column)
{
	uint8_t Address = (0x01 << 0x07) | Column;

	if(Line == 1)
	{
		HD44780_SendCommand(Address + HD44780_ROW_1);
	}
	#if(HD44780_LINES > 2)
		else if(Line == 2)
		{
			HD44780_SendCommand(Address + HD44780_ROW_2);
		}
		else if(Line == 3)
		{
			HD44780_SendCommand(Address + HD44780_ROW_3);
		}
	#endif
	else
	{
		HD44780_SendCommand(Address + HD44780_ROW_0);
	}
}

void HD44780_WriteString(const char* Message)
{
	while(*Message) 
	{
		HD44780_SendData(*Message++);
	}
}

void HD44780_WriteDecimal(const uint32_t Value)
{
	uint32_t Temp = Value / 10;
	char Buffer[2];
	
	if(Temp)
	{
		HD44780_WriteDecimal(Temp);
	}
	
	Buffer[0] = 0x30 + (Value % 10);
	Buffer[1] = '\0';
	
	HD44780_WriteString(Buffer);
}

void HD44780_SwitchDisplay(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		__HD44780_Display |= HD44780_DISPLAY_ON;
	}
	else
	{
		__HD44780_Display |= HD44780_DISPLAY_OFF;
	}

	HD44780_SendCommand(__HD44780_Display);
}

void HD44780_SwitchCursor(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		__HD44780_Display |= HD44780_DISPLAY_CURSOR_ON;
	}
	else
	{
		__HD44780_Display |= HD44780_DISPLAY_CURSOR_OFF;
	}

	HD44780_SendCommand(__HD44780_Display);
}

void HD44780_SwitchBlink(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		__HD44780_Display |= HD44780_DISPLAY_BLINK_ON;
	}
	else
	{
		__HD44780_Display |= HD44780_DISPLAY_BLINK_OFF;
	}

	HD44780_SendCommand(__HD44780_Display);
}

#if(defined(HD44780_WITH_BACKLIGHT))
	void HD44780_SwitchBacklight(const Bool_t Enable)
	{
		if(Enable == TRUE)
		{
			#if(defined(HD44780_BACKLIGHT_ACTIVE_LOW))
				GPIO_Clear(GET_PERIPHERAL(HD44780_BACKLIGHT), GET_INDEX(HD44780_BACKLIGHT));
			#else
				GPIO_Set(GET_PERIPHERAL(HD44780_BACKLIGHT), GET_INDEX(HD44780_BACKLIGHT));
			#endif
		}
		else
		{
			#if(defined(HD44780_BACKLIGHT_ACTIVE_LOW))
				GPIO_Set(GET_PERIPHERAL(HD44780_BACKLIGHT), GET_INDEX(HD44780_BACKLIGHT));
			#else
				GPIO_Clear(GET_PERIPHERAL(HD44780_BACKLIGHT), GET_INDEX(HD44780_BACKLIGHT));
			#endif
		}
	}
#endif