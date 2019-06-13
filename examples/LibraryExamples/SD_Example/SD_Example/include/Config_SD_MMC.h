/*
 * Config_SD_MMC.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the SD/MMC interface.

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

/** @file Config/Config_SD_MMC.h
 *  @brief Configuration file for the SD/MMC interface.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_SD_MMC_H_
#define CONFIG_SD_MMC_H_

 #include "Common/Common.h"
 
 #define SD_INTERFACE_TYPE						INTERFACE_USART_SPI		/**< Interface type for the SD card. */
 #define SD_INTERFACE							&USARTD0				/**< SPI interface for the SD card. */

 /*
	 SD/MMC interface definitions
 */
 #define SD_SS_PORT								&PORTE					/**< Chip select port for the SD card. */
 #define SD_SS_PIN								5						/**< Chip select pin for the SD card. */
 #define SD_SWA_PORT							&PORTE					/**< SWA port for the SD card. */
 #define SD_SWA_PIN								4						/**< SWA pin for the SD card. */

 #define SD_BLOCK_SIZE							512

#endif /* CONFIG_SD_MMC_H_ */