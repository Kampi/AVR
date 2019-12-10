/*
 * HD44780.h
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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Peripheral/HD44780/HD44780.h
 *  @brief Driver for the Hitachi HD44780 LCD controller.
 *
 *  This file contains the prototypes and definitions for the HD44780 LCD controller.
 *
 *  @author Daniel Kampert
 *  @bug	- Check for BUSY doesn´t work
 */

#ifndef HD44780_H_
#define HD44780_H_

 #include "Config.h"
 #include "Macros.h"

 #include "GPIO/GPIO.h"
 
 #include <util/delay.h>

 #if((defined(HD44780_USE_4BIT) && defined(HD44780_USE_8BIT)) || (!defined(HD44780_USE_4BIT) && !defined(HD44780_USE_8BIT)))
	#error "Invalid bus width configuration for HD44780!"
 #endif

 #if((defined(HD44780_USE_BUS)) && (!defined(HD44780_FIRST)))
	 #error "Please specify first pin for HD44780 data bus!"
 #endif

 #if((!defined(HD44780_LINES)) || (!defined(HD44780_COLUMNS)))
	 #error "Please specify the dimensions of the HD44780 display!"
 #endif

 /** @brief	Initialize the HD44780 display controller.
  */
 void HD44780_Init(void);

 /** @brief	Clear the display.
  */
 void HD44780_Clear(void);

 /** @brief	Clear the display and return the cursor to the home position.
  */
 void HD44780_ClearHome(void);

 /** @brief			Set the cursor position.
  *	 @param Line	Display line
  *  @param Column	Display column
  */
 void HD44780_Position(const uint8_t Line, const uint8_t Column);

 /** @brief			Print a single character on the display.
  *	 @param Message	Pointer to message data 
  */
 void HD44780_WriteChar(const char Message);

 /** @brief			Print a string message on the display.
  *	 @param Message	Pointer to message data 
  */
 void HD44780_WriteString(const char* Message);
 
 /** @brief			Display a decimal on the display.
  *  @param Value	Decimal value
  */
 void HD44780_WriteDecimal(const uint32_t Value);

 /** @brief			Enable/disable the display.
  *  @param Enable	Enable/Disable
  */
 void HD44780_SwitchDisplay(const uint8_t Enable);

 /** @brief			Enable/disable the display cursor.
  *  @param Enable	Enable/Disable
  */
 void HD44780_SwitchCursor(const uint8_t Enable);

 /** @brief			Enable/disable the blinking of display cursor.
  *  @param Enable	Enable/Disable
  */
 void HD44780_SwitchBlink(const uint8_t Enable);

 #if(defined(HD44780_WITH_BACKLIGHT))
	 /** @brief			Enable/disable the backlight of the display.
	  *  @param Enable	Enable/Disable
	 */
	 void HD44780_SwitchBacklight(const uint8_t Enable);
 #endif

#endif /* HD44780_H_ */