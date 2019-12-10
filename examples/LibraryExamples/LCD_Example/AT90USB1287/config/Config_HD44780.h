/*
 * Config_HD44780.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Hitachi HD44780 LCD controller and the AT90USBKey.

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

/** @file config/Config_HD44780.h
 *  @brief Configuration example for the Hitachi HD44780 LCD controller and the AT90USBKey.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_HD44780_H_
#define CONFIG_HD44780_H_

 #include "Common/Common.h"

 /*
	 LCD data mode
 */
 #define HD44780_USE_4BIT													/**< Define this symbol to use the 4 bit mode of the display controller. */
 #undef HD44780_USE_8BIT													/**< Define this symbol to use the 8 bit mode of the display controller. */
 #undef HD44780_USE_BUSY													/**< Define this symbol to use the BUSY bit check instead of waiting a fixed time. */

 /*
	 LCD backlight
 */
 #undef HD44780_WITH_BACKLIGHT												/**< Define this symbol if a backlight is used. */
 #undef HD44780_BACKLIGHT_ACTIVE_LOW										/**< Define this symbol if a low state is used to switch on the backlight. */
 #define HD44780_BACKLIGHT						PORTB, 0					/**< LCD backlight pin. */

 /*
	 LCD control signals
 */
 #define HD44780_ENABLE							PORTC, 5					/**< LCD E pin. */
 #define HD44780_RS								PORTC, 7					/**< LCD RS pin. */
 #define HD44780_RW								PORTC, 6					/**< LCD RW pin. */
 #define HD44780_DATA							PORTD, 0					/**< First data line for the LCD. All other data lines must be connected 
																				 to the following port pins. The pin index is set to 0 when you use #HD44780_USE_8BIT. */

 #define F_CPU									8000000UL					/**< MCU clock frequency. */

 /*
	 LCD display dimensions
 */
 #define HD44780_LINES			2											/**< Display rows. */
 #define HD44780_COLUMNS		16											/**< Display columns. */

#endif /* CONFIG_HD44780_H_ */