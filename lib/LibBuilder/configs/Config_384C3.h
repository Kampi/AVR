/*
 * Config_384C3.h
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

/** @file configs/Config_384C3.h
 *  @brief Project configuration file.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_384C3_H_
#define CONFIG_384C3_H_

 #define BOARD										BOARD_XMEGA384C3_XPLAINED	/**< Use the driver for the XMega-C3 XPlained board. */

 /*
	 Main clock
 */
 #define F_CPU										32000000UL					/**< MCU clock frequency. */ 

 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */
 #define SPI_BUFFER_SIZE							32							/**< Size of SPI buffer in bytes. */
 #define TWI_BUFFER_SIZE							32							/**< Size of TWI buffer in bytes. */

 /*
	 On-board display
 */
 #define USE_SSD1306															/**< Use the SSD1306 display for the display manager service. */

 /*
	 Display manager settings
 */
 #define DISPLAY_WIDTH								128							/**< Width of the display in pixel. */
 #define DISPLAY_HEIGHT								32							/**< Height of the display in pixel. */
 #define DISPLAY_PIXEL_PER_BYTE						8							/**< Number of pixel which can fit into one byte. */

 /*
	 Display interface settings
 */
 #define SSD1306_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type for the display. */
 #define SSD1306_INTERFACE							USARTD, 0					/**< SPI interface for the display. */
 #define SSD1306_CLOCK								1000000UL					/**< SPI interface speed. */

 #define SSD1306_DATA								PORTD, 0					/**< Display DATA pin (used by display driver). */
 #define SSD1306_RESET								PORTA, 3					/**< Display RESET pin (used by display driver). */
 #define SSD1306_SS									PORTF, 3					/**< Display SS pin (used by display driver). */

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

#endif /* CONFIG_384C3_H_ */