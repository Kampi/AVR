/*
 * HD44780.h
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

#ifndef HD44780_H_
#define HD44780_H_

 // TODO: Rework
 #include <avr/io.h>
 #include <util/delay.h>

 /*
	Display Settings
 */
 #define MODUS			4
 #define LINES			4
 #define COLUMNS		16

 /*
	Back light
 */
 #define LCD_BACK		PC2
 #define LCD_DDR_BACK	DDRC
 #define LCD_PORT_BACK	PORTC

 /*
	Enable
 */
 #define LCD_E  		PC1
 #define LCD_DDR_E 		DDRC
 #define LCD_PORT_E		PORTC

 /*
	RS
 */
 #define LCD_RS 		PC0
 #define LCD_DDR_RS		DDRC
 #define LCD_PORT_RS	PORTC

 /*
	Data
 */
 #define DB7			7
 #define DB6			6
 #define DB5			5
 #define DB4			4
 #define DB3			3
 #define DB2			2
 #define DB1			1
 #define DB0			0
 #define LCD_DDR_DATA 	DDRC
 #define LCDATA 		PORTC

 /*
	LCD Commands
 */
 #define CLEAR			(1 << DB0)
 #define HOME			(1 << DB1)
 #define DISPLAY_ON		(1 << DB2)
 #define CURSOR			(1 << DB1)
 #define BLINK			(1 << DB0)
 #define DISPLAY_SHIFT	(1 << DB3)
 #define Pix_5x10		(1 << DB2)
 #define Pix_5x12		~(1 << DB2)

 #if MODUS == 4
	#define DATA_LENGHT	~(1 << DB4)
 #elif MODUS == 8
	#define DATA_LENGHT	(1 << DB4)
 #endif

 /*
	Addresses
 */
 #define CG_RAM			(1 << DB6)
 #define DD_RAM			(1 << DB7)

 /*
	LCD Lines
 */
 #define ZEILE_0		0
 #define ZEILE_1		0x40
 #define ZEILE_2		(ZEILE_0 + COLUMNS)
 #define ZEILE_3		(ZEILE_1 + COLUMNS)

void Enable(void);
void Clear_LCD_Data(void);
void LCD_Init(void);
void LCD_ClrHome(void);
void LCD_Data(char Data);
void LCD_Command(char Command);
void LCD_PrString(char *String);
void LCD_100(uint8_t Value);
void LCD_10(uint8_t Value);
void LCD_1(uint8_t Value);
void LCD_Dec(uint8_t Value);
void LCD_PrFloat(float Value);
void LCD_Position(uint8_t Line, uint8_t Column);
void LCD_Date(uint8_t Day, uint8_t Month, uint8_t Year);
void LCD_Time(uint8_t Hour, uint8_t Minute, uint8_t Second);
void LCD_Enable_Backlight(void);
void LCD_Backlight_On(void);
void LCD_Backlight_Off(void);

#endif /* HD44780_H_ */