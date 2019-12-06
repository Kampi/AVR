/*
 * Config.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Project configuration example for the XMega-C3 XPlained development board.

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

/** @file Config.h
 *  @brief Project configuration example for the XMega-C3 XPlained development board.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_H_
#define CONFIG_H_

 #define BOARD										BOARD_XMEGA384C3_XPLAINED	/**< Use the driver for the XMega-C3 XPlained board. */

 /*
	 LED
 */
 #define LED0										PORTR, 0					/**< Yellow LED0. */
 #define LED1										PORTR, 1					/**< Yellow LED1. */
 #define LED2										PORTD, 4					/**< Red/Green LED. */
 #define LED3										PORTD, 5					/**< Green LED. */
 
 /* 
	Push buttons
 */
 #define SW0										PORTF, 1					/**< Push button 0. */
 #define SW1										PORTF, 2					/**< Push button 1. */

 /*
	Reference voltage [mV]
 */
 #define REFERENCE_VOLTAGE							3300

 /*
	Analog sensors
 */
 #define LIGHT_SENSOR								0							/**< Analog light sensor. */
 #define TEMPERATURE_SENSOR							1							/**< Analog temperature sensor. */

 /*
	 On-board display
 */
 #define USE_SSD1306															/**< Use the SSD1306 driver. */

 #define SSD1306_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type for the display. */

 #define SSD1306_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type for the display. */
 #define DISPLAY_INTERFACE							USARTD, 0					/**< SPI interface for the display. */
 #define DISPLAY_CLOCK								1000000UL					/**< Display interface speed. */

 /*
	 Display interface connections
 */
 #define SSD1306_DATA								PORTD, 0					/**< Display DATA pin (used by display driver). */
 #define SSD1306_RESET								PORTA, 3					/**< Display RESET pin (used by display driver). */
 #define SSD1306_SS									PORTF, 3					/**< Display SS pin (used by display driver). */

 /*
	 Display settings
 */
 #define DISPLAY_WIDTH								128							/**< Width of the display in pixel. */
 #define DISPLAY_HEIGHT								32							/**< Height of the display in pixel. */
 #define DISPLAY_PIXEL_PER_BYTE						8							/**< Number of pixel which can fit into one byte. */

 /*
	SD/MMC card
 */
 #define SD_INTERFACE_TYPE							INTERFACE_USART_SPI
 #define SD_INTERFACE								USARTD, 0					/**< SPI interface for the SD card. */

 /*
	 SD/MMC interface connections
 */
 #define SD_SS										PORTE, 5					/**< Chip select pin used for the SD card. */
 
 #define SD_WITH_SWA															/**< Enable card detection. 
																					 NOTE: Must pulled low when SD card is available.
																					 NOTE: You have to enable interrupts! */
 #define SD_SWA										PORTE, 4					/**< SWA used by the SD card. */

 /*
	SD/MMC settings
 */
 #define SD_BLOCK_SIZE								512							/**< Block size for the SD card. */

#endif /* CONFIG_H_ */