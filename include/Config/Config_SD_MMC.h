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
 *  @brief Example configuration file for the SD/MMC interface.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_SD_MMC_H_
#define CONFIG_SD_MMC_H_

 #include "Common/Common.h"
 
 /*
	SD/MMC interface
 */
 #define SD_INTERFACE_TYPE						INTERFACE_USART_SPI
 #define SD_INTERFACE							USARTD, 0				/**< SPI interface for the SD card. */

 /*
	 SD/MMC interface connections
 */
 #define SD_SS									PORTE, 5				/**< Chip select used by the SD card. */
 
 #define SD_WITH_SWA													/**< Enable card detection. 
																			 NOTE: Must pulled low when SD card is 
																			 available.
																			 NOTE: You have to enable interrupts! */
 #define SD_SWA									PORTE, 4				/**< SWA used by the SD card. */

 /*
	SD/MMC settings
 */
 #define SD_BLOCK_SIZE							512						/**< Block size for the SD card. */

#endif /* CONFIG_SD_MMC_H_ */