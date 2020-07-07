/*
 * Config_IR.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for NEC IR interface example.

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

/** @file configs/Config_IR.h
 *  @brief Configuration example for the IR interface.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_IR_H_
#define CONFIG_IR_H_

 #include "Common/Common.h"

 #define IR_INPUT									PORTE, 0
 #define IR_TIMEOUT									100							/**< Timeout time for a new message in µs. */

 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */

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

#endif /* CONFIG_IR_H_ */