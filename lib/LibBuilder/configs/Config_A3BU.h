/*
 * Config_A3BU.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Project configuration file.

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

/** @file configs/Config_A3BU.h
 *  @brief Project configuration file.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_A3BU_H_
#define CONFIG_A3BU_H_

 #define BOARD										BOARD_XMEGA256A3_XPLAINED

 /*
	 Main clock
 */
 #define F_CPU										32000000UL					/**< MCU clock frequency. */

 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */
 #define SPI_BUFFER_SIZE							32							/**< Size of SPI buffer in bytes. */
 #define TWI_BUFFER_SIZE							32							/**< Size of TWI buffer in bytes. */
 #define DMA_BUFFER_SIZE							32							/**< Size of the DMA buffer. */
 
 /*
	 On-board display
 */
 #define USE_ST7565R															/**< Use the ST7565R display for the display manager service. */

 /*
	 Display manager settings
 */
 #define DISPLAY_WIDTH								128
 #define DISPLAY_HEIGHT								32
 #define DISPLAY_PIXEL_PER_BYTE						8
 #define DISPLAY_CONTRAST_MAX						40
 #define DISPLAY_CONTRAST_MIN						30

 /*
	 On-board flash
 */
 #define USE_AT45DB642D

 /*
	 Display interface settings
 */
 #define ST7565R_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type for the display. */
 #define ST7565R_INTERFACE							USARTD, 0					/**< SPI interface for the display. */
 #define ST7565R_CLOCK								1000000UL					/**< SPI interface speed. */

 #define ST7565R_REGISTER_SELECT					PORTD, 0
 #define ST7565R_RESET								PORTA, 3
 #define ST7565R_BACKLIGHT							PORTE, 4
 #define ST7565R_SS									PORTF, 3

 /*
	 Flash interface settings
 */
 #define AT45DB642D_INTERFACE_TYPE					INTERFACE_USART_SPI			/**< SPI interface type for the display. */
 #define AT45DB642D_INTERFACE						USARTD, 0					/**< SPI interface for the display. */
 #define AT45DB642D_CLOCK							1000000UL					/**< SPI interface speed. */

#endif /* CONFIG_A3BU_H_ */
