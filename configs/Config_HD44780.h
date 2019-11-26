/*
 * Config_HD44780.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Hitachi HD44780 LCD controller.

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

/** @file configs/Config_HD44780.h
 *  @brief Configuration example for the Hitachi HD44780 LCD controller.
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

 /*
	 LCD backlight
 */
 #undef HD44780_WITH_BACKLIGHT												/**< Define this symbol if a backlight is used. */
 #undef HD44780_BACKLIGHT_ACTIVE_LOW										/**< Define this symbol if a low state is used to switch on the backlight. */
 #define HD44780_BACKLIGHT						PORTR, 0					/**< LCD backlight pin. */

 /*
	 LCD control signals
 */
 #define HD44780_ENABLE							PORTB, 2					/**< LCD E pin. */
 #define HD44780_RS								PORTB, 0					/**< LCD RS pin. */
 #define HD44780_RW								PORTB, 1					/**< LCD RW pin. */

 #define HD44780_USE_BUS													/**< Define this symbol to use a complete bus for the data pins. */
 #define HD44780_FIRST							PORTA, 4					/**< Use this symbol to mark the first pin for the data signals. 
																				 All other data signals must be connected to the following port pins.
																				 Only needed when #HD44780_USE_4BIT is defined. The pin index is set
																				 to 0 when you use #HD44780_USE_8BIT. */

 #if(!defined(HD44780_USE_BUS))
	 #define HD44780_DB7						PORTA, 7					/**< LCD D7 pin. */
	 #define HD44780_DB6						PORTA, 6					/**< LCD D6 pin. */
	 #define HD44780_DB5						PORTA, 5					/**< LCD D5 pin. */
	 #define HD44780_DB4						PORTA, 4					/**< LCD D4 pin. */
	 #define HD44780_DB3						PORTE, 0					/**< LCD D3 pin. */
	 #define HD44780_DB2						PORTE, 0					/**< LCD D2 pin. */
	 #define HD44780_DB1						PORTE, 0					/**< LCD D1 pin. */
	 #define HD44780_DB0						PORTE, 0					/**< LCD D0 pin. */
 #endif

 #define F_CPU									2000000UL					/**< MCU clock frequency. */

 /*
	 LCD display dimensions
 */
 #define HD44780_LINES			2											/**< Display rows. */
 #define HD44780_COLUMNS		16											/**< Display columns. */

#endif /* CONFIG_HD44780_H_ */