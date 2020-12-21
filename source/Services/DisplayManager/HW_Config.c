/*
 * HW_Config.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware configuration file for the display manager service.

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

/** @file Services/DisplayManager/HW_Config.c
 *  @brief Hardware configuration file for the display manager service.
 *
 *  @author Daniel Kampert
 */

#include "Services/DisplayManager/DisplayManager.h"

/** @brief SPI master configuration object.
 */
#if((defined USE_SSD1306) || (defined USE_ST7565R))
	SPIM_Config_t _DisplayManagerConfig = {
		.SPIClock = DISPLAY_CLOCK,
		.DataOrder = SPI_DATAORDER_MSB_FIRST,
		.Mode = SPI_MODE_0,
		.Device = &CONCAT(DISPLAY_INTERFACE)
	};
#endif
