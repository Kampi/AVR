/*
 * HW_Config.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware configuration file for the Ethernet service.

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

/** @file Services/Ethernet/HW_Config.c
 *  @brief Hardware configuration file for the Ethernet service.
 *
 *  @author Daniel Kampert
 */

#include "Services/Ethernet/Socket.h"

/** @brief Ethernet controller configuration object.
 */
EthernetConfig_t _EthernetControllerConfig = {
	.Submask = {255, 255, 0, 0},
	.IP = {169, 254, 133, 121},
	.Gateway = {169, 154, 133, 129},
	.MAC = {0x00, 0x00, 0x00, 0x00, 0x00, 0xAA}
};

/** @brief SPI master configuration object.
 */
#if(defined USE_W5100)
	SPIM_Config_t _EthernetConfig = {
		.Device = &W5100_INTERFACE,
		.Mode = SPI_MODE_0,
		.Prescaler = SPI_PRESCALER_64,
	};
#endif