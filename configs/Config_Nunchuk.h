/*
 * Config_Nunchuk.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Nintendo Nunchuk.

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

/** @file configs/Config_Nunchuk.h
 *  @brief Configuration example for the Nintendo Nunchuk.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_NUNCHUK_H_
#define CONFIG_NUNCHUK_H_

 #include "Common/Common.h"

 #define F_CPU										32000000					/**< CPU clock in MHz. */
 #define TWI_BUFFER_SIZE							32							/**< Size of TWI buffer in bytes. */
 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #define NUNCHUK_INTERFACE						TWIC						/**< I2C interface used by the Nunchuk. */
 #endif

 #define NUNCHUK_CLOCK								100000						/**< Clock used by the I2C interface. */

 #define USE_SSD1306
 #define SSD1306_INTERFACE_TYPE						INTERFACE_USART_SPI			/**< SPI interface type for the display. */
 #define SSD1306_INTERFACE							USARTD, 0					/**< SPI interface for the display. */
 #define SSD1306_CLOCK								8000000UL					/**< Display interface speed. */

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

#endif /* CONFIG_NUNCHUK_H_ */