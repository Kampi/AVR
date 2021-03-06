/*
 * Config_DisplayManager.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the display manager.

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

/** @file configs/Config_DisplayManager.h
 *  @brief Configuration example for the display manager.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_DISPLAYMANAGER_H_
#define CONFIG_DISPLAYMANAGER_H_

 #include "Common/Common.h"
 
 #define USE_SSD1306															/**< Use the SSD1306 display with the display manager. */
 
 // OLED interface definitions
 #define SSD1306_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< Interface type used by the display. */
 #define DISPLAY_INTERFACE							USARTD, 0					/**< USART interface used by display. */
 #define DISPLAY_CLOCK								1000000UL					/**< Display interface speed. */

 #define SSD1306_DATA								PORTD, 0					/**< Display DATA pin (used by display driver). */
 #define SSD1306_RESET								PORTA, 3					/**< Display RESET pin (used by display driver). */
 #define SSD1306_SS									PORTF, 3					/**< Display SS pin (used by display driver). */

 // Display definitions
 #define DISPLAY_WIDTH								128							/**< Width of the display in pixel. */
 #define DISPLAY_HEIGHT								32							/**< Height of the display in pixel. */
 #define DISPLAY_PIXEL_PER_BYTE						8							/**< Number of pixel which can fit into one byte. */

/*
 #define USE_ST7565R

 #define ST7565R_INTERFACE_TYPE						INTERFACE_USART_SPI
 #define DISPLAY_INTERFACE							USARTD, 0
 #define DISPLAY_CLOCK								1000000

 // LCD interface definitions
 #define ST7565R_REGISTER_SELECT					PORTD, 0
 #define ST7565R_RESET								PORTA, 3
 #define ST7565R_BACKLIGHT							PORTE, 4
 #define ST7565R_SS									PORTF, 3

 // Display definitions
 #define DISPLAY_WIDTH								128
 #define DISPLAY_HEIGHT								32
 #define DISPLAY_PIXEL_PER_BYTE						8
 #define DISPLAY_CONTRAST_MAX						40
 #define DISPLAY_CONTRAST_MIN						30
*/ 
 
#endif /* CONFIG_DISPLAYMANAGER_H_ */