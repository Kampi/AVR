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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file configs/Config_IR.h
 *  @brief Configuration example for the NEC IR interface.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_IR_H_
#define CONFIG_IR_H_

 #include "Common/Common.h"

 #define F_CPU										32000000UL					/**< CPU clock speed. */

 // IR receiver interface definitions
 #define IR_INTERRUPT_LEVEL							INT_LVL_HI					/**< Interrupt level used by the timer for the IR receiver. \n
																					 NOTE: You only need this when you use the XMega architecture! */
 #define IR_REC_INPUT								PORTE, 0					/**< IR receiver input. */
 #undef IR_USE_LED																/**< Define this symbol to use an external LED to indicate an incoming transmission. */
 #define IR_ACTIVE_LED								PORTE, 1					/**< I/O used by the external LED. \n
																					 NOTE: You only need this when you define the symbol #IR_USE_LED. */

 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */

 #define USE_ST7565R															/**< Use the ST7565R display with the display manager. */

 // LCD interface definitions
 #define ST7565R_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type used by the display. */
 #define ST7565R_INTERFACE							USARTD, 0					/**< SPI used by the display. */
 #define ST7565R_CLOCK								1000000UL					/**< Display interface speed. */

 // LCD control signals
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