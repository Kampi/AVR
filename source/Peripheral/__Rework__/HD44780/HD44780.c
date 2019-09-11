/*
 * HD44780.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for HD44780 LCD-Controller

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

#include "Peripheral/HD44780/HD44780.h"

void Enable(void)
{
	LCD_PORT_E |= (1 << LCD_E);
	_delay_us(1);
	LCD_PORT_E &= ~(1 << LCD_E);
	_delay_us(4100);
}

// Clear the LCD Data Port by setting all signals to low
void Clear_LCD_Data(void)
{
	#if(MODUS == 4)
		LCDATA &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	#elif(MODUS == 8)
		LCDATA &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4) | (1 << DB3) | (1 << DB2) | (1 << DB1) | (1 << DB0));
	#endif
}

// Initialization of the LCD
void LCD_Init(void)
{
	_delay_ms(15);
	LCD_DDR_RS |= (1 << LCD_RS);
	LCD_DDR_E |= (1 << LCD_E);
	
	#if(MODUS == 4)
		LCD_DDR_DATA |= (1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4);
	#elif(MODUS == 8)
		LCD_DDR_DATA |= (1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4) | (1 << DB3) | (1 << DB2) | (1 << DB1) | (1 << DB0);
	#endif

	// Power On
	Clear_LCD_Data();
	LCDATA |= 0x30;
	Enable();
	Enable();
	Enable();

	// Umschalten auf 4-Bit Interface
	Clear_LCD_Data();
	LCDATA |= 0x20;
	Enable();

	// Funktion setzen
	LCD_Command(0x28);

	// Return Home
	LCD_Command(HOME);

	// Display an
	LCD_Command((1 << DB3) | DISPLAY_ON);
}

// Configure MCU for using Backlight
void LCD_Enable_Backlight(void)
{
	LCD_DDR_BACK |= (1 << LCD_BACK);
}

// Turn on Backlight
void LCD_Backlight_On(void)
{
	LCD_PORT_BACK |= (1 << LCD_BACK);
}

// Turn off Backlight
void LCD_Backlight_Off(void)
{
	LCD_PORT_BACK &= ~(1 << LCD_BACK);
}

// Write a single Character to the LCD
// Data -> The given Character for the LCD to write
void LCD_Data(char Data)
{
	LCD_PORT_RS |= (1 << LCD_RS);
	
	#if(MODUS == 4)
		Clear_LCD_Data();
		LCDATA |= (Data & 0xF0);
		Enable();
		Clear_LCD_Data();
		LCDATA |= (Data << 4);
		Enable();
	#elif(MODUS == 8)
	
	#endif
}

// Send a command to the LCD
// Command -> Instruction for the LCD Controller
void LCD_Command(char Command)
{
	LCD_PORT_RS &= ~(1 << LCD_RS);
	
	#if(MODUS == 4)
		Clear_LCD_Data();
		LCDATA |= (Command & 0xF0);
		Enable();
		Clear_LCD_Data();
		LCDATA |= (Command << 0x04);
		Enable();
	#elif(MODUS == 8)
	
	#endif
}	

// Clear the Display and set the Cursor to his Home-Position
void LCD_ClrHome(void)
{
	LCD_Command(CLEAR);
}

void LCD_100(uint8_t Value)
{
	LCD_Data((Value / 100) + 0x30);
}

void LCD_10(uint8_t Value)
{
	LCD_Data(((Value % 100) / 10) + 0x30);
}

void LCD_1(uint8_t Value)
{
	LCD_Data(((Value % 100) % 10) + 0x30);
}

// Display a Hex-Value on the LCD
// Value -> Given Value for the LCD
void LCD_Dec(uint8_t Value)
{
	if(Value > 99)
	{
		LCD_100(Value);
		LCD_10(Value);
		LCD_1(Value);
	}
	else if(Value > 9)
	{
		LCD_10(Value);
		LCD_1(Value);
	}
	else
	{
		LCD_1(Value);
	}

}

// Print a float Value on the LCD
// Value -> Input float Value
void LCD_PrFloat(float Value)
{
	LCD_Dec(Value);
	LCD_Data(',');
	LCD_Dec((Value * 100) - (((uint8_t)Value) * 100));
}

// Set the Cursor to the given Position
// Line -> Line of the Display
// Column -> Column of the Display
void LCD_Position(uint8_t Line, uint8_t Column)
{
	if(Line == 0)
	{
		LCD_Command(DD_RAM + Column + ZEILE_0);
	}
	else if(Line == 1)
	{
		LCD_Command(DD_RAM + Column + ZEILE_1);
	}
	else if(Line == 2)
	{
		LCD_Command(DD_RAM + Column + ZEILE_2);
	}
	else
	{
		LCD_Command(DD_RAM + Column + ZEILE_3);
	}
}

// Print out a string on the LCD
// String -> Input String for the LCD
void LCD_PrString(char *String)
{
	while(*String) LCD_Data(*String++);	
}

// Print a given time in the Display
// Hour -> Input Hour 
// Minute -> Input Minute
// Second -> Input Seconds
void LCD_Time(uint8_t Hour, uint8_t Minute, uint8_t Second)
{
	LCD_Dec(Hour);
	LCD_Data(':');
	LCD_Dec(Minute);
	LCD_Data(':');
	LCD_Dec(Second);
}

// Print a given data in the Display
// Day -> Input Day
// Month -> Input Month
// Year -> Input Year
void LCD_Date(uint8_t Day, uint8_t Month, uint8_t Year)
{
	LCD_Dec(Day);
	LCD_Data('.');
	LCD_Dec(Month);
	LCD_Data('.');
	LCD_Dec(Year);
}