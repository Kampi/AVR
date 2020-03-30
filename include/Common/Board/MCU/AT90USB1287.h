/*
 * AT90USB1287.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: MCU definitions for Atmel AT90USB1287.

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

/** @file Common/Board/MCU/AT90USB1287.h
 *  @brief MCU specific definitions for the AT90USB1287.
 *
 *  @author Daniel Kampert
 */

#ifndef AT90USB1287__H_
#define AT90USB1287__H_

 #include "Definitions.h"

 /*
	ADC
 */
 #define ADC_DEVICES									1							/**< ADC device count in MCU */
 #define ADC_CHANNEL									1							/**< Channels per ADC device */
 #define ADC_RESOLUTION									10							/**< ADC resolution in bits */

 /*
	SPI
 */
 #define SPI_PORT										&PORTB						/**< SPI port */
 #define SPI_DEVICES									1							/**< SPI device count in MCU */
 #define SPI_SS_PIN										0							/**< Pin number of SPI SS pin */
 #define SPI_MOSI_PIN									2							/**< Pin number of SPI MOSI pin */
 #define SPI_MISO_PIN									3							/**< Pin number of SPI MISO pin */
 #define SPI_SCK_PIN									1							/**< Pin number of SPI SCK pin */

 /*
	USB
 */
 #define USB_MAX_ENDPOINTS								7

#endif /* AT90USB1287__H_ */