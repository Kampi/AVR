/*
 * Config_DisplayManager.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the display manager.

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

/** @file Config/Config_DisplayManager.h
 *  @brief Configuration file for the display manager.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_SSD1306_H_
#define CONFIG_SSD1306_H_

 #include "Common/Common.h"
 
 #define WITH_SSD1306
 
 #define DISPLAY_INTERFACE_TYPE						INTERFACE_USART_SPI
 #define DISPLAY_INTERFACE							&USARTD0
 #define DISPLAY_CLOCK								1000000

 // OLED interface definitions
 #define SSD1306_DATA_CMD_PORT						&PORTD
 #define SSD1306_DATA_CMD_PIN						0
 #define SSD1306_RESET_PORT							&PORTA
 #define SSD1306_RESET_PIN							3
 #define SSD1306_SS_PORT							&PORTF
 #define SSD1306_SS_PIN								3

 // Display definitions
 #define DISPLAY_WIDTH								128
 #define DISPLAY_HEIGHT								32
 #define DISPLAY_BYTES_PER_PIXEL					8

/*
 #define WITH_ST7565R

 #define DISPLAY_INTERFACE_TYPE						INTERFACE_USART_SPI
 #define DISPLAY_INTERFACE							&USARTD0
 #define DISPLAY_CLOCK								1000000

 // LCD interface definitions
 #define ST7565R_REGISTER_SELECT_PORT				&PORTD
 #define ST7565R_REGISTER_SELECT_PIN				0
 #define ST7565R_RESET_PORT							&PORTA
 #define ST7565R_RESET_PIN							3
 #define ST7565R_BACKLIGHT_PORT						&PORTE
 #define ST7565R_BACKLIGHT_PIN						4
 #define ST7565R_SS_PORT							&PORTF
 #define ST7565R_SS_PIN								3

 // Display definitions
 #define DISPLAY_WIDTH								128
 #define DISPLAY_HEIGHT								32
 #define DISPLAY_BYTES_PER_PIXEL					8
 #define DISPLAY_CONTRAST_MAX						40
 #define DISPLAY_CONTRAST_MIN						30
*/
 
#endif /* CONFIG_SSD1306_H_ */